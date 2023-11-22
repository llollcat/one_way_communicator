#include <fstream>
#include "iostream"
#include <string>
#include <filesystem>
#include <csignal>

#include "Sender.h"
#include "ArgumentsGetter.h"


#define D_PORT "8888"
#define D_FILE_FRAME_SIZE "512"

// for ctrl+c handler
namespace {

    std::function<void(int)> shutdown_handler;

    void signal_handler(int signal) { shutdown_handler(signal); }
}

int main(int argc, char *argv[]) {
    ArgumentsGetter input(argc, argv);

    if (input.isCmdOptionExists("-h")) {

        std::cout << "Program is send selected file to selected server by UDP.\n"
                     "Required arguments: -server, -file or -dir, \n"
                     "Optional arguments: -port, -file-frame-size\n"
                     "Example: -server 127.0.0.1 -port 8888 -file s.txt -file-frame-size 512\n"
                     "Example: -server 127.0.0.1 -port 8888 -dir /some/dir/with/files -file-frame-size 512"
                  << std::endl;
        return 0;
    }

    auto server = input.getCmdOptionSafely("-server").c_str();
    int port = stoi(input.getCmdOptionIfGiven("-port", D_PORT));
    int file_frame_size = stoi(input.getCmdOptionIfGiven("-file-frame-size", D_FILE_FRAME_SIZE))
                          - 32 - CommonFrame::COMMON_FRAME_ADDITIONAL_MEMBER_SIZE;

    if (file_frame_size <= 0) {
        std::cout << "-file-frame-size must be greater than " << 33 + CommonFrame::COMMON_FRAME_ADDITIONAL_MEMBER_SIZE;
        return 1;
    }


    Sender sender = Sender(server, port, file_frame_size);

    shutdown_handler = [&sender](int signal) -> void {
        sender.stopReceivingSignal();
    };

    std::signal(SIGINT, signal_handler);


    if (input.isCmdOptionExists("-file")) {
        const char *filename = input.getCmdOption("-file").c_str();
        sender.sendFile(0ull, filename, filename);

    } else {
        std::string path = input.getCmdOptionSafely("-dir");
        unsigned long long file_id = 1;
        for (const auto &entry: std::filesystem::directory_iterator(path)) {
            sender.sendFile(file_id, reinterpret_cast<const char *>(entry.path().filename().string().c_str()),
                            reinterpret_cast<const char *>(entry.path().string().c_str()));
        }


    }
    return 0;
}

