#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "gui.h"
#include "io.h"

int main() {
  srand(time(NULL));

  FILE* bookFile = fopen("./books.csv", "r");
  FILE* userFile = fopen("./users.csv", "r");

  if(bookFile == NULL || userFile == NULL) {
    printf("Impossible d'accéder aux fichiers de sauvegarde.\n");
    return -1;
  }

  Library library = loadLibrary(userFile, bookFile);

  fclose(bookFile);
  fclose(userFile);

  showBooks(library.books, library.bookCount, NO_FIELD);

  bookFile = fopen("./books.csv", "r");
  userFile = fopen("./users.csv", "r");

  if(bookFile == NULL || userFile == NULL) {
    printf("Impossible d'accéder aux fichiers de sauvegarde.\n");
    return -1;
  }

  saveLibrary(library, userFile, bookFile);

  fclose(bookFile);
  fclose(userFile);

  return 0;
}
