#ifndef ONE_WAY_COMMUNICATOR_FRAME_H
#define ONE_WAY_COMMUNICATOR_FRAME_H

#include <cstring>
class Frame {
protected:
    unsigned char *mp_data; // contains all data for transferring
    int mp_data_size; // size with service data. example: frame_number, single_frame_len

    // on adding new member change it
    const int FRAME_ADDITIONAL_MEMBER_SIZE= sizeof(mp_data_size);


    static void insertUInt(unsigned int ui_number, unsigned char *p_dest) {
        for (int i = 3; i >= 0; --i) {
            p_dest[i] = char(ui_number);
            ui_number >>= 8;
        }
    }

    static unsigned int getUInt(const unsigned char *p_source) {
        unsigned int out = 0;

        for (int i = 0; i < 3; ++i) {
            out = out | p_source[i];
            out <<= 8;

        }
        return out | p_source[3];

    }
    Frame(){}


public:
    static bool isControlFrame(unsigned char *data){
        return getUInt(data + 0) == 0;
    }


    [[nodiscard]] unsigned char *getData() const {
        return mp_data;
    }


    [[nodiscard]] int getDataSize() const {
        return mp_data_size;
    }

    ~Frame() {
        delete[] this->mp_data;

    }

};


#endif
