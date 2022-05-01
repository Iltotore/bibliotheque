#include <stdio.h>
#include <string.h>
#include "model.h"
#include "util.c"

char* categoryToString(Category category) {
  switch (category) {
    case Sciences: return "Sciences";
    case Literature: return "Litérature";
    case Fiction: return "Fiction";
    case GeneralKnowledge: return "Culture Générale";
  }
}

char* completeWithString(char* base, int length, char* str) {
  int currentLength = strlen(base);
  char* result = safeMalloc(sizeof(char)*length);
  strcat(result, base);

  int steps = (length-currentLength)/strlen(str);
  for(int i = 0; i < steps; i++) {
    strcat(result, str);
  }
  return result;
}

void showBooks(Book books[], int length) {
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
    completeWithString("Titre", max[0], " "),
    completeWithString("Auteur", max[1], " "),
    completeWithString("ID", max[2], " "),
    completeWithString("Catégorie", max[3], " "),
    completeWithString("Emprunté par", max[4], " ")
  );
  printf("%s\n", completeWithString("", totalLength+1, "-"));

  for(int i = 0; i < length; i++) {
    Book book = books[i];
    char* bookId = safeMalloc(lengthOfInt(book.id));
    sprintf(bookId, "%d", book.id);
    printf(
      "|%s|%s|%s|%s|%s|\n",
      completeWithString(book.title, max[0], " "),
      completeWithString(book.author, max[1], " "),
      completeWithString(bookId, max[2], " "),
      completeWithString(categoryToString(book.category), max[3], " "),
      completeWithString(book.borrower == NULL ? "" : book.borrower->login, max[4], " ")
    );
  }

    printf("%s\n", completeWithString("", totalLength+1, "-"));
}
