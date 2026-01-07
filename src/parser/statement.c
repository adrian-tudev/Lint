#include "parser/statement.h"

#include <assert.h>
#include "parser/expression.h"
#include "utils/error.h"

static Statement *parse_let_statement(void);
static Statement *parse_return_statement(void);
static Statement *parse_if_statement(void);
static Statement *parse_while_statement(void);

Statement *parse_statement(void) {
  const Token *token = peek();
  if (token == NULL) return NULL;

  switch (token->type) {

  case LET:
    return parse_let_statement();
  case RETURN:
    break;

  case IF:
    break;
  case ELIF:
    break;
  case ELSE:
    break;

  case FOR:
    break;
  case WHILE:
    break;

  // TODO: match against only valid expression symbols
  default: {
    Expression *expr = parse_expression();
    if (expr == NULL) return NULL;
    if (!match(SEMICOLON)) {
      error_log("Expected ';' after expression\n");
      return NULL;
    }
    return stmt_expr(expr);
  }
    break;
  }

  return NULL;
}

static Statement *parse_let_statement(void) {
  // guarantee we have 'let' token
  assert(match(LET));

  // store the identifier token
  const Token *variable = peek();
  if (variable == NULL) {
    error_log("Unexpected end of input after 'let'\n");
    return NULL;
  }
  if (match(IDENTIFIER)) {
    // expect '=' token
    if (!match(EQUAL)) {
      error_log("Expected '=' after identifier in let statement at %u:%u\n",
                variable->row, variable->column);
      return NULL;
    }

    // parse the rvalue expression
    Expression *rvalue = parse_expression();
    if (rvalue == NULL) {
      error_log("Expected expression after '=' in let statement at %u:%u\n",
                variable->row, variable->column);
      return NULL;
    }

    // expect ';' token
    if (!match(SEMICOLON)) {
      error_log("Expected ';' after expression in let statement at %u:%u\n",
                variable->row, variable->column);
      return NULL;
    }

    return stmt_assign(variable->token, rvalue);
  } else {
    error_log("Expected identifier after 'let' at %u:%u\n", variable->row,
              variable->column);
  }

  return NULL;
}