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
    struct sockaddr_in m_servaddr{};
    int m_sockfd{};

    void init() override {
        if ((m_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
            throw std::runtime_error("Failed. Error Code: " + std::to_string(*strerror(errno)));
        }
        // Filling server information
        m_servaddr.sin_family = AF_INET;
        m_servaddr.sin_port = htons(m_port);
        m_servaddr.sin_addr.s_addr = inet_addr(mp_server);

    }

    void send(unsigned char *p_message, int message_size) override {
        sendto(m_sockfd, p_message, message_size,
               0, (const struct sockaddr *) &m_servaddr,
               sizeof(m_servaddr));


    }


public:
    Sender(const char *p_server, unsigned int port, int file_frame_size, bool is_high_speed) :
            AbstractBaseSender(p_server, port, file_frame_size, is_high_speed) {}


    virtual ~Sender() {
        close(m_sockfd);
    }
};
#endif
