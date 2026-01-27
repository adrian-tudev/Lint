#include "parser/expression.h"

#include <assert.h>

#include "ast/grammar.h"
#include "parser/function.h"
#include "utils/error.h"

// precedence levels in increasing order
typedef enum {
    LEVEL_LOGICAL_OR,
    LEVEL_LOGICAL_AND,
    LEVEL_EQUALITY,
    LEVEL_COMPARISON,
    LEVEL_TERM,
    LEVEL_FACTOR,
    LEVEL_UNARY,
    LEVEL_PRIMARY,
} Levels;

typedef struct {
  const OperatorKind* ops;
  size_t count;
} OpPrecedence;

static Expression *parse_binary(Levels level);
static Expression* parse_unary(void);
static Expression* parse_primary(void);
static Expression* parse_function_call(void);

// expr  ::= term ( ( "+" | "-" ) term )*
// factor ::= NUMBER | IDENTIFIER | "(" expr ")"
// term ::= factor ( ( "*" | "/" ) factor )*
Expression *parse_expression(void) {
  return parse_binary(LEVEL_LOGICAL_OR);
}

static const OperatorKind ops_or[] = {OP_OR};
static const OperatorKind ops_and[] = {OP_AND};
static const OperatorKind ops_equality[] = {OP_EQUAL, OP_NOT_EQUAL};
static const OperatorKind ops_comparison[] = {OP_LESS_THAN, OP_LESS_OR_EQUAL, OP_GREATER_THAN, OP_GREATER_OR_EQUAL};
static const OperatorKind ops_term[] = {OP_ADD, OP_SUB};
static const OperatorKind ops_factor[] = {OP_MUL, OP_DIV};

static const OpPrecedence precedences[] = {
  {ops_or, 1},
  {ops_and, 1},
  {ops_equality, 2},
  {ops_comparison, 4},
  {ops_term, 2},
  {ops_factor, 2}
};

static OperatorKind find_operator_at_level(Levels level) {
  OpPrecedence prec = precedences[level];
  for (size_t i = 0; i < prec.count; i++) {
    if (match_op(prec.ops[i])) {
      return prec.ops[i];
    }
  }
  return OP_INVALID; // no match
}

static Expression* parse_binary(Levels level) {
  if (level == LEVEL_UNARY) return parse_unary();

  Expression* expr = parse_binary(level + 1);
  if (expr == NULL) return NULL;

  while (true) {
    OperatorKind op_kind = find_operator_at_level(level);
    if (op_kind == OP_INVALID) break;
    Expression* right = parse_binary(level + 1);
    if (right == NULL) return NULL;
    expr = expr_binary(op_kind, expr, right);
  }
  return expr; // Vector<Expression*>
}

static Expression* parse_unary(void) {
  const Token* token = peek();
  if (token == NULL) {
    error_log("Unexpected EOF\n");
    return NULL;
  }
  OperatorKind op = token_type_to_op(token->type);
  if (match_op(OP_NOT) || match_op(OP_SUB) || match_op(OP_ADD)) {
    Expression* operand = parse_unary();
    if (operand == NULL) return NULL;
    return expr_unary(op, operand);
  }
  return parse_primary();
}

static Expression* parse_primary(void) {
  const Token* token = peek();
  if (token == NULL) {
    error_log("Unexpected EOF, expected an expression.\n");
    return NULL;
  }

  if (match(TOK_LITERAL)) return expr_number(token->literal);
  else if (match(TOK_TRUE)) return expr_bool(true);
  else if (match(TOK_FALSE)) return expr_bool(false);
  else if (token->type == TOK_IDENTIFIER) {
    if (peek_next() != NULL && peek_next()->type == TOK_LEFT_PARENTHESIS) {
      return parse_function_call();
    }
    assert(match(TOK_IDENTIFIER));
    return expr_identifier(token->token);
  }
  else if (match(TOK_LEFT_PARENTHESIS)) {
    Expression* expr = parse_expression();
    if (!match(TOK_RIGHT_PARENTHESIS)) {
      if (ctx_end()) {
        error_log("Expected ')' after expression, but reached EOF.\n");
      } else {
        error_log("Expected ')' after expression, but got '%s'.\n", peek()->token);
      }
      return NULL;
    }
    return expr;
  } else if (match(TOK_RIGHT_PARENTHESIS)) {
    error_log("Unexpected ')' at %u:%u\n", token->row, token->column);
    return NULL;
  }
  else {
    error_log("Unexpected token '%s', expected an expression.\n", peek()->token);
    return NULL;
  }
}

static void free_expression_vector(Vector* v) {
  for (size_t i = 0; i < v->size; i++) {
    expr_free((Expression*)vec_get(v, i));
  }
  vec_free(v);
}

static Expression* parse_function_call(void) {
  // TODO: error msg?
  if (peek() == NULL) return NULL;
  const char* fn_name = peek()->token;
  assert(match(TOK_IDENTIFIER));
  assert(match(TOK_LEFT_PARENTHESIS));

  Vector args; // Vector<Expression*>
  vec_init(&args);

  // no arguments given
  if (match(TOK_RIGHT_PARENTHESIS)) goto ret;

  do {
    Expression* arg = parse_expression();
    if (arg == NULL) {
      free_expression_vector(&args);
      return NULL;
    }
    vec_push(&args, arg);
  } while (match(TOK_COMMA));
  if (!match(TOK_RIGHT_PARENTHESIS)) {
    error_log("Expected ')' after function call!\n");
    free_expression_vector(&args);
    return NULL;
  }
ret:
  return expr_fn_call(fn_name, &args);
}

