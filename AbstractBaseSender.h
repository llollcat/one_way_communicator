#ifndef ONE_WAY_COMMUNICATOR_ABSTRACTBASESENDER_H
#define ONE_WAY_COMMUNICATOR_ABSTRACTBASESENDER_H


class AbstractBaseSender {
protected:
    const char *mp_server;
    unsigned int m_port;
    bool is_init = false;

    virtual void init() = 0;

    virtual void send(unsigned char *p_message, int buffer_size) = 0;


public:
    AbstractBaseSender(const char *p_server, unsigned int port) : mp_server(p_server), m_port(port) {}


    virtual void sendData(unsigned char *p_data, int size) {
        if (!is_init) {
            init();
            is_init = true;

        }

        send(p_data, size);

    };


};


#endif
