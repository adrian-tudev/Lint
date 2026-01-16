#include "lint.h"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>

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
  ast_print_program(program);
  execute(program);
  hm_print(program->ctx);

  program_free(program);

  vec_free(&tokens);

  fclose(file);
}

// Read-Eval-Print-Loop
void repl(bool debug) {
  Program* program = program_new();
  char* line;
  while ((line = readline("> ")) != NULL) {
    if (*line) {
      Vector tokens = tokenize(line, 0);

      // try parse line
      if (!parse_line(program, tokens)) {
        vec_free(&tokens);
        free(line);
        continue;
      }

      // execute last line
      Vector stmts = program->items;
      TopLevel* last = (TopLevel*)vec_get(&stmts, stmts.size - 1);

      if (last == NULL) continue;

      execute_top_level(last, program->ctx);

      add_history(line);

      vec_free(&tokens);

      // debugging
      if (debug) {
        ast_print_program(program);
        hm_print(program->ctx);
      }
    }
    free(line);
  }

  program_free(program);
}

