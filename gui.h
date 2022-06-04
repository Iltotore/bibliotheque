#include "model.h"

#ifndef gui
#define gui

//Display the given book array with the given length. A specific field can be highlighted.
void showBooks(Book books[], int length, Field focused);

//Ask the user to enter a string (char*). This function allocates one more char to test correct length.
char* askString(char* message, int maxLength);

//Ask the user to select one of the given choices (represented by an int).
int askInt(char* message, char* choices[], int length);

#endif
