#ifndef _GENERATE_H_INCLUDED
#define _GENERATE_H_INCLUDED

/* Hier kommt der Kompiler nur einmal hin. Beim zweiten mal ist
 *  * _GENERATE_H_INCLUDED definiert und somit ist die Bedingung für #ifndef nicht
 *   * erfüllt. */

int generate( int size, char * path);

#endif
