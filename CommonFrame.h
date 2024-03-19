#ifndef ONE_WAY_COMMUNICATOR_COMMONFRAME_H
#define ONE_WAY_COMMUNICATOR_COMMONFRAME_H

#include "Frame.h"

class CommonFrame final : public Frame {

private:
    unsigned int m_frame_number;


public:
    //todo сделать автоподсчёт
    //! on adding new member change it
    static const int COMMON_FRAME_ADDITIONAL_MEMBER_SIZE = FRAME_ADDITIONAL_MEMBER_SIZE + sizeof(m_frame_number);


    // common Frame
    // p_data - without service data , t_data_size without service data
    CommonFrame(const unsigned int t_frame_number, unsigned long long t_file_id, const char *p_data,
                const int t_data_size) :
            Frame(new unsigned char[COMMON_FRAME_ADDITIONAL_MEMBER_SIZE + t_data_size],
                  COMMON_FRAME_ADDITIONAL_MEMBER_SIZE + t_data_size, t_file_id) {

        this->m_frame_number = t_frame_number;
        insertUInt(t_frame_number, this->mp_data + 0);
        insertUInt(this->m_data_size, this->mp_data + 4);
        insertULongLong(t_file_id, this->mp_data + 8);
        memcpy(this->mp_data + COMMON_FRAME_ADDITIONAL_MEMBER_SIZE, p_data, t_data_size);
    }


    // from received to class
    explicit CommonFrame(unsigned char *data) :
            Frame(new unsigned char[getUInt(data + 4)],
                  getUInt(data + 4), getULongLong(data + 8)) {

        this->m_frame_number = getUInt(data + 0);
        memcpy(this->mp_data, data, this->m_data_size);
    }

    [[nodiscard]] unsigned int getFrameNumber() const {
        return m_frame_number;
    }
};

#endif