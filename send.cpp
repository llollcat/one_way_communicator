
#include "Sender.h"
#include "FrameData.h"
#include "ArgumentsGetter.h"
#include <fstream>
#include <chrono>
#include <thread>
#include "iostream"

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
    int file_frame_size = stoi(input.getCmdOption("-file-frame-size"));  //  real size of frame is FILE_FRAME_SIZE+4




    std::ifstream in_file(filename, std::ios::binary);
    if (!in_file.is_open()) {
        std::cerr << "failed to open ";
        return -1;
    }

    in_file.seekg(0, std::ios::end);

    Sender sender = Sender(server, port);
    FrameData control_frameData = FrameData(in_file.tellg() / file_frame_size + 2, file_frame_size);
    in_file.seekg(0, std::ios::beg);


    sender.sendFrameData(control_frameData);


    char *message = new char [file_frame_size];
    for (int frame_num = 1; in_file.peek() != EOF; ++frame_num) {
        in_file.read(message, file_frame_size);
        FrameData frameData = FrameData(frame_num, message, file_frame_size);
        sender.sendFrameData(frameData);
    }
    delete[] message;

    return 0;
}