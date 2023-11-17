#ifndef ONE_WAY_COMMUNICATOR_RECEIVERLINUX_H
#define ONE_WAY_COMMUNICATOR_RECEIVERLINUX_H


#include <bits/stdc++.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "../AbstractBaseReceiver.h"


class Receiver : public AbstractBaseReceiver {
protected:
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    int init() override {



        // Creating socket file descriptor
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
            std::cerr << ("socket creation failed") << std::endl;
            return -1;
        }

        memset(&servaddr, 0, sizeof(servaddr));
        memset(&cliaddr, 0, sizeof(cliaddr));

        // Filling server information
        servaddr.sin_family = AF_INET; // IPv4
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(PORT);

        if (bind(sockfd, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {

            std::cerr << ("bind failed") << std::endl;
            return -2;
        }
        return 0;
    }

    int receive(unsigned char *message, int buffer_size) override {


        socklen_t len = sizeof(cliaddr);

        // try to receive some data, this is a blocking call
        recvfrom(sockfd, (char *) message, buffer_size,
                 0, (struct sockaddr *) &cliaddr,
                 &len);


        return 0;
    }

    int closeConnection() override {
        close(sockfd);

        return 0;

    }


public:
    Receiver(int fileFrameSize, unsigned int port) : AbstractBaseReceiver(fileFrameSize, port) {}


};


#endif
