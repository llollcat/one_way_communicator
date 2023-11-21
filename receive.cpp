#include "Receiver.h"
#include "ArgumentsGetter.h"
#include <csignal>

#define D_PORT "8888"
#define D_FILE_FRAME_SIZE "512"
//todo check filename parametr is working
namespace {
    std::function<void(int)> shutdown_handler;

    void signal_handler(int signal) { shutdown_handler(signal); }
}

int main(int argc, char *argv[]) {
    ArgumentsGetter input(argc, argv);

    if (input.isCmdOptionExists("-h")) {

        std::cout << "Program is receive file by UDP.\n"
                     "Optional arguments: -port, -file-frame-size, -file\n"
                     "Example: -port 8888 -file-frame-size 512" << std::endl;
        return 0;
    }


    int port = stoi(input.getCmdOptionIfGiven("-port", D_PORT));
    int file_frame_size = stoi(input.getCmdOptionIfGiven("-file-frame-size",
                                                         D_FILE_FRAME_SIZE));  //  real size of frame is m_file_frame_size+4


    Receiver receiver = Receiver(file_frame_size, port);

    shutdown_handler = [&receiver](int signal) -> void {
        receiver.stopReceivingSignal();
    };

    std::signal(SIGINT, signal_handler);


    receiver.getFile();


    return 0;
}



