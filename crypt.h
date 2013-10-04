#ifndef _CRYPT_H_INCLUDED
#define _CRYPT_H_INCLUDED
#include "header.h"

int encrypt(config_t* config, char * input, char * key, char * output);
int decrypt(config_t* config, char * input, char * key, char * output);

#endif
