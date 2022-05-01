#include <stdbool.h>
#ifndef model
#define model

typedef enum {
	Student,
	Teacher
} Role;

typedef struct {
	char* login;
	char* password;
	Role role;
	bool blacklisted;
} User;

typedef enum {
	Sciences,
	Literature,
	Fiction,
	GeneralKnowledge
} Category;

typedef struct {
	char* title;
	char* author;
	int id;
	Category category;
	User* borrower;
} Book;

#endif
