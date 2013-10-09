#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypt.h"
#include "header.h"
#include "generate.h"
#include "keycrypt.h"
#include <getopt.h>

/* Main file.  Read in arguments an transfer them
 * to encrypt, decrypt and generate */

void usage(const char* prgm) {
   printf( "\nUsage: %s [options] command [command options]\n\n"
         "-- Commands: ----\n"
         "\tgenerate -- generate a new keyfile\n"
         "\tencrypt  -- encrypt a file\n"
         "\tdecrypt  -- decrypt a file\n\n"
         "-- Options ----\n"
         "\t-r       -- Toggle reseed of OpenSSL PRNG (default: reseed)\n"
         "\t-k       -- Toggle if the key part used for decryption shall be\n"
         "\t-l lib   -- Library to use for encryption and random number generation\n"
         "\t-s       -- Use very strong pseudo random number generator\n"
         "\t            kept to enable a second decryption. (default: overwrite)\n\n"
         ,prgm);
}

int main(int argc, char *argv[]) {
   
   /* Default values:
    * - Reseed
    * - Do not keep keys 
    * - use openssl 
    * - use gcrypt random quality strong */
   config_t config = { 1, 0, USE_OPENSSL, GCRY_STRONG_RANDOM };

   /* Read in options for reseed and keeping key */
   int c;
   while ((c = getopt (argc, argv, "rkl:s")) != -1) {
      switch (c) {
         case 'r': 
            config.reseed = !config.reseed;
            break;
         case 'k': 
            config.keep_key = !config.keep_key;
            break;
         case 'l':
            config.cryptlib = strcmp(optarg, "gcrypt")
               ? USE_OPENSSL : USE_LIBGCRYPT;
            break;
         case 's':
            config.random_quality = GCRY_VERY_STRONG_RANDOM;
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
  
   /* Case: default */
   usage(*argv);
   return 0;
}
