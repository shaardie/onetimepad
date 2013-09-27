#include <stdlib.h>
#include <stdio.h>

int crypt(char * input, char * key, int pos, char * output) {
   
   FILE * inputf = fopen (input,"r");

   if (!inputf) {
      fprintf(stderr, "Could not open input file\n");
      return 0;
   }

   fseek(inputf,0,SEEK_END);
   int size_input = ftell(inputf);
   fseek(inputf,0,0);

   FILE * outputf = fopen (output,"w");

   if (!outputf) {
      fprintf(stderr, "Could not open output file\n");
      return 0;
   }

   FILE * keyf = fopen (key,"r");

   if (!keyf) {
      fprintf(stderr, "Could not open input file\n");
      return 0;
   }

   fseek(keyf,0,SEEK_END);
   int size_key = ftell(keyf);

   if (pos + size_input <= size_key) {
      
      fseek(keyf,pos,0);
      
      FILE * outputf = fopen (output,"w");

      if (!outputf) {
         fprintf(stderr, "Could not open output file\n");
         return 0;
      }

      int i;
      
      char * buf_input = malloc (size_input);
      fread(buf_input,1,size_input,inputf);
      
      char * buf_output = malloc (size_input);
      
      char * buf_key = malloc (size_input);
      fread(buf_key,1,size_input,keyf);

      for (i = 0; i < size_input; i++) {
         buf_output[i] = buf_input[i]^ buf_key[i];
      }
      fwrite (buf_output,1,size_input,outputf);
      
      free(buf_input);
      free(buf_output);
      free(buf_key);
      fclose(outputf);

   }
   else {
      fprintf(stderr, "Keyfile is too small\n");
      return 0;   
   }

   fclose(inputf);
   fclose(keyf);
   fclose(outputf);
   return pos+size_input;
}
