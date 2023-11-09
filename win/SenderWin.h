//
// Created by tiramisu on 09.11.2023.
//

#ifndef ONE_WAY_COMMUNICATOR_SENDERWIN_H
#define ONE_WAY_COMMUNICATOR_SENDERWIN_H


#include <winsock2.h>
#include <ws2tcpip.h>
#include "iostream"

#include "../AbstractBaseSender.h"


class Sender : public AbstractBaseSender {
private:
    SOCKET client_socket{};
    sockaddr_in server{};

    int init() override {
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


    int send(unsigned char *message, int message_size) override {
        if (sendto(this->client_socket, reinterpret_cast<char *>(message), message_size, 0,
                   (sockaddr *) &this->server, sizeof(sockaddr_in)) == SOCKET_ERROR) {
            std::cerr << "sendto() failed with error code: " << WSAGetLastError();
            return -3;
        }
        return 0;
    }



public:
    Sender(const char *server, unsigned int port) : AbstractBaseSender(server, port) {}

    virtual ~Sender() {
        closesocket(this->client_socket);
        WSACleanup();
    }
};


#endif //ONE_WAY_COMMUNICATOR_SENDERWIN_H
