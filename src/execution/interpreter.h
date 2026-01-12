#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "ast/grammar.h"
#include "utils/hashmap.h"

// walks the tree and executes the AST
bool execute(Program* program);
bool execute_function_def(Function* function);
bool execute_statement(Statement* statement, HashMap* scope);
bool execute_block(Block* block);

#endif // INTERPRETER_H_
