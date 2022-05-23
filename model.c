#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "model.h"
#include "util.h"

//Get the User with the given name from the given Library.
User* getUser(Library lib,char* login){
  for(int i=0;i<lib.userCount ;i++){
    if (strcmp(login, lib.users[i].login) == 0) {
      return lib.users+i;
    }
  }
  return NULL;
}

//Return the User corresponding to the given credentials. Return NULL if credentials don't match.
User* authenticateUser(Library library, char* login, char* password) {
  User* user = getUser(library, login);
  if(user == NULL) return NULL;
  else if(strcmp(password, user->password) == 0) return user;
  else return NULL;
}

//Register and return a new User into the passed Library using the given credentials.
User* registerUser(Library* library, char* login, char* password) {
  int newCount = library->userCount+1;
  User* users = safeMalloc(sizeof(User)*newCount);
  for(int i = 0; i < newCount; i++) users[i] = library->users[i];

  User registered;
  registered.login = login;
  registered.password = password;
  registered.role = STUDENT;
  registered.blacklisted = false;

  users[newCount-1] = registered;

  library->users = users;
  library->userCount = newCount;

  return library->users+newCount-1;
}
