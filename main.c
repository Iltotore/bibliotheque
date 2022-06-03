#include <stdio.h>
#include "model.h"
#include "menu.h"
#include "gui.h"
#include "io.h"

//Program entrypoint
int main() {

  FILE* bookFile = fopen("./books.csv", "r");
  FILE* userFile = fopen("./users.csv", "r");

  if(bookFile == NULL || userFile == NULL) {
    printf("Impossible d'accéder aux fichiers de sauvegarde.\n");
    return -1;
  }

  Library library = loadLibrary(userFile, bookFile);

  fclose(bookFile);
  fclose(userFile);

  User* user;
  char* logChoices[2] = {"Se connecter", "S'inscrire"};

  switch(askInt("Bonjour !", logChoices, 2)) {
    case 0:
    user = loginMenu(library);
    break;
    case 1:
    user = registerMenu(&library);
    break;
  }

  if(user->role == ADMINISTRATOR) adminMainMenu(&library, user);
  else mainMenu(&library, user);

  printf("Au revoir et à bientôt ! Parce que lire c'est grandir !\n");

  bookFile = fopen("./books.csv", "w");
  userFile = fopen("./users.csv", "w");

  if(bookFile == NULL || userFile == NULL) {
    printf("Impossible d'accéder aux fichiers de sauvegarde.\n");
    return -1;
  }

  saveLibrary(library, userFile, bookFile);

  fclose(bookFile);
  fclose(userFile);

  return 0;
}
