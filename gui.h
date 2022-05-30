#include "model.h"

#ifndef gui
#define gui

//Return a string representation of the given Category.
char* categoryToString(Category category);

//Display the given book array with the given length. A specific field can be highlighted.
void showBooks(Book books[], int length, Field focused);

//Ask the user to enter a string (char*).
char* askString(char* message, int maxLength);

//Ask the user to select one of the given choices (represented by an int).
int askInt(char* message, char* choices[], int length);

#endif
