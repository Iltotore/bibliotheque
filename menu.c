#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "model.h"
#include "gui.h"
#include "io.h"
#include "util.h"

#define UPPERS "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWERS "abcdefghijklmnopqrstuvwxyz"
#define DIGITS "0123456789"


//Login sub menu.
User* loginMenu(Library library) {
  int i=0;
  User* user;
  do{
    if(i==3){
      printf("Le nombre maximal d'essais a été atteint!\n");
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

//Register sub menu.
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
      printf("Le mot de passe doit avoir une longueur minimale de 8!\n");
      valid = false;
    }

    if(!strContainsAtLeast(password, UPPERS)) {
      printf("Le mot de passe doit contenir au moins une majuscule!\n");
      valid = false;
    }

    if(!strContainsAtLeast(password, LOWERS)) {
      printf("Le mot de passe doit contenir au moins une minuscule!\n");
      valid = false;
    }

    if(!strContainsAtLeast(password, DIGITS)) {
      printf("Le mot de passe doit contenir au moins un chiffre!\n");
      valid = false;
    }

    if(valid) {
      char* confirm = safeMalloc(sizeof(char)*101);
      printf("Confirmez votre mot de passe:\n");
      scanf("%100s", confirm);
      clear(stdin);
      if(strcmp(password, confirm) != 0) {
        printf("Les deux mots de passe ne correspondent pas!\n");
        valid = false;
      }
    }
  } while(!valid);

  printf("Création...\n");
  User* user = registerUser(library, login, password);
  printf("Votre compte a bien été créé. Bienvenue!\n");

  return user;
}

//Book sort sub menu.
char* sortMenu(Field* field) {
  char* choices[7];
  for(int i=0;i<7;i++){
    choices[i]= fieldToString((Field) i);
  }
  *field=(Field) askInt("Par quel attribut voulez-vous trier les livres ?",choices,7);
  return "Livres triés.";
}

//Change filter sub menu
char* filterMenu(Filter* filter) {
  char* choices[7];
  for(int i=0;i<7;i++){
    choices[i]= fieldToString((Field) i);
  }

  filter->field = (Field) askInt("Par quel attribut voulez-vous filtrer les livres ?",choices,7);
  filter->input = filter->field == NO_FIELD ? "" : askString("Entrez votre filtre", 100);

  return "Le nouveau filtre a bien été appliqué.";
}

//Borrow sub menu.
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

    if(target == NULL) printf("Aucun livre ne correspond à cet identifiant!\n");
  } while(target == NULL);

  borrowBook(user, target);

  return "Livre emprunté.";
}

//Delivery sub menu.
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
    if(target == NULL) printf("Aucun livre ne correspond à cet identifiant!\n");
  } while(target == NULL);

  deliverBook(target);

  if(user->blacklisted) return "Attention: Vous avez été banni automatiquement pour remise en retard. Pour ne plus être banni, contactez votre administrateur.";
  return "Livre rendu.";
}

//Ban sub menu.
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

//Mercy sub menu.
char* mercyMenu(Library library){
  User* target;
  do{
    char* name = askString("Quel utilisateur souhaitez-vous réhabiliter ?", 100);
    target= getUser(library,name);
    if(target==NULL) printf("L'utilisateur sélectionné n'existe pas!\n");
  }while(target==NULL);

  target->blacklisted = false;

  return "L'utilisateur a bien été réabilité.";
}

//Add book sub menu.
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

//Remove book sub menu.
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

//Promote user sub menu.
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

//Main menu (students and teachers).
void mainMenu(Library* library, User* user) {
  char* choices[5]={"Trier les livres", "Filtrer les livres", "Emprunter un livre", "Rendre un livre", "Quitter"};
  int action;
  char* result = NULL;
  Field focused = NO_FIELD;
  Filter filter;
  filter.field = NO_FIELD;
  filter.input = "";
  do{
    system("clear");
    int n;
    Book* books = filterBooks(library->books, library->bookCount, filter, &n);
    showBooks(sortBooks(books, n, focused), n, focused);
    printf("> %s\n", result == NULL ? "Vous ai-je déjà lu quelque part ?" : result);
    if(filter.field == NO_FIELD) printf("Filtre: Aucun\n");
    else printf("Filtre: %s, \"%s\"\n", fieldToString(filter.field), filter.input);
    action = askInt("Sélectionnez une action", choices, 5);
    switch (action) {
      case 0:
       result = sortMenu(&focused);
       break;
      case 1:
       result = filterMenu(&filter);
       break;
      case 2:
      result = borrowMenu(*library, user);
      break;
      case 3:
      result = deliverMenu(*library, user);
      break;
      case 4:
      break;
    }
  }while(action != 4);
}

//Main menu (administrators).
void adminMainMenu(Library* library, User* user) {
  char* choices[10]={"Trier les livres", "Filtrer les livres", "Emprunter un livre","Rendre un livre","Bannir un utilisateur","Réabiliter un utilisateur","Ajouter un livre","Supprimer un livre","Promouvoir un utilisateur","Quitter"};
  int action;
  char* result = NULL;
  Field focused = NO_FIELD;
  Filter filter;
  filter.field = NO_FIELD;
  filter.input = "";
  do{
    system("clear");
    int n;
    Book* books = filterBooks(library->books, library->bookCount, filter, &n);
    showBooks(sortBooks(books, n, focused), n, focused);
    printf("> %s\n", result == NULL ? "Vous ai-je déjà lu quelque part ?" : result);
    if(filter.field == NO_FIELD) printf("Filtre: Aucun\n");
    else printf("Filtre: %s, \"%s\"\n", fieldToString(filter.field), filter.input);
    action = askInt("Sélectionnez une action", choices, 10);
    switch (action) {
      case 0:
       result = sortMenu(&focused);
       break;
      case 1:
       result = filterMenu(&filter);
       break;
      case 2:
      result = borrowMenu(*library, user);
      break;
      case 3:
      result = deliverMenu(*library, user);
      break;
      case 4:
      result = banMenu(*library,user->login);
      break;
      case 5:
      result = mercyMenu(*library);
      break;
      case 6:
      result = addMenu(library);
      break;
      case 7:
      result = removeMenu(library);
      break;
      case 8:
      result = promoteMenu(*library, user->login);
      break;
      case 9:
      break;
    }
  }while(action != 9);
}
