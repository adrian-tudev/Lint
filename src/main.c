#include <stdio.h>
#include <string.h>

#include "execution/interpreter.h"
#include "lint.h"

RuntimeConfig cfg = (RuntimeConfig) {
  .debug_info = false,
  .show_tokens = false,
  .repl = true,
  .file = NULL,
};

int main(int argc, char* args[]) {
  printf("Lint 0.0.1\n");

  for (int i = 1; i < argc; i++) { // Start from 1 to skip the program name
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
    if (strcmp(args[i], "-d") == 0) {
      cfg.debug_info = true;
    } else if (strcmp(args[i], "--show_tokens") == 0) {
      cfg.show_tokens = true;
    } else if (strcmp(args[i], "--help") == 0) {
      printf("Usage: lint [options] [file]\n");
      printf("Options:\n");
      printf("  -d               : Enable debug information.\n");
      printf("  --show_tokens    : Display scanned tokens.\n");
      printf("  --help           : Show this help message and exit.\n");
      printf("  [file.li]        : Execute the specified Lint file.\n");
      return 0; // Exit after showing help
    }
  }

  if (cfg.file != NULL) {
    run_file(cfg);
  } else {
    repl(cfg);
  }

  return 0;
}
