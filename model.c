#include "model.h"
#include <stdlib.h>

User* getUser(Library lib,char* login){
  int i;
  for(i=0;i<lib.numUsers ;i++){
    if (login == lib.users[i].login) return lib.users+i;
  }
  return NULL;
}
