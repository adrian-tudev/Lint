#include "lint.h"

#include <stdio.h>
#include <stdlib.h>

#include "parser/parser.h"
#include "execution/interpreter.h"
#include "lexer/token.h"

void run_file(const char* path) {
  FILE* file = fopen(path, "r");
  if (file == NULL) {
    printf("File not found!\n");
    exit(1);
  }

  // temporary size probably
  char line[512];
  uint32_t rows = 1;

  Vector tokens;
  vec_init(&tokens);
  
  // read all lines and create tokens
  while (fgets(line, sizeof(line), file)) { 
    Vector line_tokens = tokenize(line, rows);
    // append line tokens to main tokens vector
    for (size_t i = 0; i < line_tokens.size; i++) {
      Token* tok = (Token*) vec_get(&line_tokens, i);
      vec_push(&tokens, tok);
    }
    vec_free(&line_tokens);
    rows++;
  }

  Program* program = parse(tokens);
  execute(program);

  program_free(program);

  // print tokens for debugging
  // for (size_t i = 0; i < tokens.size; i++) {
  //   Token* tok = (Token*) vec_get(&tokens, i);
  //   print_token(tok);
  // }

  vec_free(&tokens);

  fclose(file);
}

// 
void repl(const char* line, uint32_t row) {
  Vector tokens = tokenize(line, row);

  Program* program = parse(tokens);
  execute(program);

  program_free(program);
  vec_free(&tokens);
}
