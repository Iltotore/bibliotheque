#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"
#include "gui.h"
#include "io.h"
#include "util.h"

#define UPPERS "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWERS "abcdefghijklmnopqrstuvwxyz"
#define DIGITS "0123456789"

User* loginMenu(Library library) {
  int i=0;
  User* user;
  do{
    if(i==3){
      printf("Le nombre maximal d'essais a été atteint.\n");
      return 0;
    }
    char* login = safeMalloc(sizeof(char)*100);
    char* password =safeMalloc(sizeof(char)*100);
    printf("Saisissez votre nom d'utilisateur :\n");
    scanf("%100s",login);
    clear(stdin);
    printf("Saisissez votre mot de passe :\n");
    scanf("%100s",password);
    clear(stdin);
    user = authenticateUser(library,login,password);
    if (user == NULL) printf("Vos identifiants sont incorrects, veuillez réessayer.\n");
    i++;
  }while( user == NULL);

  return user;
}

User* registerMenu(Library* library) {
  User* existing = NULL;
  char* login = safeMalloc(sizeof(char)*101);
  do {
    printf("Saisissez votre nom d'utilisateur :\n");
    scanf("%100s", login);
    clear(stdin);
    existing = getUser(*library, login);
    if(existing != NULL) printf("Ce nom est déjà pris.\n");
  } while(existing != NULL);

  char* password = safeMalloc(sizeof(char)*101);
  bool valid = false;
  do {
    valid = true;
    printf("Saisissez votre mot de passe:\n");
    scanf("%100s", password);
    clear(stdin);
    if(strlen(password) < 8) {
      printf("Le mot de passe doit avoir une longueur minimale de 8.\n");
      valid = false;
    }

    if(!strContainsAtLeast(password, UPPERS)) {
      printf("Le mot de passe doit contenir au moins une majuscule.\n");
      valid = false;
    }

    if(!strContainsAtLeast(password, LOWERS)) {
      printf("Le mot de passe doit contenir au moins une minuscule.\n");
      valid = false;
    }

    if(!strContainsAtLeast(password, DIGITS)) {
      printf("Le mot de passe doit contenir au moins un chiffre.\n");
      valid = false;
    }

    if(valid) {
      char* confirm = safeMalloc(sizeof(char)*101);
      printf("Confirmez votre mot de passe:\n");
      scanf("%100s", confirm);
      printf("Scanned\n");
      clear(stdin);
      printf("Cleared\n");
      if(strcmp(password, confirm) != 0) {
        printf("Les deux mots de passe ne correspondent pas.\n");
        valid = false;
      }
    }
  } while(!valid);

  printf("Création...\n");
  User* user = registerUser(library, login, password);
  printf("Votre compte a bien été créé.\n");

  return user;
}

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

  char* logChoices[2] = {"Se connecter", "S'inscrire"};

  switch(askInt("Bonjour !", logChoices, 2)) {
    case 0:
      user = loginMenu(library);
      break;
    case 1:
      user = registerMenu(&library);
      break;
  }

  char* choices[2]={"Les livres de la bibliothèque","Quitter"};
  int action;
  do{
    action = askInt("Sélectionnez une action", choices, 2);
    switch (action) {
      case 0:
        showBooks(library.books, library.bookCount, NO_FIELD);
        break;
      case 1:
        break;
    }
  }while(action != 1);
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
