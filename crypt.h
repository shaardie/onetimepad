#ifndef _CRYPT_H_INCLUDED
#define _CRYPT_H_INCLUDED

/* Hier kommt der Kompiler nur einmal hin. Beim zweiten mal ist
 *  * _CRYPT_H_INCLUDED definiert und somit ist die Bedingung für #ifndef nicht
 *   * erfüllt. */

int crypt(char * input, char * key, int pos, char * output);

#endif
