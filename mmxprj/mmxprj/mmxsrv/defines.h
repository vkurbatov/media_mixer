#ifndef _MMXSRV_DEFINES_H
#define _MMXSRV_DEFINES_H

#include "mmxlib/logs/log.h"


#define SERVICE_GROUP "Media module"
#define SERVICE_NAME "Communication dispatcher"
#define SERVICE_MAJOR_VERSION 1
#define SERVICE_MINOR_VERSION 0
#define SERVICE_BUILD_VERSION BUILD_NUMBER

#define BUILD_TIME  __TIMESTAMP__

#ifdef DEBUG
#define SERVICE_STATUS "debug"
#else
#define SERVICE_STATUS "release"
#endif

#define LOG_NAME_PATTERN "/var/log/mmx/server-%d.log"

#define DEFAULT_LEVEL_LOG mmx::logs::L_DEBUG

#define DEFAULT_BASE_PORT       5400
#define DEFAULT_INTERVAL        2000

void sig_set();

int parse_args(int argc, char* argv[], void *cfg1, void *cfg2, mmx::logs::log_level_t &log_level);


#endif
