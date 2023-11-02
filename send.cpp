#include "Sender.h"
#include "ArgumentsGetter.h"
#include <fstream>
#include "iostream"
#include "CommonFrame.h"
#include "ControlFrame.h"


int main(int argc, char *argv[]) {
    ArgumentsGetter input(argc, argv);

    if (input.cmdOptionExists("-h") ||
        !(input.cmdOptionExists("-server") && input.cmdOptionExists("-port") && input.cmdOptionExists("-file") &&
          input.cmdOptionExists("-file-frame-size"))) {

        std::cout << "Program is send selected file to selected server by UDP.\n"
                     "Arguments: -server, -port, -file, -file-frame-size\n"
                     "Example: -server 127.0.0.1 -port 8888 -file s.txt -file-frame-size 512" << std::endl;
        return 0;
    }

    auto server = input.getCmdOption("-server").c_str();
    int port = stoi(input.getCmdOption("-port"));
    auto filename = input.getCmdOption("-file").c_str();
    int file_frame_size = stoi(input.getCmdOption("-file-frame-size"));


    std::ifstream in_file(filename, std::ios::binary | std::ios::ate);
    if (!in_file.is_open()) {
        std::cerr << "failed to open ";
        return -1;
    }
    long long file_size = in_file.tellg();
    in_file.seekg(0, std::ios::beg);

    long long  common_frame_number = (file_size / file_frame_size) + bool(file_size % file_frame_size);
    Sender sender = Sender(server, port);
    auto controlFrameData = ControlFrame(common_frame_number, filename, strlen(filename));


    sender.sendData(controlFrameData.getData(), controlFrameData.getDataSize());


    char *message = new char [file_frame_size];
    for (int frame_count = 1; in_file.peek() != EOF; ++frame_count) {
        in_file.read(message, file_frame_size);
        CommonFrame *commonFrame;
        if (frame_count == common_frame_number && file_size % file_frame_size !=0) {
            commonFrame = new CommonFrame(frame_count, message, (file_size % file_frame_size));
        } else{
            commonFrame = new CommonFrame(frame_count, message, file_frame_size);
        }

        sender.sendData(commonFrame->getData(), commonFrame->getDataSize());
        delete commonFrame;
    }

    delete[] message;

    return 0;
}

