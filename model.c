#include <stdlib.h>
#include <string.h>
#include "model.h"

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
