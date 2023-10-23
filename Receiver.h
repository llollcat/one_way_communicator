#ifndef UNTITLED1_RECEIVER_H
#define UNTITLED1_RECEIVER_H

#include <bits/stdc++.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "FrameData.h"


class Receiver {
protected:
    int BUFLEN;
    unsigned int PORT;
public:

    Receiver(const int BUFLEN, const unsigned int PORT) {
        this->BUFLEN = BUFLEN;

        this->PORT = PORT;
    }

    int getFile(const char *output_filename) {


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


        bool is_having_control_frame = false;
        int slen = sizeof(sockaddr_in);

        auto *message = new unsigned char[BUFLEN];
        std::map<unsigned int, FrameData *> frameDataMap;
        while (true) {
            std::cout << "Waiting for mp_data..." << std::endl;

            // try to receive some mp_data, this is a blocking call
            if (recvfrom(server_socket, reinterpret_cast<char*>(message), BUFLEN, 0, (sockaddr *) &client, &slen) == SOCKET_ERROR) {
                std::cerr << "recvfrom() failed with error code: " << WSAGetLastError();
                return -4;
            }


            auto *frameData = new FrameData(message, BUFLEN);
            frameDataMap[frameData->getFrameNumber()] = frameData;
            is_having_control_frame |= frameData->getFrameNumber() == 0;




            if (is_having_control_frame && frameDataMap.size() >= frameDataMap[0]->getFrameAmount())
                break;


        }
        std::ofstream myfile;
        myfile.open(output_filename, std::ios::binary);
        for (auto item = ++frameDataMap.begin(); item != frameDataMap.end(); ++item) {
            myfile.write(reinterpret_cast<char*>(item->second->getUCharData()) + 4, item->second->getSize() - 36);


            std::cout << std::endl;
        }
        myfile.close();

        delete[] message;

        closesocket(server_socket);
        WSACleanup();


    };


};


#endif
