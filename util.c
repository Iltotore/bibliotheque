#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "string.h"

//Safely allocate the given size. The program exits on failure.
void* safeMalloc(size_t size) {
  void* pointer = malloc(size);
  if(pointer == NULL) {
    printf("Memory allocation error.\n");
    exit(-1);
  }
  return pointer;
}

//Return the textual length of the given integer.
int lengthOfInt(int n) {
  int i = 0;
  if(n < 0) i++;
  while(n != 0) {
    i++;
    n /= 10;
  }
  return i;
}

//Clear/Flush the given stream.
void clear(FILE* stream){
  int nh;
  do{
    nh=fgetc(stream);
  }while(nh != EOF && nh != '\n');
}

//Renvoie true si la chaine de caractère `str` contient la chaine `part`
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

//Renvoie true si la chaine de caractère `str` contient au moins un élément de la chaine `chars`
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
