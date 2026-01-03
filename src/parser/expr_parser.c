#include "parser/expr_parser.h"

#include "ast/grammar.h"
#include "utils/error.h"

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

static Expression* parse_binary(Levels level) {
  if (level == LEVEL_UNARY) {
    return parse_unary();
  }

  Expression* expr = parse_binary(level + 1);
  if (expr == NULL) return NULL;

  OpPrecedence prec = precedences[level];
  while (true) {
    bool matched = false;
    for (size_t i = 0; i < prec.count; i++) {
      if (match_op(prec.ops[i])) {
        matched = true;
        break;
      }
    }
    
    if (!matched) break;

    OperatorKind op = token_type_to_op(peek()->type);
    advance();
    Expression* right = parse_binary(level + 1);
    if (right == NULL) {
      error_log("Expected expression after binary operator\n");
      return NULL;
    }
    expr = expr_binary(op, expr, right);
  }
  return expr;
}

static Expression* parse_unary(void) {
  if (match_op(OP_NOT) || match_op(OP_SUB) || match_op(OP_ADD)) {
    OperatorKind op = token_type_to_op(peek()->type);
    advance();
    Expression* operand = parse_unary();
    if (operand == NULL) {
      error_log("Expected expression after unary operator\n");
      return NULL;
    }
    return expr_unary(op, operand);
  }
  return parse_primary();
}

static Expression* parse_primary(void) {
  if (match(LITERAL)) {
    Token* token = peek();
    advance();
    Expression* expr = expr_number(token->literal);
    return expr;
  } else if (match(TRUE)) {
    advance();
    return expr_bool(true);
  } else if (match(FALSE)) {
    advance();
    return expr_bool(false);
  } else if (match(IDENTIFIER)) {
    Token* token = peek();
    advance();
    Expression* expr = expr_identifier(token->token);
    return expr;
  } else if (match(LEFT_PARENTHESIS)) {
    advance();
    Expression* expr = parse_expression();
    if (!match(RIGHT_PARENTHESIS)) {
      error_log("Expected ')' after expression\n");
      return NULL;
    }
    advance();
    return expr;
  } else {
    error_log("Unexpected token\n");
    if (!ctx_end()) print_token(peek());
    return NULL;
  }
}
