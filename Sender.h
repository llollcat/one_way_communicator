#ifndef ONE_WAY_COMMUNICATOR_SENDER_H
#define ONE_WAY_COMMUNICATOR_SENDER_H


#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>
#include "iostream"

#include "Frame.h"
#include "CommonFrame.h"


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


    int sendData(unsigned char *data, int size) {
        if (sendto(this->client_socket, reinterpret_cast<char *>(data), size, 0,
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

#else

#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>




#include "iostream"

#include "CommonFrame.h"


class Sender {
private:
    const char *SERVER;
    unsigned int PORT;
    struct sockaddr_in servaddr{};
    int sockfd{};
    int initConnection(){
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }


        // Filling server information
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        servaddr.sin_addr.s_addr = inet_addr(SERVER);

        return 0;
    }


public:
    Sender(const char *server, const unsigned int port) {
        this->SERVER = server;
        this->PORT = port;
        memset(&servaddr, 0, sizeof(servaddr));
        this->initConnection();
    }


    int sendData(unsigned char *data, int size) {


        sendto(sockfd, data, size,
               0, (const struct sockaddr *) &servaddr,
               sizeof(servaddr));
        std::cout<<"Frame sent."<<std::endl;


        return 0;
    }

    virtual ~Sender() {
        close(sockfd);
    }
};








#endif

#endif
