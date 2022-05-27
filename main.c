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

//Register sub menu
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

void borrowMenu(Library library, User* user) {
  if(remaining(library, user) == 0) {
    printf("Vous ne pouvez pas emprunter plus de livre !\n");
    return;
  }

  if(user->blacklisted) {
    printf("Vous n'avez pas le droit d'emprunter de livre. Si il s'agit d'une erreur, contactez votre administrateur\n");
    return;
  }

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

  printf("Livre emprunté.\n");
}

void deliverMenu(Library library, User* user) {
  Book* target;

  int length;
  Book* borrowed = borrowedBooks(library, user, &length);

  if(length == 0) {
    printf("Vous n'avez emprunté aucun livre.\n");
    return;
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

  printf("Livre rendu.\n");
  if(user->blacklisted) printf("Attention: Vous avez été banni automatiquement pour remise en retard. Pour ne plus être banni, contactez votre administrateur.\n");
}

void banMenu(Library library,char* current){
  User* target;
  char* name=safeMalloc(sizeof(char)*101);
  do{
    printf("Quel utilisateur souhaitez vous bannir ?\n");
    scanf("%100s",name);
    clear(stdin);
    target= getUser(library,name);
    if(target==NULL) printf("L'utilisateur sélectionné n'existe pas.\n");
    if (strcmp(name, current) == 0) printf("Vous ne pouvez pas vous bannir vous-même !\n");
  }while(target==NULL || strcmp(name, current) == 0);

  target->blacklisted = true;

  printf("L'utilisateur a bien été banni.\n");
}

void mercyMenu(Library library){
  User* target;
  char* name=safeMalloc(sizeof(char)*101);
  do{
    printf("Quel utilisateur souhaitez vous réabiliter ?\n");
    scanf("%100s",name);
    clear(stdin);
    target= getUser(library,name);
    if(target==NULL) printf("L'utilisateur sélectionné n'existe pas.\n");
  }while(target==NULL);

  target->blacklisted = false;

  printf("L'utilisateur a bien été réabilité.\n");
}

void addMenu(Library* library){
  Book book;
  book.title =safeMalloc(sizeof(char)*101);
  book.author=safeMalloc(sizeof(char)*101);
  book.borrower=NULL;
  book.deliveryDate=safeMalloc(sizeof(struct tm));
  time_t t = 0;
  *(book.deliveryDate) = *(localtime(&t));
  printf("Quel est le titre de votre livre ?\n");
  scanf("%100[^\n]",book.title);
  clear(stdin);
  printf("Quel est le l'auteur de votre livre ?\n");
  scanf("%100[^\n]",book.author);
  clear(stdin);
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
  printf("Votre livre à bien été ajouté. Félicitations !\n");
}

void removeMenu(Library* library){
  if(library->bookCount==0){
    printf("Votre bibliothèque ne contient aucun livre !\n");
    return;
  }
  int id;
  bool valid;
  do {
    printf("Entrez l'ID du livre à supprimer.\n");
    scanf("%d", &id);
    clear(stdin);
    valid =getBook(*library, id) !=NULL;
    if(!valid) printf("Aucun livre ne correspond à cet identifiant !\n");
  } while(!valid);
  removeBook(library,id);
  printf("Le livre a bien été supprimé. Félicitations !\n");
}


void promoteMenu(Library library, char* current) {
 User* target;
 char* name=safeMalloc(sizeof(char)*101);
 do{
   printf("Quel utilisateur souhaitez vous promouvoir ?\n");
   scanf("%100s",name);
   clear(stdin);
   target= getUser(library,name);
   if(target==NULL) printf("L'utilisateur sélectionné n'existe pas.\n");
   if (strcmp(name, current) == 0) printf("Vous ne pouvez pas vous promouvoir vous-même.\n");
 }while(target==NULL || strcmp(name, current) == 0);

 if(target->role == ADMINISTRATOR) printf("L'utilisateur est déjà un administrateur\n");
 else {
   promoteUser(target);
   printf("L'utilisateur a bien été promu\n");
 }
}

void mainMenu(Library* library, User* user) {
  char* choices[3]={"Emprunter un livre", "Rendre un livre", "Quitter"};
  int action;
  do{
    system("clear");
    showBooks(library->books, library->bookCount, NO_FIELD);
    action = askInt("Sélectionnez une action", choices, 3);
    switch (action) {
      case 0:
       borrowMenu(*library, user);
       break;
      case 1:
       deliverMenu(*library, user);
       break;
      case 2:
       break;
    }
  }while(action != 2);
}

void adminMainMenu(Library* library, User* user) {
  char* choices[8]={"Emprunter un livre","Rendre un livre","Bannir un utilisateur","Réabiliter un utilisateur","Ajouter un livre","Supprimer un livre","Promouvoir un utilisateur","Quitter"};
  int action;
  do{
    system("clear");
    showBooks(library->books, library->bookCount, NO_FIELD);
    action = askInt("Sélectionnez une action", choices, 8);
    switch (action) {
      case 0:
       borrowMenu(*library, user);
       break;
      case 1:
       deliverMenu(*library, user);
       break;
      case 2:
       banMenu(*library,user->login);
       break;
      case 3:
       mercyMenu(*library);
       break;
      case 4:
       addMenu(library);
       break;
      case 5:
       removeMenu(library);
       break;
      case 6:
       promoteMenu(*library, user->login);
       break;
      case 7:
       break;
    }
  }while(action != 7);
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
