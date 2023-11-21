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

    void init() override {
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
            throw std::runtime_error("Failed. Error Code: " + std::to_string(*strerror(errno)));
        }
        // Filling server information
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        servaddr.sin_addr.s_addr = inet_addr(SERVER);

    }

    void send(unsigned char *message, int message_size) override {
        sendto(sockfd, message, message_size,
               0, (const struct sockaddr *) &servaddr,
               sizeof(servaddr));


    }


public:
    Sender(const char *server, unsigned int port) : AbstractBaseSender(server, port) {}


    virtual ~Sender() {
        close(sockfd);
    }
};


#endif
