#include <stdlib.h>
#include <stdio.h>
#ifndef util
#define util

//Safely allocate the given size. The program exits on failure.
void* safeMalloc(size_t size);

//Return the textual length of the given integer.
int lengthOfInt(int n);

void clear(FILE* stream);
#endif
