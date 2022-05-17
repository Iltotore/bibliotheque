#include <stdbool.h>
#include <time.h>
#ifndef model
#define model

//The possible roles of a user.
typedef enum {
	STUDENT,
	TEACHER
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
	Book* books;
	User* users;
	int numBooks;
	int numUsers;
} Library;

User* getUser(Library lib, char* login);

#endif
