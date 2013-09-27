#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "openssl/rand.h"

int generate ( int size, char * path) {
   
   size *= 1024;
   FILE * f = fopen(path,"w");
   
   if (!f) {
      fprintf(stderr, "Could not open keyfile");
      return 0;
   }

   unsigned char * buf = malloc(size*sizeof(unsigned char));
   if (!RAND_bytes(buf,size)) {
      fprintf(stderr, "Could not create random character");
      return 0;
   }

   if (fwrite(buf, 1, size, f) != size ) {
      fprintf(stderr, "Could not write in keyfile");
      return 0;       
   }

   free(buf);
   fclose(f);
   return 1;
}
