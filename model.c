#include "model.h"


int categoryToInt(Category a){
  switch(a) {
    case SCIENCES: return 0;
    case LITERATURE: return 1;
    case FICTION: return 2;
    case GENERAL_KNOWLEDGE: return 3;
  }
}
