#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "ARGC < 2\n");
  }
  FILE *sfile = fopen(argv[1], "r");
  char buffer[1024];
  while (fgets(buffer, 1024, sfile)) {
    printf("%s",buffer);
    char c;
    while (fgetc(buffer) != EOF) {
      c = fgetc(buffer);
    }
  
  }

  fclose(sfile);
  return 0;
}
