#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"
#include "gui.h"
#include "io.h"
#include "util.h"

#define UPPERS "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWERS "abcdefghijklmnopqrstuvwxyz"
#define DIGITS "123456789"


//Login sub menu
User* loginMenu(Library library) {
  int i=0;
  User* user;
  do{
    if(i==3){
      printf("Le nombre maximal d'essais a été atteint.\n");
      exit(0);
    }
    char* login = askString("Saisissez votre nom d'utilisateur:", 100);
    char* password = askString("Saisissez votre mot de passe:", 100);
    user = authenticateUser(library,login,password);
    if (user == NULL) printf("Vos identifiants sont incorrects, veuillez réessayer.\n");
    i++;
  }while( user == NULL);

  return user;
}

//Register sub menu
User* registerMenu(Library* library) {
  User* existing = NULL;
  char* login;
  char* password;
  do {
    login = askString("Saisissez votre nom d'utilisateur:", 100);
    existing = getUser(*library, login);
    if(existing != NULL) printf("Ce nom est déjà pris.\n");
  } while(existing != NULL);

  bool valid = false;
  do {
    valid = true;
    password = askString("Saisissez votre mot de passe:", 100);
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
      clear(stdin);
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

char* sortMenu(Field* field) {
  char* choices[7];
  for(int i=0;i<7;i++){
    choices[i]= fieldToString((Field) i);
  }
  *field=(Field) askInt("Par quel attribut voulez-vous trier les livres ?",choices,7);
  return "Livres triés.";
}

char* borrowMenu(Library library, User* user) {
  if(remaining(library, user) == 0) return "Vous ne pouvez pas emprunter plus de livre !";

  if(user->blacklisted) return "Vous n'avez pas le droit d'emprunter de livre. Si il s'agit d'une erreur, contactez votre administrateur";

  Book* target;
  do {
    int id;
    printf("Entrez l'ID du livre à emprunter.\n");
    scanf("%d", &id);
    clear(stdin);
    target = getBook(library, id);

    if(target == NULL) printf("Aucun livre ne correspond à cet identifiant.\n");
  } while(target == NULL);

  borrowBook(user, target);

  return "Livre emprunté.";
}

char* deliverMenu(Library library, User* user) {
  Book* target;

  int length;
  Book* borrowed = borrowedBooks(library, user, &length);

  if(length == 0) {
    return "Vous n'avez emprunté aucun livre.";
  }

  do {
    showBooks(borrowed, length, NO_FIELD);

    int id;
    printf("Entrez l'ID du livre à rendre.\n");
    scanf("%d", &id);
    clear(stdin);

    target = getBook(library, id);

    if(target != NULL && target->borrower != user) target = NULL;
    if(target == NULL) printf("Aucun livre ne correspond à cet identifiant.\n");
  } while(target == NULL);

  deliverBook(target);

  if(user->blacklisted) return "Attention: Vous avez été banni automatiquement pour remise en retard. Pour ne plus être banni, contactez votre administrateur.";
  return "Livre rendu.";
}

char* banMenu(Library library,char* current){
  User* target;
  char* name;
  do{
    name = askString("Quel utilisateur souhaitez-vous bannir ?", 100);
    target = getUser(library,name);
    if(target==NULL) printf("L'utilisateur sélectionné n'existe pas.\n");
    if (strcmp(name, current) == 0) printf("Vous ne pouvez pas vous bannir vous-même !\n");
  }while(target==NULL || strcmp(name, current) == 0);

  target->blacklisted = true;

  return "L'utilisateur a bien été banni.";
}

char* mercyMenu(Library library){
  User* target;
  do{
    char* name = askString("Quel utilisateur souhaitez-vous réhabiliter ?", 100);
    target= getUser(library,name);
    if(target==NULL) printf("L'utilisateur sélectionné n'existe pas.\n");
  }while(target==NULL);

  target->blacklisted = false;

  return "L'utilisateur a bien été réabilité.";
}

char* addMenu(Library* library){
  Book book;
  book.borrower=NULL;
  book.deliveryDate=safeMalloc(sizeof(struct tm));
  time_t t = 0;
  *(book.deliveryDate) = *(localtime(&t));
  book.title = askString("Quel est le titre de votre livre ?", 100);
  book.author = askString("Qui est l'auteur de votre livre ?", 100);
  bool valid;

  do {
    printf("Quel est l'identifiant de votre livre ?\n");
    scanf("%d",&book.id);
    clear(stdin);
    valid=getBook(*library, book.id)==NULL;
    if(!valid) printf("L'identifiant est déjà utilisé !\n");
  } while(!valid);

  char* choices[4];
  for(int i=0;i<4;i++){
    choices[i]= categoryToString((Category) i);
  }
  book.category=(Category) askInt("Quelle est la catégorie de votre livre ?",choices,4);
  addBook(library,book);
  return "Votre livre à bien été ajouté. Félicitations !";
}

char* removeMenu(Library* library){
  if(library->bookCount==0) return "Votre bibliothèque ne contient aucun livre !";
  int id;
  bool valid;
  do {
    printf("Entrez l'ID du livre à supprimer.\n");
    scanf("%d", &id);
    clear(stdin);
    valid = getBook(*library, id) != NULL;
    if(!valid) printf("Aucun livre ne correspond à cet identifiant !\n");
  } while(!valid);
  removeBook(library,id);
  return "Le livre a bien été supprimé. Félicitations !";
}

char* promoteMenu(Library library, char* current) {
  User* target;
  char* name;
  do{
    name = askString("Quel utilisateur souhaitez-vous promouvoir ?", 100);
    target= getUser(library,name);
    if(target==NULL) printf("L'utilisateur sélectionné n'existe pas.\n");
    if (strcmp(name, current) == 0) printf("Vous ne pouvez pas vous promouvoir vous-même.\n");
  }while(target==NULL || strcmp(name, current) == 0);

  if(target->role == ADMINISTRATOR) return "L'utilisateur est déjà un administrateur";
  promoteUser(target);
  return "L'utilisateur a bien été promu";
}

void mainMenu(Library* library, User* user) {
  char* choices[4]={"Trier les livres", "Emprunter un livre", "Rendre un livre", "Quitter"};
  int action;
  char* result = NULL;
  Field focused = NO_FIELD;
  do{
    system("clear");
    showBooks(sortBooks(library->books, library->bookCount, focused), library->bookCount, focused);
    if(result != NULL) printf("> %s\n", result);
    action = askInt("Sélectionnez une action", choices, 4);
    switch (action) {
      case 0:
       result = sortMenu(&focused);
       break;
      case 1:
      result = borrowMenu(*library, user);
      break;
      case 2:
      result = deliverMenu(*library, user);
      break;
      case 3:
      break;
    }
  }while(action != 3);
}

void adminMainMenu(Library* library, User* user) {
  char* choices[9]={"Trier les livres", "Emprunter un livre","Rendre un livre","Bannir un utilisateur","Réabiliter un utilisateur","Ajouter un livre","Supprimer un livre","Promouvoir un utilisateur","Quitter"};
  int action;
  char* result = NULL;
  Field focused = NO_FIELD;
  do{
    system("clear");
    showBooks(sortBooks(library->books, library->bookCount, focused), library->bookCount, focused);
    if(result != NULL) printf("> %s\n", result);
    action = askInt("Sélectionnez une action", choices, 9);
    switch (action) {
      case 0:
       result = sortMenu(&focused);
       break;
      case 1:
      result = borrowMenu(*library, user);
      break;
      case 2:
      result = deliverMenu(*library, user);
      break;
      case 3:
      result = banMenu(*library,user->login);
      break;
      case 4:
      result = mercyMenu(*library);
      break;
      case 5:
      result = addMenu(library);
      break;
      case 6:
      result = removeMenu(library);
      break;
      case 7:
      result = promoteMenu(*library, user->login);
      break;
      case 8:
      break;
    }
  }while(action != 8);
}

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
