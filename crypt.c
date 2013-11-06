#include <stdlib.h>
#include <stdio.h>
#include "crypt.h"

/* function to encrypt file */
int encrypt(config_t* config, char * input, char * key, char * output) {
  
	/* open input file to encrypt */
   FILE * inputf = fopen (input,"r");

   if (!inputf) {
      fprintf(stderr, "Could not open input file\n");
      return 1;
   }

	/* calculate size of input file */
	fseek(inputf,0,SEEK_END);
   size_t size_input = ftell(inputf);
   fseek(inputf,0,0);

	/* open keyfile */
   FILE * keyf = fopen (key,"r+");

   if (!keyf) {
      fprintf(stderr, "Could not open input file\n");
      return 1;
   }

	/* check if keyfile is for encrypting */
   header_t keyheader;
   if (fread(&keyheader,1,sizeof(header_t),keyf) != sizeof(header_t) 
         || keyheader.status != STATUS_ENC_KEY) {
      fprintf(stderr, "Wrong keyfile\n");
      return 1;
   }

	/* check if keyfile is big enough to encrypt input file */
   if (size_input > keyheader.size - keyheader.pos) {
      fprintf(stderr, "Keyfile is to small\n");
      return 1;   
   }

	/* create new header for input file */
   header_t encheader;
   encheader        = keyheader;
   encheader.status = STATUS_ENC_FILE;
   encheader.size   = size_input;

	/* open output file */
   FILE * outputf = fopen(output,"w");

   if (!outputf) {
      fprintf(stderr, "Could not open output file\n");
      return 1;
   }
  
	/* write new header to output file*/
   if (fwrite (&encheader,1,sizeof(header_t),outputf) != sizeof(header_t)) {
      fprintf(stderr, "Could not write to output file\n");
      return 1;
   }
   
	/* read input file */
   char * buf_put = malloc (size_input);
   if (fread(buf_put,1,size_input,inputf) != size_input) {
      fprintf(stderr, "Could not read input file\n");
      return 1;
   }
   
	/* read part of keyfile which is needed to encrypt */
   char * buf_key = malloc (size_input);
   fseek(keyf, keyheader.pos, SEEK_CUR);
   if (fread(buf_key,1,size_input,keyf) != size_input) {
      fprintf(stderr, "Could not read key file\n");
      return 1;
   }

	/* XOR key and input */
   int i;
   for (i = 0; i < size_input; i++) {
      buf_put[i] = buf_put[i]^ buf_key[i];
   }

	/* write encrypted input to output file */
   if (fwrite (buf_put,1,size_input,outputf) != size_input) {
      fprintf(stderr, "Could not write to output file\n");
      return 1;
   }
   
	/* free buffer for input and key */
   free(buf_put);
   free(buf_key);

	/* close input and output file */
   fclose(inputf);
   fclose(outputf);

   /* Update header of keyfile */
   keyheader.pos += size_input;
   fseek(keyf,0,0);

	/* write updated header to keyfile */
   if (fwrite(&keyheader, 1, sizeof(header_t), keyf) != sizeof(header_t)) {
      fprintf(stderr, "Could not write to keyfile %s\n", key);
      return 1;
   }
	
	/* override used keyfile with zeros */
   fseek(keyf,keyheader.pos - size_input + sizeof(header_t),0);
   uint8_t * buf = calloc( sizeof(uint8_t), size_input );
   if (fwrite(buf, 1, size_input, keyf) != size_input ) {
      fprintf(stderr, "Could not write to keyfile %s\n", key);
      return 1;       
   }

	/* close all, free all and return success */
   free(buf);
   fclose(keyf);
   return 0;
}

/* function to decrypt file */
int decrypt(config_t* config, char * input, char * key, char * output) {
   
	/* open input file to encrypt */
   FILE * inputf = fopen (input,"r");

   if (!inputf) {
      fprintf(stderr, "Could not open input file\n");
      return 1;
   }

	/* read header of input file */
   header_t inputheader;
   if (fread(&inputheader,1,sizeof(header_t),inputf) != sizeof(header_t) 
         || inputheader.status != STATUS_ENC_FILE) {
      fprintf(stderr, "Wrong input file\n");
      return 1;
   }

	/* open keyfile */
   FILE * keyf = fopen (key,"r+");

   if (!keyf) {
      fprintf(stderr, "Could not open input file\n");
      return 1;
   }

	/* check if keyfile is for decrypting */
   header_t keyheader;
   if (fread(&keyheader,1,sizeof(header_t),keyf) != sizeof(header_t) 
         || keyheader.status != STATUS_DEC_KEY) {
      fprintf(stderr, "Wrong keyfile\n");
      return 1;
   }

	/* check if identifier match */
   if (inputheader.id != keyheader.id) {
      fprintf(stderr, "Identifier do not match\n");
      return 1;
   }
   
	/* open output file */
   FILE * outputf = fopen(output,"w");

   if (!outputf) {
      fprintf(stderr, "Could not open output file\n");
      return 1;
   }
  
	/* read input file */
   char * buf_put = malloc (inputheader.size);
   if (fread(buf_put,1,inputheader.size,inputf) != inputheader.size) {
      fprintf(stderr, "Could not read input file\n");
      return 1;
   }
   
	/* read part of keyfile used for encryption */
   char * buf_key = malloc (inputheader.size);
   fseek(keyf,inputheader.pos,SEEK_CUR);
   if (fread(buf_key,1,inputheader.size,keyf) != inputheader.size) {
      fprintf(stderr, "Could not read key file\n");
      return 1;
   }

	/*XOR key and input */
   int i;
   for (i = 0; i < inputheader.size; i++) {
      buf_put[i] = buf_put[i]^ buf_key[i];
   }

	/* write decrypted input to output file */
   if (fwrite (buf_put,1,inputheader.size,outputf) != inputheader.size) {
      fprintf(stderr, "Could not write to output file\n");
      return 1;
   }
   
	/* free buffer for input and key */
   free(buf_put);
   free(buf_key);

	/* close input and output file */
   fclose(inputf);
   fclose(outputf);

   /* Update header of keyfile */
   fseek(keyf,inputheader.pos + sizeof(header_t),SEEK_SET);

	/* override used keyfile with zeros if not should be kept */
   if (!config->keep_key) {
      uint8_t * buf = calloc( sizeof(uint8_t), inputheader.size );
      if (fwrite(buf, 1, inputheader.size, keyf) != inputheader.size ) {
         fprintf(stderr, "Could not write to keyfile %s\n", key);
         return 1;       
      }
      free(buf);
   }

	/* close all and return success */
   fclose(keyf);
   return 0;
}
