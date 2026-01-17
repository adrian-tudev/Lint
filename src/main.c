#include <stdio.h>
#include <string.h>

#include "execution/interpreter.h"
#include "lint.h"

RuntimeConfig cfg = (RuntimeConfig) {
  .debug_info = false,
  .repl = true,
  .file = NULL,
};

int main(int argc, char* args[]) {
  printf("Lint 0.0.1\n");

  for (int i = 0; i < argc; i++) {
    // check for ".li" extension file
    size_t lst = strlen(args[i]);
    if (lst >= 3) {
      if (args[i][lst - 3] == '.' 
          && args[i][lst - 2] == 'l'
          && args[i][lst - 1] == 'i') {
        cfg.file = args[i];
        cfg.repl = false;
      }
    }
    if (strcmp(args[i], "-d") == 0)
      cfg.debug_info = true;
  }

  if (cfg.file != NULL) {
    run_file(cfg);
  } else {
    repl(cfg);
  }

  return 0;
}
