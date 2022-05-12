#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "model.h"
#include "gui.h"
#include "util.h"

#define FOCUS_STYLE "\x1b[1m"
#define FOCUS_RESET "\x1b[0m"

char* categoryToString(Category category) {
  switch (category) {
    case SCIENCES: return "Sciences";
    case LITERATURE: return "Litérature";
    case FICTION: return "Fiction";
    case GENERAL_KNOWLEDGE: return "Culture Générale";
  }
}

char* completeWithString(char* base, int length, char* str) {
  int currentLength = strlen(base);
  if(currentLength > length) return base;
  char* result = safeMalloc(sizeof(char)*length);
  strcat(result, base);

  int steps = (length-currentLength)/strlen(str);
  for(int i = 0; i < steps; i++) {
    strcat(result, str);
  }
  return result;
}

char* focus(char* str, bool focused) {
  if(focused) {
    char* result = safeMalloc(sizeof(char)*(strlen(str)+strlen(FOCUS_STYLE)+strlen(FOCUS_RESET)));
    strcat(result, FOCUS_STYLE);
    strcat(result, str);
    strcat(result, FOCUS_RESET);
    return result;
  } else return str;
}

void showBooks(Book books[], int length, Field focused) {
  int max[5] = {5, 6, 2, 9, 12};
  for(int i = 0; i < length; i++) {
    Book book = books[i];
    int length[5] = {strlen(book.title), strlen(book.author), lengthOfInt(book.id), strlen(categoryToString(book.category)), book.borrower == NULL ? 0 : strlen(book.borrower->login)};
    for(int i = 0; i < 5; i++) {
      if(max[i] < length[i]) max[i] = length[i];
    }
  }

  int totalLength = max[0] + max[1] + max[2] + max[3] + max[4] + 5;

  printf("%s\n", completeWithString("", totalLength+1, "-"));
  printf(
    "|%s|%s|%s|%s|%s|\n",
    focus(completeWithString("Titre", max[0], " "), focused == TITLE),
    focus(completeWithString("Auteur", max[1], " "), focused == AUTHOR),
    focus(completeWithString("ID", max[2], " "), focused == ID),
    focus(completeWithString("Catégorie", max[3], " "), focused == CATEGORY),
    focus(completeWithString("Emprunté par", max[4], " "), focused == BORROWER)
  );
  printf("%s\n", completeWithString("", totalLength+1, "-"));

  for(int i = 0; i < length; i++) {
    Book book = books[i];
    char* bookId = safeMalloc(lengthOfInt(book.id));
    sprintf(bookId, "%d", book.id);
    printf(
      "|%s|%s|%s|%s|%s|\n",
      focus(completeWithString(book.title, max[0], " "), focused == TITLE),
      focus(completeWithString(book.author, max[1], " "), focused == AUTHOR),
      focus(completeWithString(bookId, max[2], " "), focused == ID),
      focus(completeWithString(categoryToString(book.category), max[3], " "), focused == CATEGORY),
      focus(completeWithString(book.borrower == NULL ? "" : book.borrower->login, max[4], " "), focused == BORROWER)
    );
  }

    printf("%s\n", completeWithString("", totalLength+1, "-"));
}

int askInt(char* message, char* choices[], int length) {
  printf("%s\n", message);
  for(int i = 0; i < length; i++) printf("[%d]: %s\n", i+1, choices[i]);
  int answer;
  scanf("%d", &answer);
  while (answer < 1 || answer > length) {
    printf("Veuillez saisir un nombre entre 1 et %d\n", length);
    scanf("%d", &answer);
  }
  return answer-1;
}
