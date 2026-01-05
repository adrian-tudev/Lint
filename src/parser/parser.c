#include "parser.h"

#include <assert.h>

#include "ast/grammar.h"
#include "parser/expression.h"
#include "token.h"
#include "utils/error.h"

// recursive descent parser
Program* parse(Vector tokens) {
  init_parse_context(tokens);

  Program* program = program_new();

  Expression* expr = parse_expression();
  if (expr == NULL) {
    return program;
  }

  Statement* stmt = stmt_expr(expr);
  program_add_statement(program, stmt);

  return program;
}