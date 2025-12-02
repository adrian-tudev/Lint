#include "lint.h"
#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "utils/vector.h"

void runFile(const char* path) {
  FILE* file = fopen(path, "r");
  if (file == NULL) {
    printf("File not found!\n");
    exit(1);
  }

  // temporary size probably
  char line[512];
  uint32_t rows = 1;
  while (fgets(line, sizeof(line), file)) { 
    run(line, rows++);
  }

  fclose(file);
}

void run(const char* line, uint32_t row) {
  Vector tokens = tokenize(line, 0);

  for (size_t i = 0; i < tokens.size; i++) {
    print_token(vec_get(&tokens, i));
    Token* tok = vec_get(&tokens, i);

    free(tok->token);
    free(tok);
  }

  vec_free(&tokens);
}
