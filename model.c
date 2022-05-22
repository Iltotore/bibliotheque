#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "model.h"
#include "util.h"

User* getUser(Library lib,char* login){
  for(int i=0;i<lib.userCount ;i++){
    if (strcmp(login, lib.users[i].login) == 0) {
      return lib.users+i;
    }
  }
  return NULL;
}

User* authenticateUser(Library library, char* login, char* password) {
  User* user = getUser(library, login);
  if(user == NULL) return NULL;
  else if(strcmp(password, user->password) == 0) return user;
  else return NULL;
}

User* registerUser(Library* library, char* login, char* password) {
  User* users = safeMalloc(sizeof(User)*(library->userCount+1));
  for(int i = 0; i < library->userCount; i++) users[i] = library->users[i];

  User registered;
  registered.login = login;
  registered.password = password;
  registered.role = STUDENT;
  registered.blacklisted = false;

  users[library->userCount+1] = registered;

  (*library).users = users;
  (*library).userCount++;
}
