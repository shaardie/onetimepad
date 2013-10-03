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
			"\t-r       -- Toggle reseed of OpenSSL PRNG (default: reseed)\n"
			"\t-k       -- Toggle if the key part used for decryption shall be\n"
			"\t            kept to enable a second decryption. (default: overwrite)\n"
			,prgm);
}

int main(int argc, char *argv[]) {
   
	/* Default values:
	 * - Reseed
	 * - Do not keep keys */
	config_t config = { 1, 0 };

	int c;
	while ((c = getopt (argc, argv, "rk")) != -1) {
		switch (c) {
			case 'r': 
				config.reseed = !config.reseed;
				break;
			case 'k': 
				config.keep_key = !config.keep_key;
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
      if (argc - optind == 4) {
         return encrypt(&config, argv[optind+1], argv[optind+2], argv[optind+3]);
      }
		printf("Command options: encrypt [infile] [keyfile] [outfile]\n");
		return 0;
   }

   /* Case: decrypt */
   if (!strcmp("decrypt", cmd)) {
      if (argc - optind == 4) {
         return decrypt(&config, argv[optind+1], argv[optind+2], argv[optind+3]);
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
