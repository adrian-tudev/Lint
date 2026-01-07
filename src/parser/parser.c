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

  Statement* stmt = parse_statement();
  if (stmt == NULL) return program;

  program_add_statement(program, stmt);
  return program;
}