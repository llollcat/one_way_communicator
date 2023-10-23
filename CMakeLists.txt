cmake_minimum_required(VERSION 3.25)
project(one_way_communicator)

set(CMAKE_CXX_STANDARD 17)

add_executable(one-way-communicator-send send.cpp Sender.h FrameData.h Receiver.h )
add_executable(one-way-communicator-receive Sender.h FrameData.h Receiver.h receive.cpp)
if(WIN32)
    target_link_libraries(one-way-communicator-send wsock32 ws2_32)
    target_link_libraries(one-way-communicator-receive wsock32 ws2_32)
endif()