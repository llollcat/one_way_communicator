#ifndef ONE_WAY_COMMUNICATOR_SENDERLINUX_H
#define ONE_WAY_COMMUNICATOR_SENDERLINUX_H


#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "iostream"

#include "../AbstractBaseSender.h"

class Sender : public AbstractBaseSender {
private:
    struct sockaddr_in servaddr{};
    int sockfd{};

    int init() override {
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }
        // Filling server information
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        servaddr.sin_addr.s_addr = inet_addr(SERVER);

        return 0;
    }

    int send(unsigned char *message, int message_size) override {


        sendto(sockfd, message, message_size,
               0, (const struct sockaddr *) &servaddr,
               sizeof(servaddr));


        return 0;

    }


public:
    Sender(const char *server, unsigned int port) : AbstractBaseSender(server, port) {}


    virtual ~Sender() {
        close(sockfd);
    }
};


#endif
