#include "Receiver.h"

const char FILENAME[] = "123.png";
const int FILE_BYTE_SIZE = 548;
const int PORT = 8888;

int main()
{
    Receiver receiver = Receiver(FILE_BYTE_SIZE, PORT);
    receiver.getFile(FILENAME);

}