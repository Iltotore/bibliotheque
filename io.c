#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "io.h"
#include "model.h"

void saveBook(Book book, FILE* file){
  char* username;
  if (book.borrower==NULL) username="";
  else username=book.borrower->login;
  fprintf(file,"%s,%s,%d,%d,%s,%ld\n",book.title, book.author, book.id, book.category, username, mktime(book.deliveryDate));
 }

Book loadBook(Library lib, FILE* file){
  Book book;
  int categoryId;
  char* username;
  time_t time;

  fscanf(file,"%s,%s,%d,%d,%s,%ld\n", book.title, book.author, &book.id, &categoryId, username, &time);
  book.category=intToCategory(categoryId);
  book.borrower=getUser(lib, username);
  book.deliveryDate=localtime(&time);

  return book;
}

void saveUser(User user, FILE* file) {
  fprintf(file, "%s,%s,%d,%d\n", user.login, user.password, user.role, user.blacklisted);
}
