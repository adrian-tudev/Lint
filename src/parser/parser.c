#include "parser.h"

#include <assert.h>

#include "ast/grammar.h"
#include "parser/statement.h"
#include "lexer/token.h"
#include "utils/error.h"

// recursive descent parser
Program* parse(Vector tokens) {
  init_parse_context(tokens);

  Program* program = program_new();

  Statement* stmt = NULL;
    
  while (true) {
    stmt = parse_statement();
    if (stmt == NULL) break;
    program_add_statement(program, stmt);
  }

  return program;
}

// add statements to a program (mainly for repl)
// TODO: parse all TopLevel items
bool parse_line(Program* program, Vector tokens) {
  init_parse_context(tokens);

  Statement* stmt = parse_statement();
  if (stmt == NULL) return false;

  program_add_statement(program, stmt);
  
  return true;
}

