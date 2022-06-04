#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "model.h"
#include "gui.h"
#include "util.h"

#define FOCUS_STYLE "\x1b[1m"
#define FOCUS_RESET "\x1b[0m"

//Extend a string using the given char sequence `str`.
char* completeWithString(char* base, int length, char* str) {
  int currentLength = strlen(base);
  if(currentLength > length) return base;
  char* result = safeMalloc(sizeof(char)*(length+1));
  strcpy(result, "");
  strcat(result, base);

  int step = strlen(str);
  for(int i = 0; i < length-currentLength; i+=step) strcat(result, str);
  return result;
}

//Highlight the given `str` if `focused` is true.
char* focus(char* str, bool focused) {
  if(focused) {
    char* result = safeMalloc(sizeof(char)*(strlen(str)+strlen(FOCUS_STYLE)+strlen(FOCUS_RESET)+1));
    strcpy(result, "");
    strcat(result, FOCUS_STYLE);
    strcat(result, str);
    strcat(result, FOCUS_RESET);
    return result;
  } else return str;
}

//Display the given book array with the given length. A specific field can be highlighted.
void showBooks(Book books[], int length, Field focused) {
  int max[6] = {5, 6, 2, 9, 12, 14};
  for(int i = 0; i < length; i++) {
    Book book = books[i];
    int length[6] = {strlen(book.title), strlen(book.author), lengthOfInt(book.id), strlen(categoryToString(book.category)), book.borrower == NULL ? 0 : strlen(book.borrower->login), 14};
    for(int i = 0; i < 6; i++) {
      if(max[i] < length[i]) max[i] = length[i];
    }
  }

  int totalLength = max[0] + max[1] + max[2] + max[3] + max[4] + max[5] + 6;

  printf("%s\n", completeWithString("", totalLength+1, "-"));

  printf(
    "|%s|%s|%s|%s|%s|%s|\n",
    focus(completeWithString("Titre", max[0], " "), focused == TITLE),
    focus(completeWithString("Auteur", max[1], " "), focused == AUTHOR),
    focus(completeWithString("ID", max[2], " "), focused == ID),
    focus(completeWithString("Categorie", max[3], " "), focused == CATEGORY),
    focus(completeWithString("Emprunte par", max[4], " "), focused == BORROWER),
    focus(completeWithString("Date de remise", max[5], " "), focused == DELIVERY_DATE)
  );

  printf("%s\n", completeWithString("", totalLength+1, "-"));

  for(int i = 0; i < length; i++) {
    Book book = books[i];
    char* bookId = safeMalloc(sizeof(char)*lengthOfInt(book.id));
    sprintf(bookId, "%d", book.id);

    char formattedTime[15];
    strftime(formattedTime, 15, "%D %R", book.deliveryDate);

    printf(
      "|%s|%s|%s|%s|%s|%s|\n",
      focus(completeWithString(book.title, max[0], " "), focused == TITLE),
      focus(completeWithString(book.author, max[1], " "), focused == AUTHOR),
      focus(completeWithString(bookId, max[2], " "), focused == ID),
      focus(completeWithString(categoryToString(book.category), max[3], " "), focused == CATEGORY),
      focus(completeWithString(book.borrower == NULL ? "" : book.borrower->login, max[4], " "), focused == BORROWER),
      focus(completeWithString(book.borrower == NULL ? "" : formattedTime, max[5], " "), focused == DELIVERY_DATE)
    );
  }

  printf("%s\n", completeWithString("", totalLength+1, "-"));
}

//Ask the user to enter a string (char*). This function allocates one more char to test correct length.
char* askString(char* message, int maxLength) {
  char* format = safeMalloc(sizeof(char)*(5 + lengthOfInt(maxLength+1)));

  char* answer = safeMalloc(sizeof(char)*(maxLength+2));
  do {
    printf("%s\n", message);
    sprintf(format, "%%%d[^\n]", maxLength+1);
    scanf(format, answer);
    clear(stdin);

    if(strlen(answer) > maxLength) printf("Votre réponse est trop longue (max: %d).\n", maxLength);
  } while(strlen(answer) > maxLength);
  return answer;
}

//Ask the user to select one of the given choices (represented by an int).
int askInt(char* message, char* choices[], int length) {
  printf("%s\n", message);
  for(int i = 0; i < length; i++) printf("[%d]: %s\n", i+1, choices[i]);
  int answer;
  scanf("%d", &answer);
  clear(stdin);
  while (answer < 1 || answer > length) {
    printf("Veuillez saisir un nombre entre 1 et %d\n", length);
    scanf("%d", &answer);
    clear(stdin);
  }
  return answer-1;
}
