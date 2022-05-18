#include <stdlib.h>
#include <stdio.h>
#include "util.h"

void* safeMalloc(size_t size) {
  void* pointer = malloc(size);
  if(pointer == NULL) {
    printf("Memory allocation error.\n");
    exit(-1);
  }
  return pointer;
}

int lengthOfInt(int n) {
  int i = 0;
  if(n < 0) i++;
  while(n != 0) {
    i++;
    n /= 10;
  }
  return i;
}

void clear(FILE* stream){
  int nh;
  do{
    nh=fgetc(stream);
  }while(nh != EOF && nh != '\n');
}
