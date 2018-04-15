#ifndef _MMX_NET_SELECT_EX_H
#define _MMX_NET_SELECT_EX_H

#include "select.h"

namespace mmx
{
    namespace net
    {
        class SelectExtension : public Select
        {

        public:
            SelectExtension();
            ~SelectExtension();

            bool IsRead(int fd) const;
            bool IsWrite(int fd) const;
            bool IsExcept(int fd) const;

            bool SetWrite(int fd);
            bool SetRead(int fd);
            bool SetExcept(int fd);

            bool ClrWrite(int fd);
            bool ClrRead(int fd);
            bool ClrExcept(int fd);
        };
    }
}

#endif
