#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <readline/history.h>
#include <readline/readline.h>

// HACK: readline defines a `RETURN` macro that conflicts with a token
#ifdef RETURN
#undef RETURN
#endif

#include "lint.h"

int main(int argc, char* args[]) {
  printf("Lint 0.0.1\n");
  if (argc == 2) {
    run_file(args[1]);
    return 0;
  }

  char* line;
  while ((line = readline("> ")) != NULL) {
    if (*line) {
      add_history(line);
      repl(line, 0);
    }
    free(line);
  }

  return 0;
}
