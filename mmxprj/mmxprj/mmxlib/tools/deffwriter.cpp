#include "deffwriter.h"

#include <cstring> // memcpy

#include "logs/dlog.h"

#define LOG_BEGIN(msg) DLOG_CLASS_BEGIN("DeferredWriter", msg)

//#include <errno.h>

namespace mmx
{
    namespace tools
    {
        DeferredWriter::DeferredWriter(ipc::IIO& io, int limit) :
            io_(io),
            limit_(limit),
            last_result_(0),
            lost_(0)
        {

        }

        int DeferredWriter::Write(const void* data, int size, int flags)
        {

            // для начала проверим выходную очередь
            // если она пустая, то пробуем записать сперва то что в очереди

            // флаг управления порционными посылками
            auto addr = &io_;

            addr = addr;

            bool f_write = !IsEmpty() && io_.IsCanWrite();

            // признак успешности передачи отложенных данных

            bool f_deff_complete = !f_write && io_.IsCanWrite();

            // общее количество реально отправленных байт

            last_result_ = 0;

            // код возврата операции ввода-вывода

            int w_ret = 0;


            // Этап 1. Пробуем передать непереданные ранее данные из очереди (если они есть)

            while (f_write)
            {

                // апрори считаем что следующей порции записи не будет

                f_write = false;


                // если сюда попали, то очередь уже не пуста

                auto& p = q_write_.front();

                // пробуем передать данные в канал

                w_ret = io_.Write(p.Data(), p.Size(), flags);

                // ошибка, очередь не изменяем

                if (w_ret < 0)
                {
                    DLOGD(LOG_BEGIN("Write(%x, %d, %x): deferred write error %d"),DLOG_POINTER(data), size, flags, w_ret);
                    // следующей транзакции не будет
                }
                else
                {
                    // идеальный случай, передали всю порцию данных

                    if (w_ret == p.Size())
                    {
                        // просто перемещаем очередь и пробуем передать следующую порцию

                        pop();

                        // если в очереди есть данные, то пытаемся их передать в следующей транзакции

                        f_write = !q_write_.empty();

                        f_deff_complete = !f_write;
                    }
                    else
                    {
                        DLOGD(LOG_BEGIN("Write(%x, %d, %x): deferred write only part %d "),DLOG_POINTER(data), size, flags, w_ret);

                        // данные передали частично

                        // просто смещаем позицию недопереданного пакета на w_ret байт

                        p.Seek(p.Position() + w_ret);
                    }

                    // сформируем общай счетчик переданных байт

                    last_result_ += w_ret;
                }

            }//while

            // Этап 2. Пробуем передать транзитные данные (если они заданы)

            if (data != nullptr && size > 0)
            {

                // тут останется количество переданных в канал транзитных данных

                int drop = 0;

                if (f_deff_complete)
                {

                    // сюда попадаем только если отложенных данных не было,

                    // или передали их полностью синхронно

                    // передаем транзитные данные в канал

                    w_ret = io_.Write(data, size, flags);

                    if (w_ret > 0)
                    {

                        last_result_ += w_ret;
                        drop = w_ret;
                    }

                }

                // ситуация когда нужно положить непереданые остатки транзитных данных
                // в отложенную очередь на передачу

                if (drop < size)
                {
                    DLOGD(LOG_BEGIN("Write(%x, %d, %x): push deffered data %d bytes"),DLOG_POINTER(data), size, flags, size - drop);
                    pushData((const char*)data + drop, size - drop);

                }

            }          

            return w_ret >= 0 ? last_result_ : w_ret;
        }

        int DeferredWriter::Read(void* data, int size, int flags)
        {
            return io_.Read(data, size, flags);
        }

        bool DeferredWriter::IsCanWrite() const
        {
            return true;
        }

        bool DeferredWriter::IsCanRead() const
        {
            return io_.IsCanRead();
        }

        bool DeferredWriter::IsEmpty() const
        {
            return q_write_.empty();
        }

        int DeferredWriter::Count() const
        {
            return q_write_.size();
        }

        int DeferredWriter::LastResult() const
        {
            return last_result_;
        }

        int DeferredWriter::Lost() const
        {
            return lost_;
        }

        int DeferredWriter::Drop(int count)
        {
            int rc = 0;

            while (q_write_.size() > 0 &&
                   (count == 0 || rc < count)
                   )
            {
                pop();
                rc++;
            }

            return rc;
        }

        void DeferredWriter::Reset()
        {
            while (!q_write_.empty())
            {
                q_write_.pop();
            }

            while (!q_free_.empty())
            {
                q_free_.pop();
            }

            lost_ = 0;
            last_result_ = 0;
        }

        data::Packet* DeferredWriter::getPacket(int size)
        {
            data::Packet* rc = nullptr;

            if (limit_>0 && q_write_.size() > limit_)
            {
                // превысили лимит
                // потеря данных!!
                auto& p = q_write_.front();
                lost_ += p.Size();

                q_write_.push(std::move(p));
                q_write_.pop();

            }
            else
            {

                if (!q_free_.empty())
                {
                    // пытаемся получить пакет из очереди свободных

                    q_write_.push(std::move(q_free_.front()));
                    q_free_.pop();
                }
                else
                {
                    // будет создан пакет с запрашиваемым размеров

                    q_write_.push(data::Packet(size));
                }
            }

            //

            rc = &q_write_.back();
            rc->Reset();
            rc ->Resize(size);

            return rc;
        }

        void DeferredWriter::pushData(const char* data, int size)
        {
            auto p = getPacket(size);
            std::memcpy(p->Data(), data, size);

        }

        void DeferredWriter::pop()
        {

            // не удаляем - переносим в список свободных

            if (!q_write_.empty())
            {

                q_free_.push(std::move(q_write_.front()));
                q_write_.pop();

            }
        }
    }
}
