#include "model.h"
#include <stdlib.h>

int categoryToInt(Category a){
  switch(a) {
    case SCIENCES: return 0;
    case LITERATURE: return 1;
    case FICTION: return 2;
    case GENERAL_KNOWLEDGE: return 3;
  }
}

Category intToCategory(int a){
  switch(a) {
    case 0: return SCIENCES;
    case 1: return LITERATURE;
    case 2: return FICTION;
    case 3: return GENERAL_KNOWLEDGE;
  }
}

User* getUser(char* login){
  return NULL;	
}
