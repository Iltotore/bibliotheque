#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "model.h"
#include "gui.h"

int main() {

  srand(time(NULL));

  Book dummy;
  dummy.title = "The Scala Cookbook";
  dummy.author = "Alvin Alexander";
  dummy.id = 0;
  dummy.category = Sciences;
  dummy.borrower = NULL;

  Book books[5];
  for(int i = 0; i < 5; i++) {
    books[i] = dummy;
    books[i].id = rand();
  }

  showBooks(books, 5);
  return 0;
}
