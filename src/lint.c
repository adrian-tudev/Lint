#include "lint.h"

#include <stdio.h>
#include <stdlib.h>

#include "parser/parser.h"
#include "execution/interpreter.h"
#include "lexer/token.h"

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

// tokenize, parse into AST, execute
void run(const char* line, uint32_t row) {
  Vector tokens = tokenize(line, row);

  Program* program = parse(tokens);
  execute(program);

  program_free(program);
  vec_free(&tokens);
}
