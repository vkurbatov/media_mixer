#ifndef _MMXMUX_DEFINES_H
#define _MMXMUX_DEFINES_H

#include "mmxlib/logs/log.h"

#define SERVICE_GROUP "Media module"
#define SERVICE_NAME "Media stream multiplexor (core)"
#define SERVICE_MAJOR_VERSION 1
#define SERVICE_MINOR_VERSION 0
#define SERVICE_BUILD_VERSION BUILD_NUMBER

#define BUILD_TIME  __TIMESTAMP__

#ifdef DEBUG
#define SERVICE_STATUS "debug"
#else
#define SERVICE_STATUS "release"
#endif

#define LOG_NAME_PATTERN "/var/log/mmx/core-%d.log"

#define DEFAULT_LEVEL_LOG mmx::logs::L_DEBUG

#define DEFAULT_BASE_PORT           5200
#define DEFAULT_INTERVAL            2000
#define DEFAULT_MIXED_GAIN          50
#define DEFAULT_MEDIA_PERIOD        20
#define DEFAULT_MUTE_TIME           200
#define DEFAULT_JITTER_BUFFER_SIZE  60
#define DEFAULT_MEDIA_HEALTH_TIME	0
#define JITTER_BUFFER_MIN           20
#define JITTER_BUFFER_MAX           (5 * 60 * 1000)

void sig_set();

int parse_args(int argc, char* argv[], void *cfg1, void *cfg2, mmx::logs::log_level_t &log_level);

#endif
