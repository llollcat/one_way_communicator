#ifndef ONE_WAY_COMMUNICATOR_SENDER_H
#define ONE_WAY_COMMUNICATOR_SENDER_H

#ifdef _WIN32
#include "win/SenderWin.h"
#else
#include "linux/SenderLinux.h"
#endif
#endif