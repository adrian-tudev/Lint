#ifndef PARSER_STATEMENT_H_
#define PARSER_STATEMENT_H_

#include "parser/parse_context.h"
#include "ast/grammar.h"

Statement *parse_statement(void);
Statement *parse_block_statement(void);
Block *statement_to_block(Statement *stmt);

#endif // STATEMENT_H_
