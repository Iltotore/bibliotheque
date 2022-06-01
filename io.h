#include <stdio.h>
#include "model.h"
#define KEY "A"
#ifndef io
#define io

//Save the given user in the given file.
void saveUser(User user, FILE* file);

//Load a user from the given file.
User loadUser(FILE* file);

//Save the given Book in the given file.
void saveBook(Book book, FILE* file);

//Load a book from the given file using the passed library.
Book loadBook(Library library, FILE* file);

//Save the given library.
void saveLibrary(Library library, FILE* userFile, FILE* bookFile);

//Load a new library from the given user file and book file.
Library loadLibrary(FILE* userFile, FILE* bookFile);

#endif
