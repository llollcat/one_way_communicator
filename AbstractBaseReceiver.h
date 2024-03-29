#ifndef ONE_WAY_COMMUNICATOR_ABSTRACTBASERECEIVER_H
#define ONE_WAY_COMMUNICATOR_ABSTRACTBASERECEIVER_H

#include "iostream"
#include "map"
#include <fstream>

#include "CommonFrame.h"
#include "ControlFrame.h"
#include "FileWriter.h"


class AbstractBaseReceiver {
protected:
    int m_frame_full_size;
    int m_file_frame_size;
    volatile bool m_is_working = true;
    unsigned int m_port;

    virtual void init() = 0;

    virtual void receive(unsigned char *p_message, int buffer_size) = 0;

    virtual void closeConnection() = 0;


    static void
    writeToFile(const char *p_output_filename, const std::map<unsigned int, CommonFrame *> &r_commonFrameMap) {
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
            m_file_frame_size(fileFrameSize), m_port(port) {
        m_frame_full_size = m_file_frame_size + 32 + CommonFrame::COMMON_FRAME_ADDITIONAL_MEMBER_SIZE;
    }

    virtual void stopReceivingSignal() {
        this->m_is_working = false;
    }

    void getFiles() {
        this->init();

        auto *p_message = new unsigned char[this->m_frame_full_size];
        std::map<unsigned int, ControlFrame *> controlFrameMap;
        std::map<unsigned int, unsigned int> commonFrameCounterMap;
        std::cout << "Receiving started" << std::endl;

        FileWriter fileWriter(m_file_frame_size);
        while (this->m_is_working) {

            this->receive(p_message, this->m_frame_full_size);



            if (Frame::isControlFrame(p_message)) {
                auto controlFrame = new ControlFrame(p_message);
                auto fileId = controlFrame->getFileId();
                controlFrameMap[fileId] = controlFrame;

                if (controlFrame->getCommonFrameAmount() <= commonFrameCounterMap[fileId]) {
                    char *output_filename = new char[ControlFrame::ADDITIONAL_MEMBER_SIZE + 1];
                    strncpy(output_filename,
                            reinterpret_cast<const char *>(controlFrame->getData() +
                                                           ControlFrame::ADDITIONAL_MEMBER_SIZE),
                            controlFrame->getDataSize() - ControlFrame::ADDITIONAL_MEMBER_SIZE);
                    output_filename[controlFrame->getDataSize() - ControlFrame::ADDITIONAL_MEMBER_SIZE] = '\0';

                    fileWriter.closeFile(fileId, output_filename);


                    commonFrameCounterMap.erase(fileId);

                    delete controlFrameMap[fileId];
                    controlFrameMap.erase(fileId);
                }

            } else {
                auto commonFrame = new CommonFrame(p_message);
                auto fileId =commonFrame->getFileId();

                ++commonFrameCounterMap[fileId];

                fileWriter.addToFile(fileId,commonFrame->getFrameNumber(),
                                     commonFrame->getData() + CommonFrame::COMMON_FRAME_ADDITIONAL_MEMBER_SIZE,
                                     commonFrame->getDataSize() - CommonFrame::COMMON_FRAME_ADDITIONAL_MEMBER_SIZE);



                if (controlFrameMap.count(fileId)) {
                    auto controlFrame = controlFrameMap[fileId];
                    if (controlFrame->getCommonFrameAmount() <= commonFrameCounterMap[fileId]) {
                        char *output_filename = new char[ControlFrame::ADDITIONAL_MEMBER_SIZE + 1];
                        strncpy(output_filename,
                                reinterpret_cast<const char *>(controlFrame->getData() +
                                                               ControlFrame::ADDITIONAL_MEMBER_SIZE),
                                controlFrame->getDataSize() - ControlFrame::ADDITIONAL_MEMBER_SIZE);
                        output_filename[controlFrame->getDataSize() - ControlFrame::ADDITIONAL_MEMBER_SIZE] = '\0';

                        fileWriter.closeFile(fileId, output_filename);


                        commonFrameCounterMap.erase(fileId);

                        delete controlFrameMap[fileId];
                        controlFrameMap.erase(fileId);

                    }
                }
                // delete commonFrame??
            }
        }

        this->closeConnection();

        delete[] p_message;

    };
};

#endif