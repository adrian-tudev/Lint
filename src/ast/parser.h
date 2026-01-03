#ifndef PARSER_H_
#define PARSER_H_

#include "ast/grammar.h"

// main recursive parsing function
Program* parse(Vector tokens);

// helper functions
Statement* parse_statement(void);
Expression* parse_expression(void);
Function* parse_function(void);

#endif // PARSER_H_