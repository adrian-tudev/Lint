#include "lint.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <readline/history.h>
#include <readline/readline.h>

#include "parser/parser.h"
#include "execution/interpreter.h"
#include "lexer/token.h"

static volatile bool run = 1;
void quitter(int) {
  run = 0;
}

void run_file(RuntimeConfig file_cfg) {
  FILE* file = fopen(file_cfg.file, "r");
  if (file == NULL) {
    printf("File not found!\n");
    exit(1);
  }

  // temporary size probably
  char line[512];
  uint32_t rows = 1;

  Vector tokens;
  vec_init(&tokens);

  set_interpreter_config(file_cfg);
  
  // read all lines and create tokens
  while (fgets(line, sizeof(line), file)) { 
    Vector line_tokens = tokenize(line, rows);
    vec_merge(&tokens, &line_tokens);
    vec_free(&line_tokens);
    rows++;
  }

  Program* program = parse(tokens);

  execute(program);

  // debug
  if (file_cfg.debug_info) {
    ast_print_program(program);
    hm_print(program->ctx);
  }

  program_free(program);
  vec_free(&tokens);
  fclose(file);
}

// Read-Eval-Print-Loop
void repl(RuntimeConfig cfg) {
  signal(SIGINT, quitter);

  Program* program = program_new();
  char* line;
  while (run) {
    line = readline("> ");
    if (line == NULL) break;

    if (*line) {
      add_history(line);

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

      vec_free(&tokens);

      // debugging
      if (cfg.debug_info) {
        ast_print_program(program);
        hm_print(program->ctx);
      }
    }
    free(line);
  }

  program_free(program);
}

