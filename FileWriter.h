#ifndef ONE_WAY_COMMUNICATOR_FILEWRITER_H
#define ONE_WAY_COMMUNICATOR_FILEWRITER_H

#include <string>
#include <map>
#include <fstream>
#include <iostream>

class FileWriter {

private:

    int m_frame_size;
    std::map<int, std::ofstream> m_opened_files;

    void openFileWriting(int id){
        m_opened_files.emplace(id, std::ofstream(std::to_string(id),  std::ios::out | std::ios::binary));
    }



public:
    explicit FileWriter(int frame_size): m_frame_size(frame_size){ }
    void addToFile(int file_id, int frame_number, const unsigned char *data, int size ){
        if (m_opened_files.find(file_id) == m_opened_files.end()){
            openFileWriting(file_id);
        }
        m_opened_files[file_id].seekp((frame_number-1)*m_frame_size, std::ios_base::beg).write(
                reinterpret_cast<const char *>(data), size);

    }

    void closeFile(int file_id, std::string name){
        m_opened_files[file_id].close();
        m_opened_files.erase(file_id);
        std::rename(std::to_string(file_id).c_str(), name.c_str());
    }

};


#endif
