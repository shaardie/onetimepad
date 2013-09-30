#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <openssl/rand.h>
#include "header.h"

int generate ( size_t size, char * path) {
   
   size *= 1024;

	header_t h;
	h.status = STATUS_ENC_KEY;
	h.pos       = 0;
	h.size      = size;
	if (!RAND_bytes((uint8_t*) &h.id, sizeof(uint64_t))) {
      fprintf(stderr, "Could not create random character");
      return 1;
   }

   uint8_t * buf = malloc(size * sizeof(uint8_t));
	if (!buf) {
		fprintf(stderr, "Could not allocate memory\n");
		return 1;
	}
   if (!RAND_bytes(buf,size)) {
      fprintf(stderr, "Could not create random character");
      return 1;
   }

   FILE * f = fopen(path,"w");
   
   if (!f) {
      fprintf(stderr, "Could not open keyfile %s", path);
      return 1;
   }

	if (fwrite(&h, 1, sizeof(header_t), f) != sizeof(header_t)) {
      fprintf(stderr, "Could not write to keyfile %s", path);
      return 1;
   }
   if (fwrite(buf, 1, size, f) != size ) {
      fprintf(stderr, "Could not write to keyfile");
      return 1;       
   }
   fclose(f);

	h.status = STATUS_DEC_KEY;

   char* pubpath = malloc(strlen(path) + 8 * sizeof(char));
   sprintf(pubpath, "%s.public", path);

   f = fopen(pubpath,"w");
   
   if (!f) {
      fprintf(stderr, "Could not open keyfile %s", path);
      return 1;
   }

	if (fwrite(&h, 1, sizeof(header_t), f) != sizeof(header_t)) {
      fprintf(stderr, "Could not write to keyfile %s", pubpath);
      return 1;
   }
   if (fwrite(buf, 1, size, f) != size ) {
      fprintf(stderr, "Could not write to keyfile");
      return 1;       
   }
   fclose(f);
   free(pubpath);
   free(buf);
   return 0;
}
