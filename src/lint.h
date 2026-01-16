#ifndef LINT_H_
#define LINT_H_

#include <stdbool.h>

// debug option prints the AST and the global context scope 
void repl(bool debug);
void run_file(const char* file);

#endif  // LINT_H
