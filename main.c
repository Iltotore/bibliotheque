#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "model.h"
#include "gui.h"
#include "util.h"
#include "io.h"

int main() {

  srand(time(NULL));

  FILE* bookFile = fopen("./books.csv", "r");
  FILE* userFile = fopen("./users.csv", "r");

  //TODO Load Library

  if(bookFile == NULL || userFile == NULL) {
    printf("Impossible d'accéder aux fichiers de sauvegarde.\n");
    return -1;
  }

  Library library;

  int bookCount, userCount;

  fscanf(userFile, "%d\n", &userCount);
  printf("Chargement des utilisateurs (%d)...\n", userCount);

  library.userCount = userCount;
  library.users = safeMalloc(sizeof(User)*userCount);
  for(int i = 0; i < userCount; i++) library.users[i] = loadUser(userFile);

  fclose(userFile);

  fscanf(bookFile, "%d\n", &bookCount);
  printf("Chargement des livres (%d)...\n", bookCount);

  library.bookCount = bookCount;
  library.books = safeMalloc(sizeof(Book)*bookCount);
  for(int i = 0; i < bookCount; i++) library.books[i] = loadBook(library, bookFile);

  fclose(bookFile);

  showBooks(library.books, bookCount, NO_FIELD);

  return 0;
}
