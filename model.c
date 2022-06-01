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

//Return the list of books borrowed by the given user.
Book* borrowedBooks(Library library, User* user, int* length) {
  *length = 0;
  for(int i = 0; i < library.bookCount; i++) {
    if(library.books[i].borrower == user) (*length)++;
  }

  Book* borrowed = safeMalloc(sizeof(Book)*(*length));

  int n = 0;

  for(int i = 0; i < library.bookCount; i++) {
    if(library.books[i].borrower == user) {
      borrowed[n] = library.books[i];
      n++;
    }
  }

  return borrowed;
}

//Borrow a book with the given user as borrower
void borrowBook(User* user, Book* book) {
  int duration = user->role == STUDENT ? 2*60 : 3*60;

  book->borrower = user;

  time_t endTime = time(NULL)+duration;
  struct tm* endDate = localtime(&endTime);
  *(book->deliveryDate) = *endDate;
}

//Make the given user deliver a borrowed book.
void deliverBook(Book* book) {
	time_t deliveryTime = mktime(book->deliveryDate);
	if(time(NULL) > deliveryTime) book->borrower->blacklisted = true;

  book->borrower = NULL;

  time_t t = 0;
  *(book->deliveryDate) = *(localtime(&t));

}

//Get the book with the given id from the given Library.
void addBook(Library* library,Book book) {
  int newCount = library->bookCount+1;
  Book* books = safeMalloc(sizeof(Book)*newCount);
  for(int i = 0; i < newCount-1; i++) books[i] = library->books[i];

  books[newCount-1] = book;
  library->books = books;
  library->bookCount = newCount;

}

//Remove a book from the library
void removeBook(Library* library,int id){
  int newCount=library->bookCount-1;
  Book* books=safeMalloc(sizeof(Book)*newCount);
  int n=0;
  for(int i=0;i<library->bookCount;i++){
    Book current=library->books[i];
    if(current.id != id){
      books[n]=current;
      n++;
    }
  }
  library->books = books;
  library->bookCount = newCount;
}

//Compare two ints. Similar to strcmp.
int intcmp(int a, int b) {
  if(a == b) return 0;
  if(a > b) return 1;
  return -1;
}

//Compare two books by a particular field.
int compareBooks(Book a, Book b, Field field) {
  switch (field) {
    case TITLE: return strcmp(a.title, b.title);
    case AUTHOR: return strcmp(a.author, b.author);
    case ID: return intcmp(a.id, b.id);
    case CATEGORY: return strcmp(categoryToString(a.category), categoryToString(b.category));
    case BORROWER:
      if(a.borrower == b.borrower) return 0;
      if(b.borrower == NULL) return 1;
      if(a.borrower == NULL) return -1;
      return strcmp(a.borrower->login, b.borrower->login);
    case DELIVERY_DATE: return intcmp(mktime(a.deliveryDate), mktime(b.deliveryDate));
    default:
      return compareBooks(a, b, TITLE); //Compare by title by default
  }
}

//Place the pivot in sorted list at its correct position.
int partitioning(Book books[], Field field, int begin, int end) {
  int firstGreater = begin;
  int firstLess = end;
  Book tmp;

  while(firstGreater < firstLess) {
    while(compareBooks(books[firstGreater], books[begin], field) <= 0 && firstGreater < end) firstGreater++;
    while(compareBooks(books[firstLess], books[begin], field) > 0) firstLess--;

    if(firstGreater < firstLess) {
      tmp = books[firstGreater];
      books[firstGreater] = books[firstLess];
      books[firstLess] = tmp;
    }
  }

  tmp = books[begin];
  books[begin] = books[firstLess];
  books[firstLess] = tmp;
  return firstLess;
}

//Sort a book list using the quicksort algorithm.
void quickSortBook(Book books[], Field field, int begin, int end) {
  int pivot;
  if(begin < end) {
    pivot = partitioning(books, field, begin, end);
    quickSortBook(books, field, begin, pivot-1);
    quickSortBook(books, field, pivot+1, end);
  }
}

//Sort an array of book by a particular field. Return a new array.
Book* sortBooks(Book* books, int length, Field field) {
  Book* sorted = safeMalloc(sizeof(Book)*length);
  for(int i = 0; i < length; i++) sorted[i] = books[i];
  quickSortBook(sorted, field, 0, length-1);
  return sorted;
}

//Return a string representation of the given Category.
char* categoryToString(Category category) {
  switch (category) {
    case SCIENCES: return "Sciences";
    case LITERATURE: return "Literature";
    case FICTION: return "Fiction";
    case GENERAL_KNOWLEDGE: return "Culture Generale";
    default: return "???";
  }
}

//
char* fieldToString(Field field) {
  switch (field) {
    case TITLE: return "Titre";
    case AUTHOR: return "Auteur";
    case ID: return "ID";
    case CATEGORY: return "Catégorie";
    case BORROWER: return "Emprunté par";
    case DELIVERY_DATE: return "Date de remise";
    default: return "Aucun attribut";
  }
}
