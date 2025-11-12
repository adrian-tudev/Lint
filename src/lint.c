#include "lint.h"

void run(const char* line) { 
  int sz;
  Token* tokens = tokenize(line, &sz); 
  printf("number of tokens: %d\n", sz);

  for (int i = 0; i < sz; i++) {
    printf("%s ", tokens[i].token);
    free(tokens[i].token);
  }
  printf("\n");

  free(tokens);
}
