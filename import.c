#include <stdio.h>
#include <stdint.h>
#include "header.h"

/* function to import keyfile to encrypt, to decrypt or both */
int import ( config_t* config, char * path) {

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

	/* Set pos of the new header */
	keyheader.pos = 0;

	/* If keyfile is only for encryption */
	if (config->get_status == STATUS_ENC_KEY) {
		keyheader.status    = STATUS_ENC_KEY;

		/* write new header to file */
		fseek(f,0,SEEK_SET);
		if (fwrite(&keyheader, 1, sizeof(header_t), f)
				!= sizeof(header_t)) {
			fprintf(stderr, "Could not write to keyfile %s", path);
			fclose(f);
			return 1;
		}

		/* close keyfile */
		fclose(f);

		/* rename to .public */
   	char * pubpath = malloc(strlen(path)
				+ 8 * sizeof(char));
   	sprintf(pubpath, "%s.public", path);
		rename(path,pubpath); /* error is missing */
		
		/* free all and return success */
		free(pubpath);
		return 0;
	}

	/* If keyfile is for decryption and encryption 
	 * create new keyfile path.public for encryption
	 **/
	if (config->get_status == STATUS_ENCDEC_KEY) {
		keyheader.status = STATUS_ENC_KEY;

		/* open new file path.public for encryption */
		char * pubpath = malloc(strlen(path)
				+ 8 * sizeof(char));
		sprintf(pubpath, "%s.public", path);
		
		FILE * fpublic = fopen(pubpath,"w");

		if (!fpublic) {
			fprintf(stderr, "Could not open public keyfile %s", pubpath);
			free(pubpath);
			fclose(f);
			return 1;
   	}

		/* write new header to public keyfile */
		if (fwrite(&keyheader, 1, sizeof(header_t), fpublic)
			!= sizeof(header_t)) {
			fprintf(stderr,
					"Could not write to public keyfile %s", pubpath);
			free(pubpath);
      	fclose(f);
			fclose(fpublic);
			return 1;
   	}
		
		/* allocate memory for buffer */
		uint8_t * buf;
		buf = malloc(keyheader.size * sizeof(uint8_t));
		if (!buf) {
			fprintf(stderr, "\nCould not allocate memory\n");
			free(pubpath);
      	fclose(f);
			fclose(fpublic);
			return 1;
		}
		/* Set after header  */
		fseek(f,sizeof(header_t),SEEK_SET);

		/* read key from file */
		if (fread(buf, 1, keyheader.size, f) 
				!= keyheader.size ) {
			fprintf(stderr, "Could not read from fucking keyfile %s\n",
					path);
			fclose(f);
			fclose(fpublic);
			free(pubpath);
			free(buf);
			return 1;
		}
			
		/* write key to public keyfile  */
		if (fwrite(buf, 1, keyheader.size, fpublic)
				!= keyheader.size ) {
      	fprintf(stderr, "Could not write to public keyfile %s\n",
					pubpath);
      	free(buf);
      	free(pubpath);
      	fclose(f);
			fclose(fpublic);
      	return 1;       
   	}
   	
		/* close public keyfile and free all */
		fclose(fpublic);
   	free(pubpath);
   	free(buf);
	}

	/* keyfile for decryption */
	keyheader.status    = STATUS_DEC_KEY;

	/* write new header to file */
	fseek(f,0,SEEK_SET);
	if (fwrite(&keyheader, 1, sizeof(header_t), f)
			!= sizeof(header_t)) {
		fprintf(stderr, "Could not write to keyfile %s", path);
		fclose(f);
		return 1;
	}

	/*close all and return success */
	fclose(f);
	return 0;
	
}
