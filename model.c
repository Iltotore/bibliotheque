#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
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

//Promote an user to the upper role
void promoteUser(User* user){
  if(user->role == STUDENT) user->role = TEACHER ;
  else if(user->role == TEACHER) user->role = ADMINISTRATOR;
}

//Get the book with the given id from the given Library.
Book* getBook(Library library, int id) {
  for(int i=0;i<library.bookCount ;i++){
    if (id == library.books[i].id) {
      return library.books+i;
    }
  }
  return NULL;
}

//Return the count of remaining borrowing.
int remaining(Library library, User* user) {
  int result = user->role == STUDENT ? 3 : 5;
  for(int i = 0; i < library.bookCount; i++) {
    if(library.books[i].borrower == user) result--;
  }
  return result;
}

//Borrow a book with the given user as borrower
void borrowBook(User* user, Book* book) {
  int duration = user->role == STUDENT ? 2*60 : 3*60;

  book->borrower = user;

  time_t endTime = time(NULL)+duration;
  struct tm* endDate = localtime(&endTime);
  *(book->deliveryDate) = *endDate;
}
