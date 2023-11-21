#ifndef ONE_WAY_COMMUNICATOR_RECEIVERWIN_H
#define ONE_WAY_COMMUNICATOR_RECEIVERWIN_H


#include <winsock2.h>
#include <ws2tcpip.h>

#include "iostream"
#include "map"
#include <fstream>

#include "../CommonFrame.h"
#include "../ControlFrame.h"
#include "../AbstractBaseReceiver.h"


class Receiver : public AbstractBaseReceiver {
protected:
    int sockaddr_len{};
    SOCKET server_socket{};
    sockaddr_in client{};

    void init() override {
        // initialise winsock
        WSADATA ws;
        std::cout << "Initialising Winsock..." << std::endl;
        if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
            throw std::runtime_error("Failed. Error Code: " + std::to_string(WSAGetLastError()));
        }
        std::cout << "Initialised." << std::endl;

        // create a socket
        server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        DWORD optValue = TRUE;
        setsockopt(server_socket, IPPROTO_UDP, UDP_CHECKSUM_COVERAGE, (const char *) &optValue, sizeof(optValue));
        DWORD optValue2 = FALSE;
        setsockopt(server_socket, IPPROTO_UDP, UDP_NOCHECKSUM, (const char *) &optValue2, sizeof(optValue2));

        if (server_socket == INVALID_SOCKET) {
            std::cerr << "socket() failed with error code: " << WSAGetLastError() << std::endl;
            throw std::runtime_error("Failed. Error Code: " + std::to_string(WSAGetLastError()));
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
            throw std::runtime_error("Failed. Error Code: " + std::to_string(WSAGetLastError()));
        }
        std::cout << "Bind done." << std::endl;


        sockaddr_len = sizeof(sockaddr_in);
    }

    void receive(unsigned char *message, int buffer_size) override {
        if (recvfrom(server_socket, reinterpret_cast<char *>(message), buffer_size, 0, (sockaddr *) &client,
                     &sockaddr_len) == SOCKET_ERROR) {
            std::cerr << "recvfrom() failed with error code: " << WSAGetLastError();
            throw std::runtime_error("Failed. Error Code: " + std::to_string(WSAGetLastError()));
        }
    }

    void closeConnection() override {
        closesocket(server_socket);
        WSACleanup();
    }

public:
    Receiver(int fileFrameSize, unsigned int port) : AbstractBaseReceiver(fileFrameSize, port) {}
};


#endif
