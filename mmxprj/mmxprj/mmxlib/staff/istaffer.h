#ifndef _MMX_STAFF_I_STAFFER_H
#define _MMX_STAFF_I_STAFFER_H

#include "packet.h"

namespace mmx
{
    namespace staff
    {


        typedef struct _PROTOCOL_INFO
        {
            _PROTOCOL_INFO*     parent;
            const void*         header;
            int                 proticol_id;
        }PROTOCOL_INFO,*PPROTOCOL_INFO;

        typedef int(*pyload_callback)(const char* pyload, int size, void* context);

        class IStaffer
        {
        public:
            virtual ~IStaffer() {};
            virtual int Dispatch(const char* stream, int size, PPROTOCOL_INFO pinfo = nullptr) = 0;
            //virtual const void* Header() const = 0;
            virtual void Reset() = 0;
        };
    }
}

#endif
