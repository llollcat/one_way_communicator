#ifndef ONE_WAY_COMMUNICATOR_CONTROLFRAME_H
#define ONE_WAY_COMMUNICATOR_CONTROLFRAME_H

#include "Frame.h"

class ControlFrame final : public Frame {

private:
    unsigned int m_common_frame_amount;


public:
    //! on adding new member change it
    static const int ADDITIONAL_MEMBER_SIZE =
            FRAME_ADDITIONAL_MEMBER_SIZE + sizeof(unsigned int) + sizeof(m_common_frame_amount);


    // control Frame
    ControlFrame(const unsigned int t_common_frame_amount, unsigned long long t_file_id, const char *p_filename,
                 const unsigned int t_filename_size) :
            Frame(new unsigned char[ADDITIONAL_MEMBER_SIZE + t_filename_size],
                  ADDITIONAL_MEMBER_SIZE + t_filename_size, t_file_id) {

        this->m_common_frame_amount = t_common_frame_amount;
        insertUInt(0, this->mp_data + 0);
        insertUInt(this->mp_data_size, this->mp_data + 4);
        insertUInt(this->m_common_frame_amount, this->mp_data + 8);
        insertULongLong(t_file_id, this->mp_data + 12);
        memcpy(this->mp_data + ADDITIONAL_MEMBER_SIZE, p_filename, t_filename_size);

    }

    // from received to class
    explicit ControlFrame(unsigned char *data) :
            Frame(new unsigned char[getUInt(data + 4)],
                  getUInt(data + 4), getULongLong(data + 12)) {

        this->m_common_frame_amount = getUInt(data + 8);

        memcpy(this->mp_data, data, this->mp_data_size);
    }


    [[nodiscard]] unsigned int getCommonFrameAmount() const {
        return this->m_common_frame_amount;
    }
};

#endif