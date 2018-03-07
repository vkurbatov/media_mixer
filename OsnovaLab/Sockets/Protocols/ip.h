#ifndef _IP_HEADER_H
#define _IP_HEADER_H


typedef struct _IP4HEADER
{
	unsigned char  len : 4;			// длина заголовка
	unsigned char  ver : 4;			// версия заголовка
	unsigned char  tos;				// тип сервиса 
	unsigned short length;			// длина всего пакета 
	unsigned short id;				// Id 
	unsigned short flgs_offset;		// флаги и смещение
	unsigned char  ttl;				// время жизни 
	unsigned char  protocol;		// протокол 
	unsigned short xsum;			// контрольная сумма 
	unsigned long  src;				// IP-адрес отправителя 
	unsigned long  dest;			// IP-адрес назначения 
	//unsigned short params[4];		// параметры (до 320 бит)
	//unsigned char  data[0];		// данные (до 65535 октетов)
}IP4HEADER, *PIP4HEADER;

#endif