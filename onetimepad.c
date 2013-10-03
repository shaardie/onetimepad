#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypt.h"
#include "header.h"
#include "generate.h"
#include "aes.h"
#include <getopt.h>

/* Main file.  Read in arguments an transfer them
 * to encrypt, decrypt and generate */

void usage(const char* prgm) {
   printf( "Usage: %s [options] command [command options]\n"
			"-- Commands: ----\n"
			"\tgenerate -- generate a new keyfile\n"
			"\tencrypt  -- encrypt a file\n"
         "\tdecrypt  -- decrypt a file\n\n"
			"-- Options ----\n"
			"\t-r       -- Toggle reseed of OpenSSL PRNG (default: enabled)\n"
			,prgm);
}

int main(int argc, char *argv[]) {
   
	/* Enable reseed by default */
	config_t config = { 1 };

	int c;
	while ((c = getopt (argc, argv, "r")) != -1) {
		switch (c) {
			case 'r': 
				config.reseed = !config.reseed;
				break;
		}
	}

   /* Looking for parameters and show otherwise 
    * standard commands */
   if (argc - optind < 1) {
		usage(*argv);
      return 0;
   }

	const char* cmd = argv[optind];
   
   /* Case: generate */
   if (!strcmp("generate", cmd)) {
      if (argc - optind == 3) {
			return generate( &config, atol(argv[optind+1]), argv[optind+2]);
      }
		printf("Command options: generate [size (kb)] [keyfile]\n");
		return 0;
   }

   /* Case: encrypt */
   if (!strcmp("encrypt", cmd)) {
      if (argc - optind == 3) {
         return encrypt(argv[optind+1], argv[optind+2], argv[optind+3]);
      }
		printf("Command options: encrypt [infile] [keyfile] [outfile]\n");
		return 0;
   }

   /* Case: decrypt */
   if (!strcmp("decrypt", cmd)) {
      if (argc - optind == 3) {
         return decrypt(argv[optind+1], argv[optind+2], argv[optind+3]);
      }
		printf("Command options: decrypt [infile] [keyfile] [outfile]\n");
		return 0;
   }
  
   /* Case aes_encrypt */
   if (!strcmp("aes_encrypt", cmd)) {
      if (argc - optind == 1) {
         return aes_encrypt(argv[optind+1]);
      }
		fprintf(stderr, "\n\nERROR\n\n");
		return 0;
   }

   /* Case aes_decrypt */
   if (!strcmp("aes_decrypt", cmd)) {
      if (argc - optind == 1) {
         return aes_decrypt(argv[optind+1]);
      }
		fprintf(stderr, "\n\nERROR\n\n");
		return 0;
   }

   
   /* Case: default */
	usage(*argv);
   return 0;
}
