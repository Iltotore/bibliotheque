#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "io.h"
#include "model.h"
#include "util.h"

void saveBook(Book book, FILE* file){
  fprintf(file,"%s,%s,%d,%d,%s,%ld\n",book.title, book.author, book.id, book.category, book.borrower==NULL ? " " : book.borrower->login, mktime(book.deliveryDate));
 }

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
  book.deliveryDate=localtime(&time);

  return book;
}

void saveUser(User user, FILE* file) {
  fprintf(file, "%s,%s,%d,%d\n", user.login, user.password, user.role, user.blacklisted);
}

User loadUser(FILE* file) {
  User user;
  int roleId;
  int blacklisted;

  user.login = safeMalloc(sizeof(char)*101);
  user.password = safeMalloc(sizeof(char)*101);

  fscanf(file, "%100[^,],%100[^,],%d,%d\n", user.login, user.password, &roleId, &blacklisted);
  user.role = (Role) roleId;
  user.blacklisted = (bool) blacklisted;

  return user;
}

void saveLibrary(Library library, FILE* userFile, FILE* bookFile) {
  fprintf(userFile, "%d\n", library.userCount);
  for(int i = 0; i < library.userCount; i++) saveUser(library.users[i], userFile);

  fprintf(bookFile, "%d\n", library.bookCount);
  for(int i = 0; i < library.bookCount; i++) saveBook(library.books[i], bookFile);
}

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
