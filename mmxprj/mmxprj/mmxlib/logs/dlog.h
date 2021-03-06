#ifndef _MMX_LOG_DLOG_H
#define _MMX_LOG_DLOG_H

#include "log.h"

//#define DLOG_POINTER(obj)  (((long)(obj) / 8) % 0x10000)
#define __FILENAME__ (__FILE__ + SOURCE_PATH_SIZE)
#define DLOG_POINTER(obj)  (((long)(obj)) & 0xFFFFFF)
#define DLOG_CLASS_BEGIN(cname, msg) "%.4d ## [%.6x] " cname "::" msg, __LINE__, DLOG_POINTER(this)

#define DLOG_IP(ip) ((ip) >> 24) & 0xFF, ((ip) >> 16) & 0xFF, ((ip) >> 8) & 0xFF, (ip) & 0xFF
#define DLOG_IP_FMT "%d.%d.%d.%d"

#define DLOG_LEVEL 1

#if DLOG_LEVEL <= 0
    #ifdef DEBUG
        #define    DLOGT   mmx::logs::logT
    #endif
#else
    #define    DLOGT
#endif

#if DLOG_LEVEL <= 1
    #define    DLOGD   mmx::logs::logD
#else
    #define    DLOGD
#endif

#if DLOG_LEVEL <= 2
    #define    DLOGI   mmx::logs::logI
#else
    #define    DLOGI
#endif


#if DLOG_LEVEL <= 3
    #define    DLOGW   mmx::logs::logW
#else
    #define    DLOGW
#endif

#if DLOG_LEVEL <= 4
    #define    DLOGE   mmx::logs::logE
#else
    #define    DLOGE
#endif

#if DLOG_LEVEL <= 5
    #define    DLOGC   mmx::logs::logC
#else
    #define    DLOGC
#endif

#endif
