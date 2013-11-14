#ifndef _KEY_H_INCLUDED
#define _KEY_H_INCLUDED

char * getpasswd(char * passwd, char * msg);
unsigned char * getkey(unsigned char * key, void * salt, char * msg);

#endif
