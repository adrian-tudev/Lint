#ifndef EVAL_H_
#define EVAL_H_

#include "ast/grammar.h"
#include "utils/hashmap.h"

Expression eval_expression(Expression* expression, HashMap* scope);

#endif // EVAL_H_
