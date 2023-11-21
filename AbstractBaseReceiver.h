#ifndef ONE_WAY_COMMUNICATOR_ABSTRACTBASERECEIVER_H
#define ONE_WAY_COMMUNICATOR_ABSTRACTBASERECEIVER_H


#include "iostream"
#include "map"
#include <fstream>

#include "CommonFrame.h"
#include "ControlFrame.h"


class AbstractBaseReceiver {
protected:
    int m_frame_size;
    volatile bool m_is_working = true;
    unsigned int PORT;

    virtual void init() = 0;

    virtual void receive(unsigned char *message, int buffer_size) = 0;

    virtual void closeConnection() = 0;




    static void writeToFile(char *output_filename, const std::map<unsigned int, CommonFrame *> &commonFrameMap) {
        std::ofstream output_file;
        output_file.open(output_filename, std::ios::binary);
        for (auto item: commonFrameMap) {
            output_file.write(reinterpret_cast<char *>(item.second->getData() + item.second->getAdditionalMemberSize()),
                              item.second->getDataSize() - item.second->getAdditionalMemberSize());
        }
        output_file.close();
        std::cout << "Done!" << std::endl;

    }

public:

    AbstractBaseReceiver(int fileFrameSize, unsigned int port) : m_frame_size(fileFrameSize), PORT(port) {}

    virtual void stopReceivingSignal(){
        this->m_is_working = false;
    }

    void getFile(const char *filename) {
        this->init();

        auto *message = new unsigned char[m_frame_size];
        std::map<unsigned int, ControlFrame *> controlFrameMap;
        std::map<unsigned int, std::map<unsigned int, CommonFrame *>> commonFrameMap;
        std::cout << "Receiving started" << std::endl;
        while (this->m_is_working) {
            // try to receive some data, this is a blocking call

            this->receive(message, m_frame_size);


            if (Frame::isControlFrame(message)) {
                auto controlFrame = new ControlFrame(message);
                controlFrameMap[controlFrame->getFileId()] = controlFrame;

                if (controlFrame->getCommonFrameAmount() <= commonFrameMap[controlFrame->getFileId()].size()) {
                    char *output_filename = new char[controlFrame->getAdditionalMemberSize() + 1];
                    strncpy(output_filename,
                            reinterpret_cast<const char *>(controlFrame->getData() +
                                                           controlFrame->getAdditionalMemberSize()),
                            controlFrame->getDataSize() - controlFrame->getAdditionalMemberSize());
                    output_filename[controlFrame->getDataSize() - controlFrame->getAdditionalMemberSize()] = '\0';

                    writeToFile(output_filename, commonFrameMap[controlFrame->getFileId()]);
                    commonFrameMap.erase(controlFrame->getFileId());

                }



            } else {
                auto *commonFrame = new CommonFrame(message);
                commonFrameMap[commonFrame->getFileId()][commonFrame->getFrameNumber()] = commonFrame;
                if (controlFrameMap.count(commonFrame->getFileId())) {
                    auto controlFrame = controlFrameMap[commonFrame->getFileId()];
                    if (controlFrame->getCommonFrameAmount() <= commonFrameMap[commonFrame->getFileId()].size()) {
                        char *output_filename = new char[controlFrame->getAdditionalMemberSize() + 1];
                        strncpy(output_filename,
                                reinterpret_cast<const char *>(controlFrame->getData() +
                                                               controlFrame->getAdditionalMemberSize()),
                                controlFrame->getDataSize() - controlFrame->getAdditionalMemberSize());
                        output_filename[controlFrame->getDataSize() - controlFrame->getAdditionalMemberSize()] = '\0';

                        writeToFile(output_filename, commonFrameMap[commonFrame->getFileId()]);
                        commonFrameMap.erase(commonFrame->getFileId());

                    }

                }
            }


        }

       this->closeConnection();

        delete[] message;

    };

    void getFile() {
        this->getFile(nullptr);
    }


};


#endif
