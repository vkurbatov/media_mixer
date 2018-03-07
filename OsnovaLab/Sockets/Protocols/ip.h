#ifndef _IP_HEADER_H
#define _IP_HEADER_H


typedef struct _IP4HEADER
{
	unsigned char  len : 4;			// ����� ���������
	unsigned char  ver : 4;			// ������ ���������
	unsigned char  tos;				// ��� ������� 
	unsigned short length;			// ����� ����� ������ 
	unsigned short id;				// Id 
	unsigned short flgs_offset;		// ����� � ��������
	unsigned char  ttl;				// ����� ����� 
	unsigned char  protocol;		// �������� 
	unsigned short xsum;			// ����������� ����� 
	unsigned long  src;				// IP-����� ����������� 
	unsigned long  dest;			// IP-����� ���������� 
	//unsigned short params[4];		// ��������� (�� 320 ���)
	//unsigned char  data[0];		// ������ (�� 65535 �������)
}IP4HEADER, *PIP4HEADER;

#endif