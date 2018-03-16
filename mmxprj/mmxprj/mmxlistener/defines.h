#ifndef _MMXLST_DEFINES_H
#define _MMXLST_DEFINES_H

typedef void(*callback_routine)(unsigned short, char*, int);

int listener(unsigned int addr, unsigned short begin, unsigned short end, int step = 1);

int create_dir(unsigned short begin, unsigned short end, int step);

int dispatch(int sock, int dir_id);

int process_dir(int dir_id, callback_routine call);

int destroy_dir(int dir_id);

#endif
