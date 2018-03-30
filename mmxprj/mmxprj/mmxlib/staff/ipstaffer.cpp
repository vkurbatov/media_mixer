#include "ipstaffer.h"
#include <cstring>

#include "logs/log.h"

#include <errno.h>
#include <netdb.h>

#include <iostream>

namespace mmx
{
    namespace staff
    {
        IPStaffer::IPStaffer(IStaffer* next) :
            next_(next), pos_(0)
        {

        }

        IPStaffer::~IPStaffer()
        {

        }

        int IPStaffer::Dispatch(const char* stream, int size, PPROTOCOL_INFO pinfo)
        {

            int rc = -EINVAL;

            const char* pointer = stream;

            if (stream != nullptr && size > 0)
            {

                // TODO: необходимо сделать склеивание фрагментированных пакетов

                // пока считаем что пакет пришел полностью и только один              

                headers::IP4HEADER& ip4_header = *(headers::PIP4HEADER)stream;
                unsigned short len = (ip4_header.length << 8) | (ip4_header.length >> 8);


                if (len == size)
                {
                    PROTOCOL_INFO pi = { pinfo, stream, IPPROTO_IP };

                    if (next_ != nullptr)
                    {
                        rc = next_->Dispatch(stream + sizeof(ip4_header), len - sizeof(ip4_header), &pi);
                    }
                    else
                    {
                        rc = size;
                    }

                }
                else
                {
                    //logs::logC("[IPStaffer] len =%d, size = %d", len, size);

                }

                /* доделать позже...

                // данные все получены, формируем новый пакет

                if (pos_ == 0)
                {
                    // заголовок можем считать полностью

                    headers::IP4HEADER& ip4_header = *(headers::PIP4HEADER)stream;

                    if (size >= sizeof(ip4_header_))
                    {

                        // если пакет и pyload пришли полностью, то нету смысла копировать заголовок
                        // в этом случае организуем транзитную передачу

                        // нету смысла копировать, можно просто транслировать
                        //std::strcpy(&ip4_header_, sizeof(ip4_header_));

                    }
                }
                else
                {

                }

                */
            }
            return rc;
        }

        void IPStaffer::Reset()
        {
            pos_ = 0;
        }
    }
}
