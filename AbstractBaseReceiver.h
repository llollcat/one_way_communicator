#ifndef ONE_WAY_COMMUNICATOR_ABSTRACTBASERECEIVER_H
#define ONE_WAY_COMMUNICATOR_ABSTRACTBASERECEIVER_H

#include "iostream"
#include "map"
#include <fstream>
#include <thread>

#include "CommonFrame.h"
#include "ControlFrame.h"


class AbstractBaseReceiver {
protected:
    int m_frame_full_size;
    volatile bool m_is_working = true;
    unsigned int m_port;

    virtual void init() = 0;

    virtual void receive(unsigned char *p_message, int buffer_size) = 0;

    virtual void closeConnection() = 0;


    static void writeToFile(const char *p_output_filename, const std::map<unsigned int, CommonFrame *> &r_commonFrameMap) {
        std::ofstream output_file;
        output_file.open(p_output_filename, std::ios::binary);
        for (auto item: r_commonFrameMap) {
            output_file.write(reinterpret_cast<char *>(item.second->getData() + CommonFrame::COMMON_FRAME_ADDITIONAL_MEMBER_SIZE),
                              item.second->getDataSize() - CommonFrame::COMMON_FRAME_ADDITIONAL_MEMBER_SIZE);
        }
        output_file.close();
        std::cout << "Done!" << std::endl;
    }

public:

    AbstractBaseReceiver(int fileFrameSize, unsigned int port) :
            m_frame_full_size(fileFrameSize), m_port(port) {}

    virtual void stopReceivingSignal() {
        this->m_is_working = false;
    }

    void getFiles() {
        this->init();

        auto *p_message = new unsigned char[this->m_frame_full_size];
        std::map<unsigned int, ControlFrame *> controlFrameMap;
        std::map<unsigned int, std::map<unsigned int, CommonFrame *>> commonFrameMap;
        std::cout << "Receiving started" << std::endl;
        while (this->m_is_working) {
            this->receive(p_message, this->m_frame_full_size);

            auto handler = [&]() {
                if (Frame::isControlFrame(p_message)) {
                    auto controlFrame = new ControlFrame(p_message);
                    controlFrameMap[controlFrame->getFileId()] = controlFrame;

                    if (controlFrame->getCommonFrameAmount() <= commonFrameMap[controlFrame->getFileId()].size()) {
                        char *output_filename = new char[ControlFrame::ADDITIONAL_MEMBER_SIZE + 1];
                        strncpy(output_filename,
                                reinterpret_cast<const char *>(controlFrame->getData() +
                                                               ControlFrame::ADDITIONAL_MEMBER_SIZE),
                                controlFrame->getDataSize() - ControlFrame::ADDITIONAL_MEMBER_SIZE);
                        output_filename[controlFrame->getDataSize() - ControlFrame::ADDITIONAL_MEMBER_SIZE] = '\0';

                        writeToFile(output_filename, commonFrameMap[controlFrame->getFileId()]);
                        commonFrameMap.erase(controlFrame->getFileId());
                    }

                } else {
                    auto *commonFrame = new CommonFrame(p_message);
                    commonFrameMap[commonFrame->getFileId()][commonFrame->getFrameNumber()] = commonFrame;
                    if (controlFrameMap.count(commonFrame->getFileId())) {
                        auto controlFrame = controlFrameMap[commonFrame->getFileId()];
                        if (controlFrame->getCommonFrameAmount() <= commonFrameMap[commonFrame->getFileId()].size()) {
                            char *output_filename = new char[ControlFrame::ADDITIONAL_MEMBER_SIZE + 1];
                            strncpy(output_filename,
                                    reinterpret_cast<const char *>(controlFrame->getData() +
                                                                   ControlFrame::ADDITIONAL_MEMBER_SIZE),
                                    controlFrame->getDataSize() - ControlFrame::ADDITIONAL_MEMBER_SIZE);
                            output_filename[controlFrame->getDataSize() - ControlFrame::ADDITIONAL_MEMBER_SIZE] = '\0';

                            writeToFile(output_filename, commonFrameMap[commonFrame->getFileId()]);
                            commonFrameMap.erase(commonFrame->getFileId());

                        }
                    }
                }
            };

            handler();




        }

        this->closeConnection();

        delete[] p_message;

    };
};

#endif