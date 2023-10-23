//
// Created by tiramisu on 19.10.2023.
//

#ifndef UNTITLED_SENDER_H
#define UNTITLED_SENDER_H

#include <bits/stdc++.h>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;


class Sender {
protected:
    int BUFLEN;
    const char *SERVER;
    unsigned int PORT;
    SOCKET client_socket;
    sockaddr_in server;

public:

    Sender(const int BUFLEN, const char *SERVER, const unsigned int PORT) {
        this->BUFLEN = BUFLEN;
        this->SERVER = SERVER;
        this->PORT = PORT;
    }

    int initConnection(){

        // initialise winsock
        WSADATA ws;
        cout << "Initialising Winsock..." << endl;
        if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
            cerr << "Failed. Error Code: " << WSAGetLastError() << endl;
            return 1;
        }
        cout << "Initialised." << endl;

        // create socket

        this->client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        DWORD optValue = TRUE;
        setsockopt(client_socket, IPPROTO_UDP, UDP_CHECKSUM_COVERAGE, (const char *) &optValue, sizeof(optValue));
        DWORD optValue2 = FALSE;
        setsockopt(client_socket, IPPROTO_UDP, UDP_NOCHECKSUM, (const char *) &optValue2, sizeof(optValue2));
        if (client_socket == SOCKET_ERROR) // <<< UDP socket
        {
            cerr << "socket() failed with error code: " << WSAGetLastError() << endl;
            return 2;
        }
        cout << "Socket created." << endl;


        // setup address structure

        this->server.sin_family = AF_INET;
        this->server.sin_port = htons(PORT);
        this->server.sin_addr.S_un.S_addr = inet_addr(SERVER);




    }

    int sendFrameData( FrameData& frameData) {

        // start communication



        if (sendto(this->client_socket, reinterpret_cast<char*>(frameData.getuCharData()), frameData.getSize(), 0, (sockaddr *) &this->server, sizeof(sockaddr_in)) ==
            SOCKET_ERROR) {
            cerr << "sendto() failed with error code: " << WSAGetLastError();
            return 3;
        }
        cout << "Frame send" << endl;




    }

    virtual ~Sender() {
        closesocket(this->client_socket);
        WSACleanup();
    }


};


#endif //UNTITLED_SENDER_H
