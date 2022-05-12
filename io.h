#include <stdio.h>
#ifndef io
#define io

//Save the given Book in the given file.
void saveBook(Book a,FILE* text);

//Load a book from the given file.
Book loadBook(FILE* text);

#endif
