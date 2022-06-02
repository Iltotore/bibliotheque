#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "io.h"
#include "model.h"
#include "util.h"
#include "cypher.h"

//Save the given user in the given file.
void saveUser(User user, FILE* file) {
  fprintf(file, "%s,%s,%d,%d\n", user.login, encrypt(user.password,KEY), user.role, user.blacklisted);
}

//Load a user from the given file.
User loadUser(FILE* file) {
  User user;
  int roleId;
  int blacklisted;

  user.login = safeMalloc(sizeof(char)*101);
  char* tempPassword = safeMalloc(sizeof(char)*101);

  fscanf(file, "%100[^,],%100[^,],%d,%d\n", user.login, tempPassword, &roleId, &blacklisted);
  user.password=decrypt(tempPassword,KEY);
  user.role = (Role) roleId;
  user.blacklisted = (bool) blacklisted;

  return user;
}

//Save the given Book in the given file.
void saveBook(Book book, FILE* file){
  fprintf(file,"%s,%s,%d,%d,%s,%ld\n",book.title, book.author, book.id, book.category, book.borrower==NULL ? " " : book.borrower->login, mktime(book.deliveryDate));
}

///Load a book from the given file using the passed library.
Book loadBook(Library lib, FILE* file){
  Book book;
  book.title = safeMalloc(sizeof(char)*101);
  book.author = safeMalloc(sizeof(char)*101);
  char* username = safeMalloc(sizeof(char)*101);
  int categoryId;
  time_t time;

  fscanf(file,"%100[^,],%100[^,],%d,%d,%100[^,],%ld\n", book.title, book.author, &book.id, &categoryId, username, &time);

  book.category = (Category) categoryId;
  book.borrower=getUser(lib, username);
  book.deliveryDate=safeMalloc(sizeof(struct tm));
  *book.deliveryDate=*localtime(&time);

  return book;
}

//Save the given library.
void saveLibrary(Library library, FILE* userFile, FILE* bookFile) {
  fprintf(userFile, "%d\n", library.userCount);
  for(int i = 0; i < library.userCount; i++) saveUser(library.users[i], userFile);

  fprintf(bookFile, "%d\n", library.bookCount);
  for(int i = 0; i < library.bookCount; i++) saveBook(library.books[i], bookFile);
}

//Load a new library from the given user file and book file.
Library loadLibrary(FILE* userFile, FILE* bookFile) {
  Library library;

  int bookCount, userCount;

  fscanf(userFile, "%d\n", &userCount);
  printf("Chargement des utilisateurs (%d)...\n", userCount);

  library.userCount = userCount;
  library.users = safeMalloc(sizeof(User)*userCount);
  for(int i = 0; i < userCount; i++) library.users[i] = loadUser(userFile);

  fscanf(bookFile, "%d\n", &bookCount);
  printf("Chargement des livres (%d)...\n", bookCount);

  library.bookCount = bookCount;
  library.books = safeMalloc(sizeof(Book)*bookCount);
  for(int i = 0; i < bookCount; i++) library.books[i] = loadBook(library, bookFile);

  return library;
}
