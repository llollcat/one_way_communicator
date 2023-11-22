#include <fstream>
#include "iostream"
#include <string>
#include <filesystem>
#include <csignal>

#include "Sender.h"
#include "ArgumentsGetter.h"
#include "CommonFrame.h"
#include "ControlFrame.h"

#define D_PORT "8888"
#define D_FILE_FRAME_SIZE "512"


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

    if (file_frame_size<=0){
        std::cout << "-file-frame-size must be greater than " << 33 + CommonFrame::COMMON_FRAME_ADDITIONAL_MEMBER_SIZE;
        return 1;
    }


    bool is_working = true;
    shutdown_handler = [&is_working](int signal) -> void {
        is_working = false;
    };

    std::signal(SIGINT, signal_handler);
    //todo move it to class
    auto sendFile = [&file_frame_size, &server, &port, &is_working]
            (unsigned long long t_file_id, const char *filename_name, const char *filename_path) {

        std::ifstream in_file(filename_path, std::ios::binary | std::ios::ate);
        if (!in_file.is_open()) {
            std::cerr << "failed to open ";
            return -1;
        }

        long long file_size = in_file.tellg();
        in_file.seekg(0, std::ios::beg);

        long long common_frame_number = (file_size / file_frame_size) + bool(file_size % file_frame_size);
        Sender sender = Sender(server, port);
        auto controlFrameData = ControlFrame(common_frame_number, t_file_id, filename_name, strlen(filename_name));


        sender.sendData(controlFrameData.getData(), controlFrameData.getDataSize());


        char *message = new char[file_frame_size];
        for (int frame_count = 1; in_file.peek() != EOF && is_working; ++frame_count) {
            in_file.read(message, file_frame_size);
            CommonFrame *commonFrame;
            if (frame_count == common_frame_number && file_size % file_frame_size != 0) {
                commonFrame = new CommonFrame(frame_count, t_file_id, message, (file_size % file_frame_size));
            } else {
                commonFrame = new CommonFrame(frame_count, t_file_id, message, file_frame_size);
            }

            sender.sendData(commonFrame->getData(), commonFrame->getDataSize());
            delete commonFrame;
        }
        std::cout << "Done." << std::endl;
        delete[] message;

        return 0;

    };


    if (input.isCmdOptionExists("-file")) {
        const char *filename = input.getCmdOption("-file").c_str();
        return sendFile(0ull, filename, filename);
    } else {
        std::string path = input.getCmdOptionSafely("-dir");
        unsigned long long file_id = 1;
        for (const auto &entry: std::filesystem::directory_iterator(path)) {
            sendFile(file_id, reinterpret_cast<const char *>(entry.path().filename().string().c_str()),
                     reinterpret_cast<const char *>(entry.path().string().c_str()));
        }


        return 0;


    }

}

