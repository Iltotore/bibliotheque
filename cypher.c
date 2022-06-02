#include <stdlib.h>
#include <string.h>
#include "cypher.h"
#include "util.h"

//Encrypt the given password. Uses a cropped ASCII Table between 33 (!) and 126 (~) to avoid invisible characters.
char* encrypt(char* message,char* key){
  int lmessage = strlen(message);
  char* resizedKey = lengthKey(key,lmessage);
  char* result = safeMalloc(sizeof(char)*(lmessage+1));
  for (int i = 0; i < lmessage; i++) {
    int offset = message[i]+resizedKey[i]-'A'; //Classical Viegenère cypher.
    result[i] = offset%127+'!'*(offset/127); //Apply the calculated offset. If the value is out of bounds (>=127), it is shifted to `!` + modulo by 127.
  }
  result[lmessage] = '\0';
  return result;
}


//Decrypt the encrypted password. Uses a cropped ASCII Table between 33 (!) and 126 (~) to avoid invisible characters.
char* decrypt(char* crypmessage,char* key){
  int lmessage = strlen(crypmessage);
  char* resizedKey = lengthKey(key,lmessage);
  char* result = safeMalloc(sizeof(char)*(lmessage+1));
  for (int i = 0; i <lmessage; i++) {
    int offset = 'A'+crypmessage[i]-resizedKey[i]; //Classical Viegenère cypher reverse.
    if(offset >= '!' && offset <= '~') result[i] = offset; //If the classically reversed character is in bounds, then it is the correct one.
    else result[i] = offset-'!'+127; //Otherwise, it means it was shifted during encryption an should be "unshifted".
  }
  result[lmessage] = '\0';

  return result;
}

//Extends the given key to fit the given length
char* lengthKey(char* key, int length){
  char* result = safeMalloc(sizeof(char)*(length+1));
  int lkey= strlen(key);
  for (int i = 0; i< length; i++){
    result[i] = key[i%lkey];
  }
  result[length] = '\0';
  return result;
}
