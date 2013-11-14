#include <stdlib.h>
#include <stdio.h>
#include "crypt.h"
#include "key.h"
#include "twofish.h"

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

	/* read keyfile */
	char * buf_key = malloc (keyheader.size);
	fseek(keyf, sizeof(header_t), SEEK_CUR);
	if (fread(buf_key,1,keyheader.size,keyf) != keyheader.size) {
		fprintf(stderr, "Could not read key file\n");
		return 1;
	}

	/* key deriation */
   char * twokey;
	void * salt = (void*) &keyheader.salt;
	getkey(&twokey,&salt,"Please enter passphrase to decrypt keyfile: ");

	/* decryption */
	if (twofish_decrypt((char *)buf_key,keyheader.size,twokey)) {
		fprintf(stderr, "Could not decrypt keyfile %s\n",
				key);
		free(buf_key);
		free(buf_put);
		fclose(keyf);
		fclose(inputf);
		return 1;       
	}

	/* XOR key and input */
	int i;
	for (i = 0; i < size_input; i++) {
		buf_put[i] = buf_put[i]^ buf_key[i+keyheader.pos];
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

	/* close all, free all and return success */
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

	/* read keyfile */
	char * buf_key = malloc (keyheader.size);
	fseek(keyf, sizeof(header_t), SEEK_CUR);
	if (fread(buf_key,1,keyheader.size,keyf) != keyheader.size) {
		fprintf(stderr, "Could not read key file\n");
		return 1;
	}

	/* key deriation */
   char * twokey;
	void * salt = (void*) &keyheader.salt;
	getkey(&twokey,&salt,"Please enter passphrase to decrypt keyfile: ");

	/* decryption */
	if (twofish_decrypt((char *)buf_key,keyheader.size,twokey)) {
		fprintf(stderr, "Could not decrypt keyfile %s\n",
				key);
		free(buf_key);
		free(buf_put);
		fclose(keyf);
		fclose(inputf);
		return 1;       
	}

	/*XOR key and input */
	int i;
	for (i = 0; i < inputheader.size; i++) {
		buf_put[i] = buf_put[i]^ buf_key[i+keyheader.pos];
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

	/* close all and return success */
	fclose(keyf);
	return 0;
}
