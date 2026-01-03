#ifndef GRAMMAR_H_
#define GRAMMAR_H_

#include <stdbool.h>

#include "ast/grammar_types.h"

// =====================
// Constructors / destructors
//
// Ownership model:
// - AST nodes own their child node pointers.
// - AST nodes do NOT own identifier/string memory.
// =====================

Expression *expr_number(double value);
Expression *expr_bool(bool value);
Expression *expr_identifier(const char *name);
Expression *expr_string(const char *value);
Expression *expr_unary(OperatorKind op, Expression *operand);
Expression *expr_binary(OperatorKind op, Expression *left, Expression *right);
void expr_free(Expression *expr);

Block *block_new(void);
void block_init(Block *block);
bool block_add(Block *block, Statement *stmt);
void block_clear(Block *block);
void block_free(Block *block);

Statement *stmt_expr(Expression *expr);
Statement *stmt_assign(const char *identifier, Expression *rvalue);
Statement *stmt_return(Expression *value_or_null);
Statement *stmt_if(Expression *condition, Block *then_body, Block *else_body_or_null);
Statement *stmt_while(Expression *condition, Block *body);
// Statement *stmt_block(void);
void stmt_free(Statement *stmt);

Function *function_new(const char *identifier);
bool function_add_param(Function *fn, const char *param);
bool function_add_stmt(Function *fn, Statement *stmt);
void function_free(Function *fn);

Program *program_new(void);
bool program_add_statement(Program *p, Statement *stmt);
bool program_add_function(Program *p, Function *fn);
void program_free(Program *p);

#endif // GRAMMAR_H_