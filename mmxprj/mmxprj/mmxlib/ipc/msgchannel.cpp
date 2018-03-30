#include "msgchannel.h"

#include <cstring>      // strcpy
#include <error.h>

namespace mmx
{
    namespace ipc
    {
        MsgChannel::MsgChannel(IChannel& channel) :
            channel_(channel), buffer_()
        {
            buffer_.reserve(DEFAULT_BUFFER_CAP);
        }

        int MsgChannel::SendMsg(const headers::PMSG_HEADER pmsg, const char* data, int flags)
        {

            int rc = -EINVAL;

            // есть заголовок, и корректно переданы данные с размером

            if (pmsg != nullptr && (pmsg->length == 0 || data != nullptr))
            {

                rc = channel_.Write((const char*)pmsg, sizeof(headers::MSG_HEADER), flags);


                // пока считаем что должны оптравить весь заголовок

                if (rc == sizeof(sizeof(headers::MSG_HEADER)))
                {

                    // заголовок отправлен, попробуем отправить данные

                    if (pmsg->length > 0)
                    {
                        rc = channel_.Write(data, pmsg->length, flags);
                    }
                }

            }

            return rc;
        }

        int MsgChannel::RecvMsg(headers::PMSG_HEADER pmsg, char* data, unsigned short size, int flags)
        {
            int rc = -EINVAL;



            return rc;
        }
    }
}
