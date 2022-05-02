#include <stdbool.h>
#ifndef model
#define model

typedef enum {
	STUDENT,
	TEACHER
} Role;

typedef struct {
	char* login;
	char* password;
	Role role;
	bool blacklisted;
} User;

typedef enum {
	SCIENCES,
	LITERATURE,
	FICTION,
	GENERAL_KNOWLEDGE
} Category;

typedef struct {
	char* title;
	char* author;
	int id;
	Category category;
	User* borrower;
} Book;

#endif
