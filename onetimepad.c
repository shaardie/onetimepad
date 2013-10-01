#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypt.h"
#include "generate.h"
#include "aes.h"

/* Main file.  Read in arguments an transfer them
 * to encrypt, decrypt and generate */

int main(int argc, char *argv[]) {
   
   /* Looking for parameters and show otherwise 
    * standard commands */
   if (argc < 2) {
      fprintf(stderr, "\nStandard commands:\n"
				"\tgenerate -- generate a new keyfile\n"
				"\tencrypt\t -- encrypt a file\n"
            "\tdecrypt\t -- decrypt a file\n\n");
      return 1;
   }
   
   /* Case: generate */
   if (strcmp("generate", argv[1])  == 0) {
      if (argc == 4) {

         int size = atoi(argv[2]);
      
         if (size > 0) {
            return generate(size,argv[3]);
         }
         
         else {
            fprintf(stderr, "\nNon-positiv size\n\n");
            return 0;
         }
      }
      
      else {
         fprintf(stderr, "\nFormat:\t generate [size (kb)]"
         " [keyfile]\n\n");
         return 0;
      }
   }

   /* Case: encrypt */
   if (strcmp("encrypt", argv[1])  == 0) {
      if (argc == 5) {
         return encrypt(argv[2], argv[3], argv[4]);
      }

      else {
         fprintf(stderr, "\nFormat:\t encrypt [infile] [keyfile] "
					"[outfile]\n\n");
         return 0;
      }
   }


   /* Case: decrypt */
   if (strcmp("decrypt", argv[1])  == 0) {
      if (argc == 5) {
         
         return decrypt(argv[2], argv[3], argv[4]);
      }

      else {
         fprintf(stderr, "\nFormat:\t decrypt [infile] [keyfile] "
					"[outfile]\n\n");
         return 0;
      }
   }
  
   /* Case aes_encrypt */
   if (strcmp("aes_encrypt", argv[1])  == 0) {
      if (argc == 3) {
         
         return aes_encrypt(argv[2]);
      }

      else {
         fprintf(stderr, "\n\nERROR\n\n");
         return 0;
      }
   }

   /* Case aes_decrypt */
   if (strcmp("aes_decrypt", argv[1])  == 0) {
      if (argc == 3) {
         
         return aes_decrypt(argv[2]);
      }

      else {
         fprintf(stderr, "\n\nERROR\n\n");
         return 0;
      }
   }

   
   /* Case: default */
   fprintf(stderr, "\nStandard commands:\n"
			"\tgenerate -- generate a new keyfile\n"
			"\tencrypt\t -- encrypt a file\n"
         "\tdecrypt\t -- decrypt a file\n\n");
   return 0;
}
