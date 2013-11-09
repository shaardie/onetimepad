#ifndef _TWOFISH_H_INCLUDED
#define _TWOFISH_H_INCLUDED

int twofish_encrypt( char * in,
		uint64_t inlen,
		char * key );

int twofish_decrypt( char * in,
		uint64_t inlen,
		char * key );

#endif
