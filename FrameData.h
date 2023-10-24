#ifndef ONE_WAY_COMMUNICATOR_FRAMEDATA_H
#define ONE_WAY_COMMUNICATOR_FRAMEDATA_H



class FrameData {
private:
    const int COMMON_FRAME_MEMBER_AMOUNT = 4;
    const int CONTROL_FRAME_MEMBER_AMOUNT = 4 * 3;


    unsigned char *mp_data;
    int m_size_with_additional_data; // size with service data. ex frame_number, single_frame_len
    bool m_is_control;
    unsigned int m_frame_number;
    unsigned int m_all_frame_amount;
    unsigned int m_single_frame_len; // currently unused

    static void insertUInt(unsigned int in, unsigned int num_start, unsigned char *p_dest) {
        for (int i = 3; i >= 0; --i) {
            p_dest[num_start + i] = char(in);
            in >>= 8;
        }
    }

    static unsigned int getUInt(unsigned int num_start, const unsigned char *p_source) {
        unsigned int out = 0;

        for (int i = 0; i < 3; ++i) {
            out = out | p_source[num_start + i];
            out <<= 8;

        }
        return out | p_source[num_start + 3];

    }


public:
    // common Frame
    // p_data - without service data , t_data_size without service data
    FrameData(const unsigned int t_frame_number, const char *p_data, const int t_data_size) {
        m_is_control = false;
        this->m_all_frame_amount = -1;
        this->m_single_frame_len = -1;
        this->m_frame_number = t_frame_number;
        this->m_size_with_additional_data = t_data_size + COMMON_FRAME_MEMBER_AMOUNT;
        this->mp_data = new unsigned char[m_size_with_additional_data];
        insertUInt(t_frame_number, 0, this->mp_data);
        memcpy(this->mp_data + 4, p_data, t_data_size);

    }

    // control Frame
    // t_single_frame_len - currently unused
    FrameData(const unsigned int t_frame_amount, const unsigned int t_single_frame_len) {
        m_is_control = true;
        this->m_frame_number = 0;
        this->m_all_frame_amount = t_frame_amount;
        this->m_size_with_additional_data = CONTROL_FRAME_MEMBER_AMOUNT;
        this->m_single_frame_len = t_single_frame_len;
        this->mp_data = new unsigned char[m_size_with_additional_data];

        insertUInt(0, 0, this->mp_data);
        insertUInt(t_frame_amount, 4, this->mp_data);
        insertUInt(t_single_frame_len, 8, this->mp_data);


    }

    // from received to class
    FrameData(unsigned char *data, int t_size) {
        this->m_frame_number = getUInt(0, data);
        this->m_is_control = this->m_frame_number == 0;

        if (this->m_is_control) {
            this->m_size_with_additional_data = t_size;
            this->m_all_frame_amount = getUInt(4, data);
            this->m_single_frame_len = getUInt(8, data);
            this->mp_data = new unsigned char[t_size];
            memcpy(this->mp_data, data, t_size);

            return;
        }

        this->m_size_with_additional_data = t_size;
        this->mp_data = new unsigned char[t_size];
        memcpy(this->mp_data, data, t_size);


    }


    unsigned char *getUCharData() {
        return this->mp_data;
    }

    [[nodiscard]] int getSize() const {
        return this->m_size_with_additional_data;
    }

    [[nodiscard]] unsigned int getFrameAmount() const {
        return this->m_all_frame_amount;
    }

    [[nodiscard]] unsigned int getFrameNumber() const {
        return this->m_frame_number;
    }


    [[nodiscard]] bool isControl() const {
        return this->m_is_control;
    }


    ~FrameData() {
        delete[] this->mp_data;

    }
};


#endif
