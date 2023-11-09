//
// Created by tiramisu on 02.11.2023.
//

#ifndef ONE_WAY_COMMUNICATOR_ABSTRACTBASERECEIVER_H
#define ONE_WAY_COMMUNICATOR_ABSTRACTBASERECEIVER_H


#include "iostream"
#include "map"
#include <fstream>

#include "CommonFrame.h"
#include "ControlFrame.h"


class AbstractBaseReceiver {
protected:
    int FRAME_SIZE{2048};
    int FILE_FRAME_SIZE;
    unsigned int PORT;

    virtual int init() = 0;

    virtual int receive(unsigned char *message, int buffer_size) = 0;

    virtual int closeConnection() = 0;


public:

    AbstractBaseReceiver(int fileFrameSize, unsigned int port) : FILE_FRAME_SIZE(fileFrameSize), PORT(port) {}


    int getFile(const char *filename) {
        this->init();

        auto *message = new unsigned char[FRAME_SIZE];
        std::map<unsigned int, CommonFrame *> commonFrameMap;
        ControlFrame *controlFrame = nullptr;
        std::cout << "Receiving started" << std::endl;
        while (true) {


            // try to receive some data, this is a blocking call

            if (this->receive(message, FRAME_SIZE) != 0) {
                return -4;
            }

            if (Frame::isControlFrame(message)) {
                controlFrame = new ControlFrame(message);
            } else {
                auto *commonFrame = new CommonFrame(message);
                commonFrameMap[commonFrame->getFrameNumber()] = commonFrame;
            }


            if (controlFrame != nullptr && commonFrameMap.size() >= controlFrame->getCommonFrameAmount())
                break;


        }

        char *output_filename;
        if (filename == nullptr) {
            output_filename = new char[controlFrame->getDataSize() - controlFrame->getAdditionalMemberSize() + 1];
            strncpy(output_filename,
                    reinterpret_cast<const char *>(controlFrame->getData() + controlFrame->getAdditionalMemberSize()),
                    controlFrame->getDataSize() - controlFrame->getAdditionalMemberSize());
            output_filename[controlFrame->getDataSize() - controlFrame->getAdditionalMemberSize()] = '\0';
            output_filename[controlFrame->getDataSize() - controlFrame->getAdditionalMemberSize() - 1] = '1';

        } else {
            output_filename = new char[strlen(output_filename)];
            strcpy(output_filename, filename);
        }
        std::ofstream output_file;
        output_file.open(output_filename, std::ios::binary);
        for (auto item: commonFrameMap) {
            output_file.write(reinterpret_cast<char *>(item.second->getData() + item.second->getAdditionalMemberSize()),
                              item.second->getDataSize() - item.second->getAdditionalMemberSize());
        }
        output_file.close();

        delete[] message;

        this->closeConnection();
        std::cout << "Done!" << std::endl;
        return 0;
    };

    int getFile() {
        return this->getFile(nullptr);
    }


};


#endif //ONE_WAY_COMMUNICATOR_ABSTRACTBASERECEIVER_H
