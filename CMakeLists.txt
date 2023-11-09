cmake_minimum_required(VERSION 3.22)
project(one_way_communicator)

set(CMAKE_CXX_STANDARD 17)

add_executable(one-way-communicator-send send.cpp Sender.h Frame.h Receiver.h ControlFrame.h  CommonFrame.h win/ReceiverWin.h AbstractBaseReceiver.h linux/ReceiverLinux.h linux/SenderLinux.h)
add_executable(one-way-communicator-receive Sender.h Frame.h Receiver.h receive.cpp ArgumentsGetter.h CommonFrame.h ControlFrame.h win/ReceiverWin.h AbstractBaseReceiver.h linux/ReceiverLinux.h AbstractBaseSender.h linux/SenderLinux.h win/SenderWin.h)
if (WIN32)
    target_link_libraries(one-way-communicator-send wsock32 ws2_32)
    target_link_libraries(one-way-communicator-receive wsock32 ws2_32)

endif ()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()
if (NOT CMAKE_CXX_COMPILER_ID  STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()
