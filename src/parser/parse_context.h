#ifndef PARSER_AUX_H_
#define PARSER_AUX_H_

#include "utils/vector.h"
#include "ast/grammar.h"

void init_parse_context(Vector tokens);

// have a peek at the current token in the ParseContext
const Token* peek(void);
const Token* peek_next(void);
bool ctx_end(void);

// Return the current index the ParseContext is at
size_t get_ctx_idx(void);
// Set the current index of ParseContext
void set_ctx_idx(size_t idx);

// true if current operator in ParseContext matches given OperatorKind
bool match_op(OperatorKind op_kind);

// true if current token in ParseContext matches given TokenType
bool match(TokenType type);

// increment the token pointer
void advance(void);

#endif // PARSER_AUX_H_
