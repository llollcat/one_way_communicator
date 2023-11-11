#ifndef ONE_WAY_COMMUNICATOR_ABSTRACTBASESENDER_H
#define ONE_WAY_COMMUNICATOR_ABSTRACTBASESENDER_H


class AbstractBaseSender {
protected:
    const char *SERVER;
    unsigned int PORT;
    bool is_init = false;
    virtual int init() = 0;

    virtual int send(unsigned char *message, int buffer_size) = 0;


public:
    AbstractBaseSender(const char *server, unsigned int port) : SERVER(server), PORT(port) {}


    virtual int sendData(unsigned char *data, int size) {
        if (!is_init) {
            init();
            is_init = true;
        }

        send(data, size);

        return 0;
    };



};


#endif //ONE_WAY_COMMUNICATOR_ABSTRACTBASESENDER_H
