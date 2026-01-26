#include "parser.h"

#include <assert.h>

#include "ast/grammar.h"
#include "parser/statement.h"
#include "parser/function.h"
#include "lexer/token.h"
#include "utils/error.h"

// recursive descent parser
Program* parse(Vector tokens) {
  init_parse_context(tokens);

  Program* program = program_new();

  // TopLevel statements
  Statement* stmt = NULL;
  Function* fn = NULL;
    
  while (true) {
    size_t lst_idx = get_ctx_idx();
    stmt = parse_statement();
    if (stmt) {
      program_add_statement(program, stmt);
      continue;
    }   
    // reset token
    set_ctx_idx(lst_idx);

    // failed to parse statement: 
    // try parse function definition
    fn = parse_function_def();
    NULL;
    if (fn) {
      program_add_function(program, fn);
      continue;
    }

    // failed to parse anything
    break;
  }

  return program;
}

// add statements to a program (mainly for repl)
bool parse_line(Program* program, Vector tokens) {
  init_parse_context(tokens);

  Statement* stmt = parse_statement();
  if (stmt == NULL) return false;

  program_add_statement(program, stmt);
  
  return true;
}

