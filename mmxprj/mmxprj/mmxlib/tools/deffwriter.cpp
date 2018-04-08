#include "deffwriter.h"

#include <cstring> // memcpy

//#include <errno.h>

namespace mmx
{
    namespace tools
    {
        DeferredWriter::DeferredWriter(ipc::IIO& io, int limit) :
            io_(io),
            limit_(limit > 0 ? limit : 1)
        {

        }

        int DeferredWriter::Write(const char* data, int size, int flags)
        {

            // для начала проверим выходную очередь
            // если она пустая, то пробуем записать сперва то что в очереди

            // флаг управления порционными посылками

            bool f_write = !IsEmpty();

            // общее количество реально отправленных байт

            int w_total = 0;

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
                    }
                    else
                    {
                        // данные передали частично

                        // просто смещаем позицию непереданного пакета на w_ret байт

                        p.Seek(p.Position() + w_ret);
                    }

                    // сформируем общай счетчик переданных байт

                    w_total += w_ret;
                }

            }//while

            // Этап 2. Пробуем передать транзитные данные (если они заданы)

            if (data != nullptr && size > 0)
            {

                if (w_ret >= 0)
                {

                    // сюда попадаем только если отложенные данные не передавали,

                    // или передали их полностью синхронно

                    // передаем транзитные данные в канал

                    w_ret = io_.Write(data, size, flags);

                    if (w_ret > 0)
                    {
                        w_total += w_ret;
                    }

                }

                // в этом месте в w_ret останется результат первого или второго этапа

                // ситуаия когда положить остатки в отложенную очередь на передачу

                if (w_ret < size)
                {
                    int off = w_ret < 0 ? 0 : w_ret;
                    pushData(data + off, size - off);
                }

            }

            return w_ret >= 0 ? w_total : w_ret;
        }

        int DeferredWriter::Read(char* data, int size, int flags)
        {
            return io_.Read(data, size, flags);
        }

        bool DeferredWriter::IsEmpty() const
        {
            return q_write_.empty();
        }

        int DeferredWriter::Count() const
        {
            return q_write_.size();
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

        staff::Packet* DeferredWriter::getPacket(int size)
        {
            staff::Packet* rc = nullptr;

            if (q_write_.size() > limit_)
            {
                // привысили лимит
                // потеря данных!!

                q_write_.push(std::move(q_write_.front()));
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

                    q_write_.push(staff::Packet(size));
                }
            }

            //

            rc = &q_write_.front();
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
