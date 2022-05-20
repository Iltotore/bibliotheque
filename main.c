#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "gui.h"
#include "io.h"
#include "util.h"

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
  User* user;
  int i=0;
  do{
    if(i==3){
      printf("Le nombre maximal d'essais a été atteint.\n");
      return 0;
    }
    char* login = safeMalloc(sizeof(char)*100);
    char* password =safeMalloc(sizeof(char)*100);
    printf("Rentrez votre nom d'utilisateur : \n");
    scanf("%100s",login);
    clear(stdin);
    printf("Rentrez votre mot de passe : \n");
    scanf("%100s",password);
    clear(stdin);
    user = authenticateUser(library,login,password);
    if (user == NULL) printf("Vos identifiants sont incorrects, veuillez réessayer.\n");
    i++;
  }while( user == NULL);
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
