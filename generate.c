#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <openssl/rand.h>
#include "generate.h"
#include "header.h"


/* Generate a keyfile [path] with a header and 1042*[size] random
 * bytes using openssl/rand.h
 * 
 **/

int generate ( config_t* config, size_t size, char * path) {
  
	/* redefine size */
   size *= 1024;
	
	/* init header and set header.size */
	header_t h;
   h.size = size;

	/*init memory pointer */
	uint8_t * buf;

	/* case: Using Openssl to create random bytes */
   if (config->cryptlib == USE_OPENSSL) {

		/* create random id for header */
      if (!RAND_bytes((uint8_t*) &h.id, sizeof(uint64_t))) {
         fprintf(stderr, "\nCould not create good random character\n");
         return 1;
      }

      /* init PRNG */
      if (config->reseed) {
         if (RAND_load_file("/dev/random", 32) != 32) {
            fprintf(stderr, "Could not initialize PRNG from /dev/random\n");
            return 1;
         }
      }

		/* init memory */
      buf = malloc(size * sizeof(uint8_t));
      if (!buf) {
         fprintf(stderr, "\nCould not allocate memory\n");
         return 1;
      }

		/* create random bytes */
      if (!RAND_bytes(buf,size)) {
         fprintf(stderr, "\nCould not create random character\n");
         free(buf);
         return 1;
      }

	/* case: Using libgcrypt to create random bytes */
   } else if (config->cryptlib == USE_LIBGCRYPT) {

		/* create random id for header */
      gcry_randomize((uint8_t*) &h.id, sizeof(uint64_t), GCRY_WEAK_RANDOM);

      /* Generate random numbers. We could also use gcry_random_bytes_secure which
       * uses protected memory for the key storage. Unfortunalely we only have
       * very limited secure memory and in the end we store the data on the hard
       * disk anyway.
       **/
      buf = gcry_random_bytes(size, config->random_quality);
      if (!buf) {
         fprintf(stderr, "\nError generating random bytes\n");
         return 1;
      }
   }

	/* open keyfile */
   FILE * f = fopen(path,"w");
   if (!f) {
      fprintf(stderr, "Could not open keyfile %s", path);
      free(buf);
      return 1;
   }

	/* write header to keyfile */
   if (fwrite(&h, 1, sizeof(header_t), f) != sizeof(header_t)) {
      fprintf(stderr, "Could not write to keyfile %s", path);
      free(buf);
      fclose(f);
      return 1;
   }

	/* write random bytes to keyfile */
   if (fwrite(buf, 1, size, f) != size ) {
      fprintf(stderr, "Could not write to keyfile");
      free(buf);
      fclose(f);
      return 1;       
   }

	/* close all, free all and return success */
   fclose(f);
   free(buf);
   return 0;
}
