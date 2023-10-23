//
// Created by tiramisu on 19.10.2023.
//

#ifndef UNTITLED_FRAMEDATA_H
#define UNTITLED_FRAMEDATA_H

#include <bits/stdc++.h>

unsigned int single_frame_len = 512;

class FrameData {
protected:
    unsigned char *data;
    int size_with_additional_data; // размер вместе со всеми служебными
    bool is_control;
    unsigned int frame_number;
    unsigned int frame_amount;
    unsigned int __single_frame_len = 512;

    static void insert_uint(unsigned int in, unsigned int num_start, unsigned char *dest) {
        for (int i = 3; i >= 0; --i) {
            dest[num_start + i] = char(in);
            in >>= 8;
        }
    }

    unsigned int get_uint(unsigned int num_start, unsigned char *source) {
        unsigned int out = 0;

        for (int i = 0; i < 3; ++i) {
            out = out | source[num_start + i];
            out <<= 8;

        }
        out = out | source[num_start + 3];


        return out;
    }


public:
    // common
    // data - без служебных, размер без служебных
    // todo обавить проверку на превышение максимальной длиннны фрейма
    FrameData(unsigned int frame_number, const char *data, int data_size) {
        is_control = false;
        this->frame_number = frame_number;
        this->size_with_additional_data = data_size + 4;
        this->data = new unsigned char[data_size];
        insert_uint(frame_number, 0, this->data);
        memcpy(this->data + 4, data, data_size);

    }

    // control
    // single_frame_len - максимальный размер фрейма  data - без служебных, размер без служебных
    FrameData(unsigned int frame_amount, unsigned int __single_frame_len) {
        is_control = true;
        this->frame_number = 0;
        this->frame_amount = frame_amount;
        this->size_with_additional_data = 4 * 3;
        this->data = new unsigned char[4 * 3];
        insert_uint(0, 0, this->data);
        insert_uint(frame_amount, 4, this->data);
        insert_uint(single_frame_len, 8, this->data);


    }

    // from received to class
    FrameData(unsigned char *data, int size) {
        this->frame_number = get_uint(0, data);
        this->is_control = this->frame_number == 0;


        if (this->is_control) {
            this->size_with_additional_data = size;
            this->frame_amount = get_uint(4, data);
            this->__single_frame_len = get_uint(8, data);
            this->data = new unsigned char[size];
            memcpy(this->data, data, size);
            for (int i = 0; i < 30; ++i) {
                std::cout << std::hex << int(data[i]) << ' ';
            }


            return;
        }
        this->size_with_additional_data = size;
        this->data = new unsigned char[size];
        memcpy(this->data, data, size);


    }


    unsigned char *getuCharData() {

        return this->data;
    }

    [[nodiscard]] int getSize() const {
        return this->size_with_additional_data;
    }

    [[nodiscard]] unsigned int getFrameAmount() const {
        return this->frame_amount;
    }

    [[nodiscard]] unsigned int getFrameNumber() const {
        return this->frame_number;
    }


    [[nodiscard]] bool isControl() const {
        return this->is_control;
    }

    [[nodiscard]] unsigned int getSingleFrameLen() const {
        return single_frame_len;
    }

    ~FrameData() {
        // delete[] this->data;

    }
};


#endif //UNTITLED_FRAMEDATA_H
