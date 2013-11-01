#ifndef _HEADER_H_INCLUDED
#define _HEADER_H_INCLUDED

#include <stdint.h>

#define GCRYPT_NO_DEPRECATED
#include <gcrypt.h>

#define STATUS_UNUSED_KEY 0
#define STATUS_DEC_KEY    1
#define STATUS_ENC_KEY    2
#define STATUS_ENCDEC_KEY 3
#define STATUS_ENC_FILE   4

#define USE_OPENSSL   0
#define USE_LIBGCRYPT 1

typedef struct {
   uint8_t status;
   uint64_t pos;
   uint16_t id;
   uint64_t size;
} header_t;

typedef struct {
   int reseed;
   int keep_key;
   int cryptlib;
   gcry_random_level_t random_quality;
	int get_status; 
} config_t;

#endif
