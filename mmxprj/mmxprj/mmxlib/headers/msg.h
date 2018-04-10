#ifndef _MMX_HEADERS_MSG_H
#define _MMX_HEADERS_MSG_H

namespace mmx
{
    namespace headers
    {
    	
        const unsigned short    MSG_MAGIC = 0x1200;
        const unsigned char     VER = 1;

#pragma pack(push, 1)

        typedef struct _MSG_HEADER
        {
        
			unsigned short	magic;			// обязательная сигнатура
			unsigned short	msg;			// ID - сообщения (функции)
			unsigned short	pac_lost;		// количество пакетов следующих за текущим 
											// (0 - если однопакетная передача)
			unsigned short	pac_id;			// уникальный идентификатор пакета в трансзакции 
			unsigned short	ver:8;			// версия протокола
			unsigned short	camp:8;			// совместимая версия для функции (0 - совпадает c ver)
			unsigned short	length;			// размер данных пользователя
			unsigned char	reserved1[8];	// зарезервированно
			int				any1;			// уточняющий параметр 1
			int				any2;			// уточняющий параметр 2
			
            
        }MSG_HEADER,*PMSG_HEADER;

#pragma pack(pop)

    }
}

#endif
