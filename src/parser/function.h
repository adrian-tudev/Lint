#ifndef FUNCTION_H_
#define FUNCTION_H_

#include "ast/grammar.h"
#include "parse_context.h"

Function* parse_function_def(void);
Expression* parse_function_call(void);

#endif // FUNCTION_H_
