#include <stdlib.h>
#include <stdio.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <termios.h>
#include <string.h>
#include <stdint.h>

/* Encrypt and decrypt files with aes */

int password (char * pw) {
   
   /* Read password to crypt*/
   struct termios oflags, nflags;
   char * password = calloc(64,sizeof(char));

   /* disabling echo */
   tcgetattr(fileno(stdin), &oflags);
   nflags = oflags;
   nflags.c_lflag &= ~ECHO;
   nflags.c_lflag |= ECHONL;

   if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
      perror("tcsetattr");
      return 1;
   }
   
   /* Read password two times and compare */
   char * password_test = calloc(64,sizeof(char));
   do {

      printf("Please enter a password shorter than "
          "64 characters twice. If the two passwords "
          "are different, you have to type them again.\n"
          "First time:");
      fgets(password, 64*sizeof(char), stdin);
   
      printf("Second time:");
      fgets(password_test, 64*sizeof(char) , stdin);
      

   } while (strcmp(password,password_test) != 0);
   
   free(password_test);
   
   /*  restore terminal */
   if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
      perror("tcsetattr");
      free(password);
      return 1;
   }
}

int aes_encrypt(char * file) {
   char pw [64];
   return 0;
}

int aes_decrypt(char * file) {
   return 0;
}
