#include <stdbool.h>
#include <time.h>
#ifndef model
#define model

//The possible roles of a user.
typedef enum {
	STUDENT,
	TEACHER,
	ADMINISTRATOR
} Role;

//A registered user with its login, password, role and whether they are blacklisted.
typedef struct {
	char* login;
	char* password;
	Role role;
	bool blacklisted;
} User;

//The possible book categories.
typedef enum {
	SCIENCES,
	LITERATURE,
	FICTION,
	GENERAL_KNOWLEDGE
} Category;

//A stored book containing a title, an author, an id (must be unique), a category and a borrower (nullable).
typedef struct {
	char* title;
	char* author;
	int id;
	Category category;
	User* borrower;
	struct tm* deliveryDate;
} Book;

//Represent each field of a Book.
typedef enum {
	TITLE,
	AUTHOR,
	ID,
	CATEGORY,
	BORROWER,
	DELIVERY_DATE,
	NO_FIELD
} Field;

typedef struct {
	Field field;
	char* input;
} Filter;

//Represent a Library. A Library contains an array of Users and an array of Book;
typedef struct {
	Book* books;
	User* users;
	int bookCount;
	int userCount;
} Library;

//Get the User with the given name from the given Library.
User* getUser(Library lib, char* login);

//Return the User corresponding to the given credentials. Return NULL if credentials don't match.
User* authenticateUser(Library library, char* login, char* password);

//Register and return a new User into the passed Library using the given credentials.
User* registerUser(Library* library, char* login, char* password);

//Promote a user to the next rank.
void promoteUser(User* user);

//Get the book with the given id from the given Library.
Book* getBook(Library library, int id);

//Return the count of remaining borrowing.
int remaining(Library library, User* user);

//Return the list of books borrowed by the given user.
Book* borrowedBooks(Library library, User* user, int* length);

//Borrow a book with the given user as borrower.
void borrowBook(User* user, Book* book);

//Make the given user deliver a borrowed book.
void deliverBook(Book* book);

//Add a book to the library.
void addBook(Library* library,Book book);

//Remove a book from the library.
void removeBook(Library* library,int id);

//Compare two books by a particular field.
int compareBooks(Book a, Book b, Field field);

//Sort an array of book by a particular field. Return a new array.
Book* sortBooks(Book* books, int length, Field field);

//Filter the given book array. Return a new array.
Book* filterBooks(Book* books, int length, Filter filter, int* resultLength);

//Return the string representation of the given Category.
char* categoryToString(Category category);

//Return the string representation of the given Field.
char* fieldToString(Field field);

#endif
