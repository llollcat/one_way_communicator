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
    int m_sockfd{};
    struct sockaddr_in m_servaddr{};
    struct sockaddr_in m_cliaddr{};

    void init() override {
        // Creating socket file descriptor
        if ((m_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
            throw std::runtime_error("Failed. Error Code: " + std::to_string(*strerror(errno)));
        }

        memset(&m_servaddr, 0, sizeof(m_servaddr));
        memset(&m_cliaddr, 0, sizeof(m_cliaddr));

        // Filling server information
        m_servaddr.sin_family = AF_INET; // IPv4
        m_servaddr.sin_addr.s_addr = INADDR_ANY;
        m_servaddr.sin_port = htons(m_port);

        if (bind(m_sockfd, (const struct sockaddr *) &m_servaddr, sizeof(m_servaddr)) < 0) {

            throw std::runtime_error("Failed. Error Code: " + std::to_string(*strerror(errno)));

        }

    }

    void receive(unsigned char *p_message, int buffer_size) override {
        socklen_t len = sizeof(m_cliaddr);

        // try to receive some data, this is a blocking call
        recvfrom(m_sockfd, (char *) p_message, buffer_size,
                 0, (struct sockaddr *) &m_cliaddr,
                 &len);

    }

    void closeConnection() override {
        close(m_sockfd);
    }


public:
    Receiver(int fileFrameSize, unsigned int port) : AbstractBaseReceiver(fileFrameSize, port) {}

};


#endif
