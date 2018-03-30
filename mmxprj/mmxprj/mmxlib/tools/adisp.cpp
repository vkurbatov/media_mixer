
#include "adisp.h"

#include "errno.h"

namespace mmx
{
    namespace tools
    {
        ADispatcher::ADispatcher() :
            q_write_(),
            q_read_()
        {

        }

        int ADispatcher::Write(const char* data, int size, int flags)
        {
            // для начала проверим выходную очередь
            // если она пустая, то пробуем записать сперва то что в очереди

            // флаг управления порционными посылками

            bool f_write = !q_write_.empty();

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

                w_ret = getIO().Write(p.Data(), p.Size(), flags);

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

                        q_write_.pop();

                        // если в очереди есть данные, то пытаемся их передать в следующей транзакции

                        f_write = !q_write_.empty();
                    }
                    else
                    {
                        // данные передали частично

                        // просто смещаем позицию непереданного пакета на w_ret байт

                        p.Seek(w_ret);
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

                    w_ret = getIO().Write(data, size, flags);

                    if (w_ret < 0)
                    {

                        // при любой ошибке сохраняем данные в очереди не передачу

                        q_write_.push(staff::Packet(data, size));

                    }
                    else
                    {

                        // если данне переданы не полностью

                        if (w_ret < size)
                        {

                            // запишем остаток в очередь на передачу

                            q_write_.push(staff::Packet(data + w_ret, size - w_ret));
                        }

                        // сформируем общай счетчик переданных байт

                        w_total += w_ret;
                    }


                }
            }

            return w_ret >= 0 ? w_total : w_ret;
        }

        int ADispatcher::Read(char* data, int size, int flags)
        {
            int r_ret = -EINVAL;

            // общий счетчик прочитанных байт

            int r_total = 0;

            // если транзитный буффер не задан, то читаем в приемную внутреннюю очередь

            if (data == nullptr)
            {

                // если размер задан

                if (size > 0)
                {

                    // выделяем пакет

                    staff::Packet pack(size);

                    r_ret = getIO().Read(pack.Data(), size, flags);

                    if (r_ret < 0)
                    {
                        // если ошибка при чтении

                        // ничего не делаем
                    }
                    else
                    {

                        // подгоняем размер пакета до требуемого

                        if (r_ret < size)
                        {
                            pack.Resize(r_ret);
                        }

                        r_total += r_ret;

                        // в очередь на чтение

                        q_read_.push(std::move(pack));
                    }
                }

            }

            // указатель на буффер задан, транзитная передача

            else
            {

                // если размер задан

                if (size > 0)
                {
                    // сперва прочитаем из приемного буфера, если не пуст

                    bool f_read = !q_read_.empty();

                    while (f_read)
                    {
                        // априори считаем что очередь приемная - пустая

                        f_read = false;

                        auto& p = q_read_.front();

                        // если запрошенных данных меньше, чем в пакете

                        if (size < p.Size())
                        {

                            // копируем только необходимый "кусок"

                            std::copy(p.Data(), p.Data() + size, data);

                            // смещаем указатель данных на запрошенный размер

                            p.Seek(size);

                            r_total += size;

                            // признак что все данные счиали

                            size = 0;

                        }

                        // ситуация когда запрошенных данных больше или равно чем есть в текущем пакете

                        else
                        {
                            //копируем весь пакет в приемный буффер

                            std::copy(p.Data(), p.Data() + p.Size(), data);

                            r_total += p.Size();

                            // изменяем параметры транзитного буфера

                            data += p.Size();

                            size -= p.Size();

                            // удаляем прочитаный пакет из очереди

                            q_read_.pop();

                            // признак провторной и итерации чтения

                            f_read = size > 0 && !q_read_.empty();
                        }

                    } //while

                    // либо не все вычитали из внутренних буферов, либо в них ничего и небыло

                    if (size > 0)
                    {

                        r_ret = getIO().Read(data, size, flags);

                    }
                    else
                    {
                        // ситуация когда буффер вычитали из внутренней очереди

                        r_ret = 0;
                    }
                }
            }

            return r_ret;
        }

        bool ADispatcher::IsWriteEmpty() const
        {
            return q_write_.empty();
        }

        bool ADispatcher::IsReadEmpty() const
        {
            return q_read_.empty();
        }

        void ADispatcher::Reset()
        {
            while (!q_write_.empty())
            {
                q_write_.pop();
            }

            while (!q_read_.empty())
            {
                q_read_.pop();
            }
        }
    }
}
