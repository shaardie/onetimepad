#include <stdlib.h>
#include <stdio.h>
#include "header.h"

int encrypt(char * input, char * key, char * output) {
   
   FILE * inputf = fopen (input,"r");

   if (!inputf) {
      fprintf(stderr, "Could not open input file\n");
      return 1;
   }

   fseek(inputf,0,SEEK_END);
   size_t size_input = ftell(inputf);
   fseek(inputf,0,0);

   FILE * keyf = fopen (key,"r+");

   if (!keyf) {
      fprintf(stderr, "Could not open input file\n");
      return 1;
   }

   header_t keyheader;
   if (fread(&keyheader,1,sizeof(header_t),keyf) != sizeof(header_t) 
         || keyheader.status != STATUS_ENC_KEY) {
      fprintf(stderr, "Wrong keyfile\n");
      return 1;
   }

   if (size_input > keyheader.size - keyheader.pos) {
      fprintf(stderr, "Keyfile is to small\n");
      return 1;   
   }

   header_t encheader;
   encheader        = keyheader;
   encheader.status = STATUS_ENC_FILE;
   encheader.size   = size_input;

   FILE * outputf = fopen(output,"w");

   if (!outputf) {
      fprintf(stderr, "Could not open output file\n");
      return 1;
   }
  
   if (fwrite (&encheader,1,sizeof(header_t),outputf) != sizeof(header_t)) {
      fprintf(stderr, "Could not write to output file\n");
      return 1;
   }
   
   char * buf_put = malloc (size_input);
   if (fread(buf_put,1,size_input,inputf) != size_input) {
      fprintf(stderr, "Could not read input file\n");
      return 1;
   }
   
   char * buf_key = malloc (size_input);
   fseek(keyf, keyheader.pos, SEEK_CUR);
   if (fread(buf_key,1,size_input,keyf) != size_input) {
      fprintf(stderr, "Could not read key file\n");
      return 1;
   }

   int i;
   for (i = 0; i < size_input; i++) {
      buf_put[i] = buf_put[i]^ buf_key[i];
   }

   if (fwrite (buf_put,1,size_input,outputf) != size_input) {
      fprintf(stderr, "Could not write to output file\n");
      return 1;
   }
   
   free(buf_put);
   free(buf_key);

   fclose(inputf);
   fclose(outputf);

   /* Update keyfile */
   keyheader.pos += size_input;
   fseek(keyf,0,0);

	if (fwrite(&keyheader, 1, sizeof(header_t), keyf) != sizeof(header_t)) {
      fprintf(stderr, "Could not write to keyfile %s\n", key);
      return 1;
   }
   fseek(keyf,keyheader.pos - size_input + sizeof(header_t),0);
   uint8_t * buf = calloc( sizeof(uint8_t), size_input );
   if (fwrite(buf, 1, size_input, keyf) != size_input ) {
      fprintf(stderr, "Could not write to keyfile %s\n", key);
      return 1;       
   }
   free(buf);
   fclose(keyf);

   return 0;
}


int decrypt(char * input, char * key, char * output) {
   
   FILE * inputf = fopen (input,"r");

   if (!inputf) {
      fprintf(stderr, "Could not open input file\n");
      return 1;
   }

   header_t inputheader;
   if (fread(&inputheader,1,sizeof(header_t),inputf) != sizeof(header_t) 
         || inputheader.status != STATUS_ENC_FILE) {
      fprintf(stderr, "Wrong input file\n");
      return 1;
   }

   FILE * keyf = fopen (key,"r+");

   if (!keyf) {
      fprintf(stderr, "Could not open input file\n");
      return 1;
   }

   header_t keyheader;
   if (fread(&keyheader,1,sizeof(header_t),keyf) != sizeof(header_t) 
         || keyheader.status != STATUS_DEC_KEY) {
      fprintf(stderr, "Wrong keyfile\n");
      return 1;
   }

   if (inputheader.id != keyheader.id) {
      fprintf(stderr, "Identifier do not match\n");
      return 1;
   }
   
   FILE * outputf = fopen(output,"w");

   if (!outputf) {
      fprintf(stderr, "Could not open output file\n");
      return 1;
   }
  
   char * buf_put = malloc (inputheader.size);
   if (fread(buf_put,1,inputheader.size,inputf) != inputheader.size) {
      fprintf(stderr, "Could not read input file\n");
      return 1;
   }
   
   char * buf_key = malloc (inputheader.size);
   fseek(keyf,inputheader.pos,SEEK_CUR);
   if (fread(buf_key,1,inputheader.size,keyf) != inputheader.size) {
      fprintf(stderr, "Could not read key file\n");
      return 1;
   }

   int i;
   for (i = 0; i < inputheader.size; i++) {
      buf_put[i] = buf_put[i]^ buf_key[i];
   }

   if (fwrite (buf_put,1,inputheader.size,outputf) != inputheader.size) {
      fprintf(stderr, "Could not write to output file\n");
      return 1;
   }
   
   free(buf_put);
   free(buf_key);

   fclose(inputf);
   fclose(outputf);

   /* Update keyfile */
   fseek(keyf,inputheader.pos + sizeof(header_t),SEEK_SET);
   uint8_t * buf = calloc( sizeof(uint8_t), inputheader.size );
   if (fwrite(buf, 1, inputheader.size, keyf) != inputheader.size ) {
      fprintf(stderr, "Could not write to keyfile %s\n", key);
      return 1;       
   }
   free(buf);
   fclose(keyf);

   return 0;
}
