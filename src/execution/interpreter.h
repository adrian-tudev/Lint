#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "ast/grammar.h"

// walks the tree and executes the AST
bool execute(Program* program);
bool execute_function_def(Function* function);
bool execute_statement(Statement* statement);
bool execute_block(Block* block);

Expression eval_expression(Expression* expression);

#endif // INTERPRETER_H_
