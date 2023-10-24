#ifndef ONE_WAY_COMMUNICATOR_SENDER_H
#define ONE_WAY_COMMUNICATOR_SENDER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include "iostream"

#include "FrameData.h"


class Sender {
private:
    const char *SERVER;
    unsigned int PORT;
    SOCKET client_socket{};
    sockaddr_in server{};

    int initConnection() {
        // initialise winsock
        WSADATA ws;
        std::cout << "Initialising Winsock..." << std::endl;
        if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
            std::cerr << "Failed. Error Code: " << WSAGetLastError() << std::endl;
            return -1;
        }
        std::cout << "Initialised." << std::endl;

        // create socket

        this->client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        DWORD optValue = TRUE;
        setsockopt(client_socket, IPPROTO_UDP, UDP_CHECKSUM_COVERAGE, (const char *) &optValue, sizeof(optValue));
        DWORD optValue2 = FALSE;
        setsockopt(client_socket, IPPROTO_UDP, UDP_NOCHECKSUM, (const char *) &optValue2, sizeof(optValue2));
        std::cout << "Socket created." << std::endl;


        // setup address structure
        this->server.sin_family = AF_INET;
        this->server.sin_port = htons(PORT);
        this->server.sin_addr.S_un.S_addr = inet_addr(SERVER);
        return 0;
    }


public:
    Sender(const char *SERVER, const unsigned int PORT) {
        this->SERVER = SERVER;
        this->PORT = PORT;
        this->initConnection();
    }


    int sendFrameData(FrameData &frameData) {
        if (sendto(this->client_socket, reinterpret_cast<char *>(frameData.getUCharData()), frameData.getSize(), 0,
                   (sockaddr *) &this->server, sizeof(sockaddr_in)) == SOCKET_ERROR) {
            std::cerr << "sendto() failed with error code: " << WSAGetLastError();
            return -3;
        }

        std::cout << "Frame sent" << std::endl;
        return 0;
    }

    virtual ~Sender() {
        closesocket(this->client_socket);
        WSACleanup();
    }
};


#endif
