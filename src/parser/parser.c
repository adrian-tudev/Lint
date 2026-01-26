#include "parser.h"

#include <assert.h>

#include "ast/grammar.h"
#include "parser/statement.h"
#include "parser/function.h"
#include "lexer/token.h"
#include "utils/error.h"

static bool try_parse(Program* program);

// recursive descent parser
Program* parse(Vector tokens) {
  init_parse_context(tokens);

  Program* program = program_new();

  while (try_parse(program)) { }

  return program;
}

// toplevel statements to a program (mainly for repl)
bool parse_line(Program* program, Vector tokens) {
  init_parse_context(tokens);
  return try_parse(program);
}

static bool try_parse(Program* program) {
  // TopLevel statements
  Statement* stmt = NULL;
  Function* fn = NULL;
    
  size_t lst_idx = get_ctx_idx();
  stmt = parse_statement();
  if (stmt) {
    program_add_statement(program, stmt);
    return true;
  }   
  // reset token
  set_ctx_idx(lst_idx);

  // failed to parse statement: 
  // try parse function definition
  fn = parse_function_def();
  if (fn) {
    program_add_function(program, fn);
    return true;
  }

  return false;
}

