#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "ast/grammar_init.h"

// walks the tree and executes the AST
bool execute(Program* program);
bool execute_function_def(Function* function);
bool execute_statement(Statement* statement);

Expression eval_expression(Expression* expression);

#endif // INTERPRETER_H_
