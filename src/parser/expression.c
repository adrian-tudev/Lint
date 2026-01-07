#include "parser/expression.h"

#include "ast/grammar.h"
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
  return expr;
}

static Expression* parse_unary(void) {
  const Token* token = peek();
  if (token == NULL) {
    error_log("Unexpected end of input, expected unary expression.\n");
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
  if (match(LITERAL)) return expr_number(token->literal);
  else if (match(TRUE)) return expr_bool(true);
  else if (match(FALSE)) return expr_bool(false);
  else if (match(IDENTIFIER)) return expr_identifier(token->token);
  else if (match(LEFT_PARENTHESIS)) {
    Expression* expr = parse_expression();
    if (!match(RIGHT_PARENTHESIS)) {
      if (ctx_end()) {
        error_log("Expected ')' after expression, but reached end of input.\n");
      } else {
        error_log("Expected ')' after expression, but got '%s'.\n", peek()->token);
      }
      return NULL;
    }
    return expr;
  } else {
    if (ctx_end()) {
      error_log("Unexpected end of input, expected an expression.\n");
    } else {
      error_log("Unexpected token '%s', expected an expression.\n", peek()->token);
    }
    return NULL;
  }
}
