// receiver
#include <bits/stdc++.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Receiver.h"
using namespace std;

const char FILENAME[] = "123.txt";

const int BUFLEN = 548;
const int PORT = 8888;

int main()
{
    Receiver receiver = Receiver(BUFLEN, PORT);
    receiver.getFile(FILENAME);

}