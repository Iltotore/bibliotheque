#ifndef cypher
#define cypher

//Encrypt the given password
char* encrypt(char* message,char* key);

//Decrypt the encrypted password
char* decrypt(char* crypmessage,char* key);

//Lengthen the key
char* lengthKey(char* key,int n);

#endif
