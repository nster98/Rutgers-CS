#include <stdio.h> 
#include <stdlib.h> 
int main(int argc, char *argv[]) { 
   if (argc < 3 || argc > 4) {
      printf("Usage:./myfiles file_in_1 file_in_2 [file_out (default:myfile.out)]");
   }
   else {
      // Open two files to be merged 
      FILE *fp1 = fopen(argv[1], "r");
      FILE *fp2 = fopen(argv[2], "r");
      FILE *fp3;
      
      char *file_out;
      // Open file to store the result
      if (argc == 3) {
         FILE *fp3 = fopen("myfile.out", "w"); 
         file_out = "myfile.out";
      }
      else {
         FILE *fp3 = fopen(argv[3], "w");
         file_out = argv[3]; 
      }
      char c; 
      
      if (fp1 == NULL || fp2 == NULL || fp3 == NULL) { 
         puts("No such file or directory"); 
         exit(0); 
      }

      // Copy contents of first file to file3.txt 
      while ((c = fgetc(fp1)) != EOF) 
      fputc(c, fp3); 
         
      // Copy contents of second file to file3.txt 
      while ((c = fgetc(fp2)) != EOF) 
      fputc(c, fp3); 
      printf("Merged %s and %s into %s", argv[1], argv[2], file_out); 
      
      fclose(fp1); 
      fclose(fp2); 
      fclose(fp3); 
      return 0; 
   }
} 