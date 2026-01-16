#ifndef PARSER_H_
#define PARSER_H_

#include <stdbool.h>

#include "ast/grammar.h"

// main recursive parsing function
Program* parse(Vector tokens);
bool parse_line(Program* program, Vector tokens);

#endif // PARSER_H_
