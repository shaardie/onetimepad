#include <stdio.h>
#include <stdint.h>
#include "header.h"

int import ( config_t* config, char * path) {
   
   FILE * f = fopen(path,"r+");
   
   if (!f) {
      fprintf(stderr, "Could not open keyfile %s", path);
      return 1;
   }

   fseek(f,0,SEEK_END);
   size_t size = ftell(f) - sizeof(header_t);
   fseek(inputf,0,0);

   header_t keyheader;
   
	if (fread(&keyheader,1,sizeof(header_t),f) != sizeof(header_t) ) {
      fprintf(stderr, "Could not read from keyfile %s\n", path);
      fclose(f);
      return 1;
   }

   if (keyheader.status != STATUS_UNUSED_KEY ) {
      fprintf(stderr, "Not a valid keyfile to import\n");
      fclose(f);
      return 1;
   }

  	/*hier müssen 2 gemacht werden*/

   keyheader.status    = config->get_status;
   keyheader.pos       = 0;
   keyheader.size      = size;
  
   fseek(f,0,SEEK_END);
   if (fwrite(&keyheader, 1, sizeof(header_t), f) != sizeof(header_t)) {
      fprintf(stderr, "Could not write to keyfile %s", path);
      fclose(f);
      return 1;
   }

   fclose(f);

   return 0;
}
