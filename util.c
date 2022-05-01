#include <stdlib.h>

void* safeMalloc(size_t size) {
  void* pointer = malloc(size);
  if(pointer == NULL) {
    printf("Memory allocation error.\n");
    exit(-1);
  }
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
