#ifndef ONE_WAY_COMMUNICATOR_COMMONFRAME_H
#define ONE_WAY_COMMUNICATOR_COMMONFRAME_H

#include "Frame.h"
class CommonFrame final: public Frame  {

private:
    unsigned int m_frame_number;


    //! on adding new member change it
    const int COMMON_FRAME_ADDITIONAL_MEMBER_SIZE= FRAME_ADDITIONAL_MEMBER_SIZE + sizeof (m_frame_number);


public:
    // common Frame
    // p_data - without service data , t_data_size without service data
    CommonFrame(const unsigned int t_frame_number, long long t_file_id, const char *p_data, const int t_data_size) {
        this->m_frame_number = t_frame_number;
        this->mp_data_size = COMMON_FRAME_ADDITIONAL_MEMBER_SIZE + t_data_size;
        this->mp_data = new unsigned char[this->mp_data_size];
        insertUInt(t_frame_number, this->mp_data+0);
        insertUInt(this->mp_data_size,this->mp_data+4);

        memcpy(this->mp_data + COMMON_FRAME_ADDITIONAL_MEMBER_SIZE, p_data, t_data_size);

    }


    // from received to class
    CommonFrame(unsigned char *data) {
        this->m_frame_number = getUInt(data + 0);
        this->mp_data_size =  getUInt(data + 4);

        this->mp_data = new unsigned char[this->mp_data_size];
        memcpy(this->mp_data, data, this->mp_data_size);

    }

    [[nodiscard]] unsigned int getFrameNumber() const {
        return m_frame_number;
    }


    [[nodiscard]] int getAdditionalMemberSize() const {
        return COMMON_FRAME_ADDITIONAL_MEMBER_SIZE;
    }


};


#endif //ONE_WAY_COMMUNICATOR_COMMONFRAME_H
