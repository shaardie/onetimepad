#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypt.h"
#include "generate.h"
#include "openssl/rand.h"


int main(int argc, char *argv[]) {
   
   /* Testing if there are parameters */
   if (argc < 2) {
      fprintf(stderr, "\nStandart commands:\n"
				"\tgenerate -- generate a new keyfile\n"
				"\tcrypt\t -- en/decrypt a file\n\n");
      return 0;
   }
   
   /* Case: generate */
   if (strcmp("generate", argv[1])  == 0) {
      if (argc == 4) {

         int size = atoi(argv[2]);
      
         if (size > 0) {
            return generate(size,argv[3]);
         }
         
         else {
            fprintf(stderr, "Non-positiv size.\n");
            return 0;
         }
      }
      
      else {
         fprintf(stderr, "\nFormat:\t generate [size (kb)] [keyfile]\n\n");
         return 0;
      }
   }

   /* Case: encrypt */
   if (strcmp("encrypt", argv[1])  == 0) {
      if (argc == 5) {
         return encrypt(argv[2], argv[3], argv[4]);
      }

      else {
         fprintf(stderr, "\nFormat:\t encrypt [inputfile] [keyfile] "
					"[position in keyfile] [outputfile]\n\n");
         return 0;
      }
   }


   /* Case: decrypt */
   if (strcmp("decrypt", argv[1])  == 0) {
      if (argc == 5) {
         
         return decrypt(argv[2], argv[3], argv[4]);
      }

      else {
         fprintf(stderr, "\nFormat:\t decrypt [inputfile] [keyfile] "
					"[position in keyfile] [outputfile]\n\n");
         return 0;
      }
   }

   fprintf(stderr, "\nUnknown Yheaklögfd\n");
   

   return 0;
}
