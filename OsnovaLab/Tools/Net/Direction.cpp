#include "OsnovaLab/Tools/Net/Direction.h"
#include "OsnovaLab/Thread/UniqueLock.h"


namespace OsnovaLab
{
    namespace Tools
    {
        namespace Net
        {
            Direction::Direction(Sockets::IClient& client) :
                client_(client)
            {

            }

            Direction::~Direction()
            {

            }


            Sockets::socket_result_t Direction::Dispatch(const Sockets::event_mask_t& eventMask)
            {
                Sockets::socket_result_t rc = Sockets::DEFAULT_E_INVARG;

                Sockets::socket_result_t w_rc = 0, r_rc = 0;


                // пришло событие о возможности передачи данных
                if (eventMask & Sockets::EV_WRITE)
                {
                     Thread::UniqueLock wr_lock(wr_mutex_);
                    // работает только если в выходное очереди есть хоть какие то данные на передачу
                    if (send_queue_.Count() > 0)
                    {
                        // для прямого доступа к буферу данных, дабы сократить количество операций
                        // буферизированного воода-вывода
                        auto front = send_queue_.Front();

                        w_rc = client_.Send((Sockets::client_data_ptr_t)(front->Data()), front->Size());

                        // отправить могли не все
                        if (w_rc > 0)
                        {
                            // pop с нулевым указателем просто передвинет смещение чтения в пакете
                            send_queue_.Pop(nullptr, w_rc);
                        }
                        else
                        {
                            // только EWOULDBLOCK считаем нормальной ситуацией при завершении
                            if (w_rc != Sockets::DEFAULT_E_UNBLOCK)
                            {
                                return rc = w_rc;
                            }
                            else
                            {
                                w_rc = 0;
                            }

                        }
                    }
                }

                // пришло событии о наличии данных в буфере чтения
                if (eventMask & Sockets::EV_READ)
                {
                    Thread::UniqueLock rd_lock(rd_mutex_);
                    char buff[1600];
                    r_rc = client_.Recv(buff, sizeof(size_t));

                    if (r_rc > 0)
                    {
                        recv_queue_.Push(buff,r_rc);
                    }
                    else
                    {
                        if (r_rc != Sockets::DEFAULT_E_UNBLOCK)
                        {
                            return rc = r_rc;
                        }
                        else
                        {
                            r_rc = 0;
                        }
                    }
                }

                rc = w_rc + r_rc;

                return rc;
            }

            Sockets::socket_size_t Direction::GetSendQueueStatus() const
            {

                return send_queue_.TotalSize();
            }

            Sockets::socket_size_t Direction::GetRecvQueueStatus() const
            {
                return recv_queue_.TotalSize();
            }


            //IClient
            Sockets::client_size_t  Direction::Send(const Sockets::client_data_ptr_t data, Sockets::client_size_t size)
            {
                Thread::UniqueLock wr_lock(wr_mutex_);

                // сокеты неблокирующие, по этому как минимум без одного уровня буферизации не обойтись
                // так как нужно обрабатывать входной и выходной поток асинхронно. Для этого пригодятся пакетные очереди
                // с дозаписью и блочным чтением.

                return send_queue_.Push((Data::packet_data_ptr_t)data, size);
            }

            Sockets::client_size_t  Direction::Recv(Sockets::client_data_ptr_t data, Sockets::client_size_t size)
            {              
                Thread::UniqueLock rd_lock(rd_mutex_);

                // читаем только из очереди

                return recv_queue_.Pop((Data::packet_data_ptr_t)data, size);
            }

            const Sockets::SocketPtr&  Direction::GetHandle() const
            {
                return client_.GetHandle();
            }
        }
    }
}
