#ifndef PARSER_H_
#define PARSER_H_

#include "ast/grammar.h"

// main recursive parsing function
Program* parse(Vector tokens);

#endif // PARSER_H_