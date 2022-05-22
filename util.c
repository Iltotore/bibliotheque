#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "string.h"

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

bool strcontains(char* str, char* part) {
  int strLength = strlen(str);
  int partLength = strlen(part);
  int matching = 0;

  for(int i = 0; i < strLength - partLength || matching == partLength; i++) {
    if(str[i] == part[matching]) matching++;
    else matching = 0;
  }

  return matching == partLength;
}

bool strContainsAtLeast(char* str, char* chars) {
  int strLength = strlen(str);
  int charCount = strlen(chars);

  for(int i = 0; i < strLength; i++) {
    for(int j = 0; j < charCount; j++) {
      if(str[i] == chars[j]) return true;
    }
  }

  return false;
}
