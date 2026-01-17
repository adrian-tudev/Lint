#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "ast/grammar.h"
#include "utils/hashmap.h"

typedef struct RuntimeConfig{
  bool repl;
  bool debug_info;
  const char* file;
} RuntimeConfig;

void set_interpreter_config(RuntimeConfig config);

// walks the tree and executes the AST
bool execute(Program* program);
bool execute_top_level(TopLevel* item, HashMap* ctx);
bool execute_function_def(Function* function);
bool execute_statement(Statement* statement, HashMap* scope);
bool execute_block(Block* block, HashMap* parent_scope);

#endif // INTERPRETER_H_
