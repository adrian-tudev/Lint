#define  _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "lint.h"

int main(int argc, char* args[]) {
  printf("Lint 0.0.1\n");
  if (argc == 2) {
    runFile(args[1]);
    return 0;
  }

  while (true) {
    printf("> ");
    char* line = NULL;
    size_t len = 0;

    if (getline(&line, &len, stdin) != -1) {
      run(line, 0);
    }
  }

  return 0;
}
