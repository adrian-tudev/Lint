#ifndef LINT_H_
#define LINT_H_

#include <stdbool.h>

#include "execution/interpreter.h"

// debug option prints the AST and the global context scope 
void repl(RuntimeConfig cfg);
void run_file(RuntimeConfig file_cfg);

#endif  // LINT_H
