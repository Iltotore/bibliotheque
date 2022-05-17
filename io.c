#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "io.h"
#include "model.h"

void saveBook(Book a, FILE* text){
  char* username;
  if (a.borrower==NULL) username="";
  else username=a.borrower->login;
  fprintf(text,"%s,%s,%d,%d,%s,%ld",a.title, a.author, a.id, a.category, username, mktime(a.deliveryDate));
 }

 Book loadBook(Library lib,FILE* text){
   Book book;
   int categoryId;
   char* username;
   time_t time;

   fscanf(text,"%s,%s,%d,%d,%s,%ld", book.title, book.author, &book.id, &categoryId, username, &time);

   book.category=intToCategory(categoryId);
   book.borrower=getUser(lib, username);
   book.deliveryDate=localtime(&time);

   return book;
}
