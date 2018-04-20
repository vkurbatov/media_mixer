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
            unsigned short	pack_lost;		// количество пакетов следующих за текущим
											// (0 - если однопакетная передача)
            unsigned short	pack_id;        // уникальный идентификатор пакета в трансзакции
			unsigned short	ver:8;			// версия протокола
            unsigned short	comp:8;			// совместимая версия для функции (0 - совпадает c ver)
            unsigned short	length;			// размер данных пользователя + размер заголовка
			unsigned char	reserved1[8];	// зарезервированно
            union                           // уточняющие параметры
            {
                int         n_params[2];    //
                short       s_params[4];	//
                char        c_params[8];	//
            };
			
            
        }MSG_HEADER,*PMSG_HEADER;

        typedef struct _MSG
        {
            MSG_HEADER  header;
            char        data[1];
        }MSG, *PMSG;

#pragma pack(pop)

    }
}

#endif
