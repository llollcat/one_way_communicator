#ifndef ONE_WAY_COMMUNICATOR_ABSTRACTBASESENDER_H
#define ONE_WAY_COMMUNICATOR_ABSTRACTBASESENDER_H


class AbstractBaseSender {
protected:
    const char *SERVER;
    unsigned int PORT;
    bool is_init = false;

    virtual void init() = 0;

    virtual void send(unsigned char *message, int buffer_size) = 0;


public:
    AbstractBaseSender(const char *server, unsigned int port) : SERVER(server), PORT(port) {}


    virtual void sendData(unsigned char *data, int size) {
        if (!is_init) {
            init();
            is_init = true;

        }

        send(data, size);

    };


};


#endif
