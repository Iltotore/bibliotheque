#define KEY "CYTECHLIBRARY"
#ifndef cypher
#define cypher

//Encrypt the given password. Uses a cropped ASCII Table between 33 (!) and 126 (~) to avoid invisible characters.
char* encrypt(char* message,char* key);

//Decrypt the encrypted password. Uses a cropped ASCII Table between 33 (!) and 126 (~) to avoid invisible characters.
char* decrypt(char* crypmessage,char* key);

//Extends the given key to fit the given length
char* lengthKey(char* key, int length);

#endif
