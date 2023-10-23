// sender
#include <bits/stdc++.h>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

#include "FrameData.h"

const char FILENAME[] = "rufus-4.2p.exe";
const char SERVER[] = "127.0.0.1";  // or "localhost" - ip address of UDP server
const int BUFLEN = 512;  // max length of answer

const unsigned int PORT = 8888;  // the port on which to listen for incoming data

#include "Sender.h"

int main() {

    Sender sender = Sender(BUFLEN, SERVER, PORT);
    sender.initConnection();
    ifstream in_file(FILENAME, ios::binary);;
    if (!in_file.is_open()) {
        std::cerr << "failed to open ";
        return -5;
    }
    in_file.seekg(0, ios::end);

    FrameData control_frameData = FrameData(in_file.tellg() / BUFLEN + 2, BUFLEN);
    in_file.seekg(0, ios::beg);


    sender.sendFrameData(control_frameData);


    char message[BUFLEN];
    for (int frame_num = 1; in_file.peek() != EOF; ++frame_num) {
        in_file.read(message, BUFLEN);
        FrameData frameData = FrameData(frame_num, message, BUFLEN);
        sender.sendFrameData(frameData);

    }


}