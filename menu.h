#include "model.h"

#ifndef menu
#define menu

//Login sub menu.
User* loginMenu(Library library);

//Register sub menu.
User* registerMenu(Library* library);

//Book sort sub menu.
char* sortMenu(Field* field);

//Borrow sub menu.
char* borrowMenu(Library library, User* user);

//Delivery sub menu.
char* deliverMenu(Library library, User* user);

//Ban sub menu.
char* banMenu(Library library,char* current);

//Mercy sub menu.
char* mercyMenu(Library library);

//Add book sub menu.
char* addMenu(Library* library);

//Remove book sub menu.
char* removeMenu(Library* library);

//Promote user sub menu.
char* promoteMenu(Library library, char* current);

//Main menu (students and teachers).
void mainMenu(Library* library, User* user);

//Main menu (administrators).
void adminMainMenu(Library* library, User* user);

#endif
