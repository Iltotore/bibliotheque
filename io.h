#include <stdio.h>
#include "model.h"
#ifndef io
#define io

//Save the given Book in the given file.
void saveBook(Book book, FILE* file);

//Load a book from the given file using the passed library.
Book loadBook(Library library, FILE* file);

//Save the given user in the given file.
void saveUser(User user, FILE* file);

//Load a user form the given file.
User loadUser(FILE* file);

#endif
