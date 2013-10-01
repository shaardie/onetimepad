#ifndef _AES_H_INCLUDED
#define _AES_H_INCLUDED

/* Hier kommt der Kompiler nur einmal hin. Beim zweiten mal ist
 *  * _AES_H_INCLUDED definiert und somit ist die Bedingung für #ifndef nicht
 *   * erfüllt. */

int aes_encrypt(char * file);
int aes_decrypt(char * file);

#endif
