#include <stdio.h>
#include <stdint.h>
#include "header.h"
#include "twofish.h"
#include "key.h"

/* function to import keyfile to encrypt, to decrypt or both */
int import ( config_t* config, char * path) {

	if (!(config->get_status == STATUS_ENC_KEY 
				|| config->get_status == STATUS_DEC_KEY )) {
		fprintf(stderr, "Not a valid key status.\n");
		return 1;
	}

	/* Open keyfile path to read and write */
	FILE * f = fopen(path,"r+");
	if (!f) {
		fprintf(stderr, "Could not open keyfile %s", path);
		return 1;
	}

	/* Create new header */
	header_t keyheader;

	/* Read header from keyfile */
	if (fread(&keyheader,1,sizeof(header_t),f) 
			!= sizeof(header_t) ) {
		fprintf(stderr, "Could not read from keyfile %s\n", path);
		fclose(f);
		return 1;
	}
	
	/* Set pos and status of the new header */
	keyheader.pos    = 0;
	keyheader.status = config->get_status; 
	
	/* write new header to file */
	fseek(f,0,SEEK_SET);
	if (fwrite(&keyheader, 1, sizeof(header_t), f)
			!= sizeof(header_t)) {
		fprintf(stderr, "Could not write to keyfile %s", path);
		fclose(f);
		return 1;
	}

	/* allocate memory for buffer */
	uint8_t * buf;
	buf = malloc(keyheader.size * sizeof(uint8_t));
	if (!buf) {
		fprintf(stderr, "\nCould not allocate memory\n");
		fclose(f);
		return 1;
	}
	
	/* read random bytes */
	fseek(f,sizeof(header_t),SEEK_SET);
	if (fread( buf, 1, keyheader.size, f) 
			!= keyheader.size ) {
		fprintf(stderr, "Could not read from keyfile %s\n",
				path);
		fclose(f);
		free(buf);
		return 1;
	}

	/* key deriation */
   unsigned char * key;
	void * salt;
	getkey(key,salt,"Please enter decryption passphrase: ");

	/* read key in crypt context */
	if (twofish_decrypt((char *)buf,keyheader.size,(char *)key)) {
		fprintf(stderr, "Could not decrypt keyfile %s\n",
				path);
		free(buf);
		fclose(f);
		return 1;       
	}
	



	/* decryption */
	if (twofish_decrypt((char *)buf,keyheader.size,(char *)key)) {
		fprintf(stderr, "Could not decrypt keyfile %s\n",
				path);
		free(buf);
		fclose(f);
		return 1;       
	}
	
	/* placeholder */
	char * key2      = "abcdefg abcdefg abcdefg abcdefg ";

	/* decryption */
	if (twofish_encrypt((char *)buf,keyheader.size,key2)) {
		fprintf(stderr, "Could not encrypt keyfile %s\n",
				path);
		free(buf);
		fclose(f);
		return 1;       
	}

	/* write decrypted random bytes to keyfile  */
	fseek(f,sizeof(header_t),SEEK_SET);
	if (fwrite(buf, 1, keyheader.size, f)
			!= keyheader.size ) {
		fprintf(stderr, "Could not write to public keyfile %s\n",
				path);
		free(buf);
		fclose(f);
		return 1;       
	}

	/* close keyfile, free buf */
	free(buf);
	fclose(f);

	/* If keyfile is for encryption */
	if (keyheader.status == STATUS_ENC_KEY) {

		/* rename to .public */
		char * pubpath = malloc(strlen(path)
				+ 8 * sizeof(char));
		sprintf(pubpath, "%s.public", path);
		rename(path,pubpath); /* error is missing */
		
		/* free all and return success */
		free(pubpath);
	}

	return 0;

}
