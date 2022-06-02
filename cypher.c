#include <stdlib.h>
#include <string.h>
#include "cypher.h"
#include "util.h"

//Encrypt the given password
char* encrypt(char* message,char* key){
  int lmessage= strlen(message);
  char* resizedKey=lengthKey(key,lmessage);
  char* result=safeMalloc(sizeof(char)*lmessage);
  for (int i = 0; i <lmessage; i++) {
    result[i] = message[i]+resizedKey[i]-'A';
    printf("%c %c %c\n", message[i], resizedKey[i], result[i]);
  }
  return result;
}


//Decrypt the encrypted password
char* decrypt(char* crypmessage,char* key){
  int lmessage= strlen(crypmessage);
  char* resizedKey=lengthKey(key,lmessage);
  char* result=safeMalloc(sizeof(char)*lmessage);
  for (int i = 0; i <lmessage; i++) {
    result[i] = crypmessage[i]-resizedKey[i]+'A';
  }
  return result;
}

//Lengthen the key
char* lengthKey(char* key, int n){
  char* result = safeMalloc(sizeof(char)*n);
  int lkey= strlen(key);
  for (int i = 0; i< n; i++){
    result[i] = key[i%lkey];
  }
  return result;
}
