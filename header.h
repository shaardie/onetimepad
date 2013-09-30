#ifndef _HEADER_H_INCLUDED
#define _CRYPT_H_INCLUDED

#include <stdint.h>

typedef struct {
	uint64_t rw_status;
	uint64_t pos;
	uint64_t id;
	uint64_t size;
} header_t;

#endif
