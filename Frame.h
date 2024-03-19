#ifndef ONE_WAY_COMMUNICATOR_FRAME_H
#define ONE_WAY_COMMUNICATOR_FRAME_H

#include <cstring>

class Frame { // NOLINT(cppcoreguidelines-pro-type-member-init)
protected:
    unsigned char *mp_data; // contains all data for transferring
    unsigned int m_data_size; // size with service data. example: frame_number, single_frame_len
    unsigned long long file_id;


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

    static void insertULongLong(unsigned long long ull_number, unsigned char *p_dest) {
        for (int i = 7; i >= 0; --i) {
            p_dest[i] = char(ull_number);
            ull_number >>= 8;
        }
    }

    static unsigned long long getULongLong(const unsigned char *p_source) {
        unsigned long long out = 0;

        for (int i = 0; i < 7; ++i) {
            out = out | p_source[i];
            out <<= 8;

        }
        return out | p_source[7];
    }


public:
    //! on adding new member change it
    static const int FRAME_ADDITIONAL_MEMBER_SIZE = sizeof(file_id) + sizeof(m_data_size);


    Frame(unsigned char *Data, unsigned int DataSize, unsigned long long int fileId) :
            mp_data(Data), m_data_size(DataSize), file_id(fileId) {}

    static bool isControlFrame(unsigned char *data) {
        return getUInt(data + 0) == 0;
    }


    [[nodiscard]] unsigned char *getData() const {
        return mp_data;
    }


    [[nodiscard]] unsigned int getDataSize() const {
        return m_data_size;
    }

    [[nodiscard]] unsigned long long int getFileId() const {
        return file_id;
    }

    ~Frame() {
        delete[] this->mp_data;
    }
};

#endif