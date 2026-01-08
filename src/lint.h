#ifndef LINT_H_
#define LINT_H_

#include "lexer/token.h"

void repl(const char* line, uint32_t row);
void run_file(const char* file);

#endif  // LINT_H
