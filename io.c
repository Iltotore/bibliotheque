#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "model.h"

void saveBook(Book a,FILE* text){
  char* username;
  if (a.borrower ==NULL) username="";
  else username=a.borrower->login;
  fprintf(text,"%s,%s,%d,%d,%s",a.title, a.author, a.id, a.category, username); 
 }
 
 Book loadBook(FILE* text){
   Book book;
   int categoryId;
   char* username;
   
   
   fscanf(text,"%s,%s,%d,%d,%s",book.title,book.author,&book.id,&categoryId,username);
   
   book.category=intToCategory(categoryId);
   book.borrower=getUser(username);
   
   return book;
 }
