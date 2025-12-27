#include "lint.h"

#include <stdio.h>
#include <stdlib.h>

#include "ast/parser.h"
#include "interpreter.h"
#include "token.h"

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
  Vector tokens = tokenize(line, row);

  // for now, just execute expressions
  Program* program = parse(tokens);
  for (size_t i = 0; i < program->items.size; i++) {
    Statement* stmt = ((TopLevel*)vec_get(&program->items, i))->as.statement;
    Expression* expr = stmt->as.expr;
    float result = eval_expression(expr).as.number;
    printf("%f\n", result);
  }

  program_free(program);
  vec_free(&tokens);
}
