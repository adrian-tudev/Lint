#ifndef EXPR_PARSER_H_
#define EXPR_PARSER_H_

#include "parser/parse_context.h"
#include "ast/grammar.h"

Expression *parse_expression(void);

#endif // EXPR_PARSER_H_