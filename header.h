#ifndef _HEADER_H_INCLUDED
#define _HEADER_H_INCLUDED

#include <stdint.h>

#define STATUS_ENC_KEY  1
#define STATUS_DEC_KEY  0
#define STATUS_ENC_FILE 42

typedef struct {
	uint64_t status;
	uint64_t pos;
	uint64_t id;
	uint64_t size;
} header_t;

typedef struct {
	int reseed;
} config_t;

#endif
