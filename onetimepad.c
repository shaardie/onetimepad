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

   /* Case: crypt */
   if (strcmp("crypt", argv[1])  == 0) {
      if (argc == 6) {
         
         int pos = atoi(argv[4]);

         if (pos >= 0) {
            return crypt(argv[2], argv[3], pos, argv[5]);
         }

         else {
            fprintf(stderr, "Non-positiv size or pos\n");         
         }
      }

      else {
         fprintf(stderr, "\nFormat:\t crypt [inputfile] [keyfile] "
					"[position in keyfile] [outputfile]\n\n");
         return 0;
      }
   }


   return 0;
}
