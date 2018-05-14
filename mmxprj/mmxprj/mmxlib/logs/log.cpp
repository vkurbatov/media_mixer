#include <cstdio>       // форматирование строк
#include <cstdarg>      // работа с переменнам количеством аргументов
#include <ctime>        // работа со временем
#include <cstring>      // для strcpy
#include <mutex>        // для случая многопоточного логирования

#include "logs/log.h"

#include <sys/time.h>   // получение миллисекунд   
#include <sys/stat.h>   // mkdir


#define LOG_BUFFER_SIZE     8196L   // более чем достаточно
#define LOG_HEADER_FORMAT   "%02d.%02d.%02d %02d:%02d:%02d.%06d [%c] "
#define LOG_MAX_FILENAME    256L


namespace mmx
{
    namespace logs
    {
        static std::FILE*       g_file = nullptr;
        static char             g_filename[LOG_MAX_FILENAME] = { '\0' };
        static char             g_dir[LOG_MAX_FILENAME] = { '\0' };
        static char             g_log[LOG_MAX_FILENAME] = { '\0' };
        static bool             g_init = false;
        static bool             g_mt = false;
        static log_level_t      g_max_level = L_DEBUG;
        static std::mutex       g_mutex;
        static unsigned char    g_day;


        int log_init(const char* filename, log_level_t max_level, bool mt)
        {
            // априори устанавливаем ошибку инициализации

            int rc = -1;


            bool l_mt = g_mt;

            if (l_mt)
            {
                g_mutex.lock();
            }

            // лог уже проинициализирован

            if (g_init)
            {

                // проверим первый аргумент

                if (filename == nullptr)
                {
                    // если первый аргумент не задан,
                    // то запрос на деинициализацию

                    if (g_file != nullptr)
                    {
                        std::fclose(g_file);
                        g_file = nullptr;
                    }

                    // для сброса пути достаточно затерминировать первый символ пути

                    *g_filename = '\0';

                    *g_log = '\0';

                    *g_dir = '\0';

                    g_mt = false;

                    g_init = false;

                    g_day = 0;

                    // признак деинициализации

                    rc = 0;
                }
            }

            // лог не инициализирован

            else
            {
                // если путь задан, то поступил запрос на инициализацию

                if (filename != nullptr && *filename != '\0')
                {
                    int l = strlen(filename);

                    std::time_t tim = std::time(nullptr);
                    std::tm* tim_str = std::localtime(&tim);

                    while(l > 0 && filename[l] != '/') l--;

                    if (l > 0)
                    {
                        std::strncpy(g_dir,filename, ++l);

                        g_dir[l] = '\0';

                        mkdir(g_dir, 0666);

                    }

                    std::strcpy(g_log,filename + l);

                    std::sprintf(g_filename, "%s%02d%02d-%s", g_dir, tim_str->tm_mon+1, tim_str->tm_mday, g_log);

                    // std::strcpy(g_filename, filename);

                    g_mt = mt;

                    g_max_level = max_level;

                    g_init = true;

                    // признак успешной инициализации

                    rc = 1;

                }
            }

            if (l_mt)
            {
                g_mutex.unlock();
            }

            return rc;
        }

        // закрытая функция вывода логов на устройство

        void __out(const char* log_message)
        {
            std::time_t tim = std::time(nullptr);
            std::tm* tim_str = std::localtime(&tim);

            //if (g_day != tim_str->tm_mday )
            if (tim_str->tm_sec > 50)
            {
                std::sprintf(g_filename, "%s%02d%02d-%s", g_dir, tim_str->tm_mon+1, tim_str->tm_mday, g_log);
                g_day = tim_str->tm_mday;

                if (g_file != nullptr)
                {
                    std::fflush(g_file);
                    std::fclose(g_file);
                    g_file = nullptr;
                }
            }

            // дескриптор файла пуст, либо открываем первый раз
            // либо предыдущая попытка была неуспешной

            if (g_file == nullptr)
            {

                // если путь указан ненулевой

                if (*g_filename != '\0')
                {
                    // попробуем открыть файл для дозаписи

                    g_file = std::fopen(g_filename,"a");
                }
            }

            // если дескриптор файла открыт
            if (g_file != nullptr)
            {
                int n = std::fputs(log_message, g_file);
                std::fflush(g_file);
            }
            else
            {
                // вывод на консоль
                std::puts(log_message);
            }
        }

        // закрытая функция для вывода сообщений по аргументам

        void __logV(log_level_t level, const char* format, va_list vl)
        {
            // статический буффер для форматирования лог-сообщения

            static char buffer[LOG_BUFFER_SIZE];

            // тут храняться строковые представления уровней логов

            static const char s_levels[] = {'T','D','I','W','E','C'};

            ::timeval tv;

            // временная схема (первый символ @ в форматной строке игнорирует уровень ведения логов)

            if (g_init == true && (level >= g_max_level || (*format == '@' && *(++format) != '\0')))
            {
                bool l_mt = g_mt;

                if (l_mt)
                {
                    g_mutex.lock();
                }

                // сперва получим время

                ::gettimeofday(&tv, nullptr);

                std::tm* tim = std::localtime(&tv.tv_sec);

                // формируем заголовок сообщения и получаем позицию
                // для форматирования пользовательских данных

                auto n = std::sprintf(buffer, LOG_HEADER_FORMAT,
                                      tim->tm_year % 100,
                                      tim->tm_mon + 1,
                                      tim->tm_mday,
                                      tim->tm_hour,
                                      tim->tm_min,
                                      tim->tm_sec,
                                      (int)tv.tv_usec,
                                      s_levels[level]);

                // позиция конца строки нужна для проверки перехода

                n += std::vsprintf(buffer + n, format, vl);

                // если строка пользователя явно не терминирована переходом
                // на новую строку

                if (buffer[n - 1] != '\n')
                {
                    // делаем это сами:

                    buffer[n] = '\n';
                    buffer[n + 1] = '\0';
                }

                __out(buffer);

                if (l_mt)
                {
                    g_mutex.unlock();
                }

            }
        }

        // функции-обертки для вывода форматируемых сообщений

        void log(log_level_t level, const char* format, ...)
        {
            va_list vl;
            va_start (vl, format);

            __logV(level, format, vl);

            va_end(vl);
        }

        void logT(const char* format, ...)
        {
            va_list vl;
            va_start (vl, format);

            __logV(L_TRACE, format, vl);

            va_end(vl);
        }

        void logD(const char* format, ...)
        {
            va_list vl;
            va_start (vl, format);

            __logV(L_DEBUG, format, vl);

            va_end(vl);
        }

        void logI(const char* format, ...)
        {
            va_list vl;
            va_start (vl, format);

            __logV(L_INFO, format, vl);

            va_end(vl);
        }

        void logW(const char* format, ...)
        {
            va_list vl;
            va_start (vl, format);

            __logV(L_WARNING, format, vl);

            va_end(vl);
        }

        void logE(const char* format, ...)
        {
            va_list vl;
            va_start (vl, format);

            __logV(L_ERROR, format, vl);

            va_end(vl);
        }

        void logC(const char* format, ...)
        {
            va_list vl;
            va_start (vl, format);

            __logV(L_CRITICAL, format, vl);

            va_end(vl);
        }
    }
}
