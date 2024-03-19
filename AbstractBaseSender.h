#ifndef ONE_WAY_COMMUNICATOR_ABSTRACTBASESENDER_H
#define ONE_WAY_COMMUNICATOR_ABSTRACTBASESENDER_H

#include <thread>
#include <cmath>

#include "ControlFrame.h"
#include "CommonFrame.h"

class AbstractBaseSender {
protected:
    const char *mp_server;
    unsigned int m_port;
    bool m_is_init = false;
    bool m_is_working = true;
    bool m_is_high_speed = false;
    int m_file_frame_size;

    virtual void init() = 0;

    virtual void send(unsigned char *p_message, unsigned int buffer_size) = 0;

    virtual void sendData(unsigned char *p_data, unsigned int size) {
        if (!m_is_init) {
            init();
            m_is_init = true;
        }

        send(p_data, size);
    };


public:
    AbstractBaseSender(const char *p_server, unsigned int port, int file_frame_size, bool is_high_speed) :
            mp_server(p_server), m_port(port), m_is_high_speed(is_high_speed), m_file_frame_size(file_frame_size) {}


    virtual void stopReceivingSignal() {
        this->m_is_working = false;
    }

    void sendFile(unsigned long long t_file_id, const char *filename_name, const char *filename_path) {
        if (!this->m_is_working)
            return;

        std::ifstream in_file(filename_path, std::ios::binary | std::ios::ate);
        if (!in_file.is_open()) {
            std::cerr << "failed to open ";
            return;
        }

        int file_size = in_file.tellg();
        in_file.seekg(0, std::ios::beg);

        long long common_frame_number = (file_size / m_file_frame_size) + bool(file_size % m_file_frame_size);
        auto controlFrameData = ControlFrame(common_frame_number, t_file_id, filename_name, strlen(filename_name));

        sendData(controlFrameData.getData(), controlFrameData.getDataSize());


        char *message = new char[m_file_frame_size];
        for (int frame_count = 1; in_file.peek() != EOF && this->m_is_working; ++frame_count) {

            in_file.read(message, m_file_frame_size);
            CommonFrame *commonFrame;
            if (frame_count == common_frame_number && file_size % m_file_frame_size != 0) {
                commonFrame = new CommonFrame(frame_count, t_file_id, message, (file_size % m_file_frame_size));
            } else {
                commonFrame = new CommonFrame(frame_count, t_file_id, message, m_file_frame_size);
            }
            if (!m_is_high_speed) {
              //  std::cout << frame_count << std::endl;
            }


            sendData(commonFrame->getData(), commonFrame->getDataSize());
            delete commonFrame;
        }

        std::cout << "Done." << std::endl;
        in_file.close();
        delete[] message;
    };
};
#endif