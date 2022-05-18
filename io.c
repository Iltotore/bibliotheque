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
