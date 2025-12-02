#ifndef LINT_H_
#define LINT_H_

#include "token.h"

void run(const char* line, uint32_t row);
void runFile(const char* file);

#endif  // LINT_H
