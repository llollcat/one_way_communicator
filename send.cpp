#include <bits/stdc++.h>

#include "Sender.h"
#include "FrameData.h"


const char FILENAME[] = "Untitled-1.png";
const char SERVER[] = "127.0.0.1";  // or "localhost" - ip address of UDP server
const int FILE_BYTE_SIZE = 512;  // max length of answer
const unsigned int PORT = 8888;  // the port on which to listen for incoming mp_data


int main() {

    Sender sender = Sender(SERVER, PORT);


    std::ifstream in_file(FILENAME, std::ios::binary);;
    if (!in_file.is_open()) {
        std::cerr << "failed to open ";
        return -5;
    }

    in_file.seekg(0, std::ios::end);
    FrameData control_frameData = FrameData(in_file.tellg() / FILE_BYTE_SIZE + 2, FILE_BYTE_SIZE);
    in_file.seekg(0, std::ios::beg);


    sender.sendFrameData(control_frameData);


    char message[FILE_BYTE_SIZE];
    for (int frame_num = 1; in_file.peek() != EOF; ++frame_num) {
        in_file.read(message, FILE_BYTE_SIZE);
        FrameData frameData = FrameData(frame_num, message, FILE_BYTE_SIZE);
        sender.sendFrameData(frameData);

    }
    return 0;
}