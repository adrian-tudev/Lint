#include <stdio.h>

#include "lint.h"

int main(int argc, char* args[]) {
  printf("Lint 0.0.1\n");
  if (argc == 2) {
    run_file(args[1]);
    return 0;
  } else {
    repl();
  }

  return 0;
}
