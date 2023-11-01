#ifndef ONE_WAY_COMMUNICATOR_RECEIVER_H
#define ONE_WAY_COMMUNICATOR_RECEIVER_H


#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>
#include "iostream"
#include "map"
#include <fstream>

#include "CommonFrame.h"
#include "ControlFrame.h"


class Receiver {
private:
    int FRAME_SIZE;
    int FILE_FRAME_SIZE;
    unsigned int PORT;
public:

    Receiver(const int FILE_FRAME_SIZE, const unsigned int PORT) {
        //todo изменить на динамический размер
        this->FILE_FRAME_SIZE = FILE_FRAME_SIZE;
        this->FRAME_SIZE = FILE_FRAME_SIZE + 2048;

        this->PORT = PORT;
    }

    int getFile(const char *output_filename) const {
        // initialise winsock
        WSADATA ws;
        std::cout << "Initialising Winsock..." << std::endl;
        if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
            std::cerr << "Failed. Error Code: " << WSAGetLastError() << std::endl;
            return -1;
        }
        std::cout << "Initialised." << std::endl;

        // create a socket
        auto server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        DWORD optValue = TRUE;
        setsockopt(server_socket, IPPROTO_UDP, UDP_CHECKSUM_COVERAGE, (const char *) &optValue, sizeof(optValue));
        DWORD optValue2 = FALSE;
        setsockopt(server_socket, IPPROTO_UDP, UDP_NOCHECKSUM, (const char *) &optValue2, sizeof(optValue2));

        if (server_socket == INVALID_SOCKET) {
            std::cerr << "socket() failed with error code: " << WSAGetLastError() << std::endl;
            return -2;
        }
        std::cout << "Socket created." << std::endl;

        // prepare the sockaddr_in structure
        sockaddr_in server{};

        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(PORT);

        // bind
        if (bind(server_socket, (sockaddr *) &server, sizeof(server)) == SOCKET_ERROR) {
            std::cerr << "Bind failed with error code: " << WSAGetLastError();
            return -3;
        }
        std::cout << "Bind done." << std::endl;


        sockaddr_in client{};


        int sockaddr_len = sizeof(sockaddr_in);

        auto *message = new unsigned char[FRAME_SIZE];
        std::map<unsigned int, CommonFrame*> commonFrameMap;
        ControlFrame *controlFrame = nullptr;
        std::cout << "Receiving started" << std::endl;
        while (true) {
            // try to receive some data, this is a blocking call
            if (recvfrom(server_socket, reinterpret_cast<char *>(message), FRAME_SIZE, 0, (sockaddr *) &client,
                         &sockaddr_len) == SOCKET_ERROR) {
                std::cerr << "recvfrom() failed with error code: " << WSAGetLastError();
                return -4;
            }

            if (Frame::isControlFrame(message)){
                controlFrame = new ControlFrame(message);
            }else {
                auto *commonFrame = new CommonFrame(message);
                commonFrameMap[commonFrame->getFrameNumber()] = commonFrame;
            }


            if (controlFrame != nullptr && commonFrameMap.size() >= controlFrame->getCommonFrameAmount())
                break;


        }

        std::ofstream output_file;
        output_file.open(output_filename, std::ios::binary);
        for (auto item :commonFrameMap) {
            output_file.write(reinterpret_cast<char *>(item.second->getData()+ item.second->getAdditionalMemberSize()) , FILE_FRAME_SIZE);

        }
        output_file.close();

        delete[] message;

        closesocket(server_socket);
        WSACleanup();

        std::cout << "Done!" << std::endl;
        return 0;
    };
};

#else


#endif


#endif
