#ifndef ONE_WAY_COMMUNICATOR_RECEIVERLINUX_H
#define ONE_WAY_COMMUNICATOR_RECEIVERLINUX_H


#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "string"

#include "../AbstractBaseReceiver.h"


class Receiver : public AbstractBaseReceiver {
protected:
    int sockfd{};
    struct sockaddr_in servaddr{}, cliaddr{};

    void init() override {
        // Creating socket file descriptor
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
            throw std::runtime_error("Failed. Error Code: " + std::to_string(*strerror(errno)));
        }

        memset(&servaddr, 0, sizeof(servaddr));
        memset(&cliaddr, 0, sizeof(cliaddr));

        // Filling server information
        servaddr.sin_family = AF_INET; // IPv4
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(PORT);

        if (bind(sockfd, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {

            throw std::runtime_error("Failed. Error Code: " + std::to_string(*strerror(errno)));

        }

    }

    void receive(unsigned char *message, int buffer_size) override {
        socklen_t len = sizeof(cliaddr);

        // try to receive some data, this is a blocking call
        recvfrom(sockfd, (char *) message, buffer_size,
                 0, (struct sockaddr *) &cliaddr,
                 &len);

    }

    void closeConnection() override {
        close(sockfd);
    }


public:
    Receiver(int fileFrameSize, unsigned int port) : AbstractBaseReceiver(fileFrameSize, port) {}


};


#endif
