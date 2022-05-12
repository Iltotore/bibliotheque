#include "model.h"

#ifndef gui
#define gui

char* categoryToString(Category category);
void showBooks(Book books[], int length, Field focused);
int askInt(char* message, char* choices[], int length);

#endif
