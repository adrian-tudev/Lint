#ifndef PARSER_AUX_H_
#define PARSER_AUX_H_

#include "utils/vector.h"
#include "ast/grammar.h"

void init_parse_context(Vector tokens);

// have a peek at the current token in the ParseContext
const Token* peek(void);
const Token* peek_next(void);
bool ctx_end(void);

// true if current operator in ParseContext matches given OperatorKind
bool match_op(OperatorKind op_kind);

// true if current token in ParseContext matches given TokenType
bool match(TokenType type);

// increment the token pointer
void advance(void);

#endif // PARSER_AUX_H_
