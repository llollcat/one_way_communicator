#include "Receiver.h"
#include "ArgumentsGetter.h"



int main(int argc, char *argv[]) {
    ArgumentsGetter input(argc, argv);

    if (input.cmdOptionExists("-h") ||
        (!input.cmdOptionExists("-port") &&
         input.cmdOptionExists("-file-frame-size"))) {

        std::cout << "Program is receive file by UDP.\n"
                     "Arguments: -port, -file, -file-frame-size\n"
                     "Example: -port 8888 -file i.txt -file-frame-size 512" << std::endl;
        return 0;
    }
    int port = stoi(input.getCmdOption("-port"));
    int file_frame_size = stoi(input.getCmdOption("-file-frame-size"));  //  real size of frame is FILE_FRAME_SIZE+4


    Receiver receiver = Receiver(file_frame_size, port);

    if (input.cmdOptionExists("-file")){
        auto filename = input.getCmdOption("-file").c_str();
        receiver.getFile(filename);

    }else
        receiver.getFile("out.txt");


    return 0;
}



