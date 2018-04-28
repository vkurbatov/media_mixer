#ifndef _MMX_LOG_DLOG_H
#define _MMX_LOG_DLOG_H

#include "log.h"

//#define DLOG_POINTER(obj)  (((long)(obj) / 8) % 0x10000)
#define DLOG_POINTER(obj)  (((long)(obj)) & 0xFFFFFF)
#define DLOG_CLASS_BEGIN(cname, msg) cname "{%.6x}::" msg, DLOG_POINTER(this)

#define DLOG_LEVEL 0

#if DLOG_LEVEL <= 0
    #define    DLOGT   mmx::logs::logT
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
