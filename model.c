#include <stdlib.h>
#include <string.h>
#include "model.h"

User* getUser(Library lib,char* login){
  int i;
  for(i=0;i<lib.userCount ;i++){
    if (strcmp(login, lib.users[i].login) == 0) {
      return lib.users+i;
    }
  }
  return NULL;
}
