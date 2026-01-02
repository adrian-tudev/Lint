#include "parser.h"

#include <assert.h>

#include "ast/grammar_init.h"
#include "utils/error.h"
#include "token.h"

typedef struct ParseContext {
  Vector tokens;
  size_t position;
} ParseContext;

static ParseContext ctx;
static void init_parse_context(Vector tokens);
static bool match_op(OperatorKind op_kind);
static bool match(TokenType type);
static void advance(void);
static Token* peek(void);
static bool ctx_end(void);
static OperatorKind token_type_to_op(TokenType type);

static Expression* parse_binary(size_t level);
static Expression* parse_unary(void);
static Expression* parse_primary(void);

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

// expr  ::= term ( ( "+" | "-" ) term )*
// factor ::= NUMBER | IDENTIFIER | "(" expr ")"
// term ::= factor ( ( "*" | "/" ) factor )*
Expression *parse_expression(void) {
  return parse_binary(0);
}

static void init_parse_context(Vector tokens) {
  ctx.tokens = tokens;
  ctx.position = 0;
}

static bool match_op(OperatorKind op_kind) {
  if (ctx_end()) return false;
  Token* token = (Token*)vec_get(&ctx.tokens, ctx.position);
  return token_type_to_op(token->type) == op_kind;
}

// true if current token in ParseContext matches given TokenType
static bool match(TokenType type) {
  if (ctx_end()) return false;
  Token* token = (Token*)vec_get(&ctx.tokens, ctx.position);
  return token->type == type;
}

// increment the token pointer
static void advance(void) {
  if (ctx_end()) return;
  ctx.position++;
}

static Token* peek(void) {
  if (ctx_end()) return NULL;
  return (Token*)vec_get(&ctx.tokens, ctx.position);
}

static bool ctx_end(void) {
  return ctx.position >= ctx.tokens.size;
}

static void token_error(void) {
  Token* tok = (Token*)vec_get(&ctx.tokens, ctx.position);
  print_token(tok);
}

static OperatorKind token_type_to_op(TokenType type) {
  switch (type) {
    case PLUS: return OP_ADD;
    case MINUS: return OP_SUB;
    case STAR: return OP_MUL;
    case SLASH: return OP_DIV;
    case BANG: return OP_NOT;
    case EQUAL_EQUAL: return OP_EQUAL;
    case BANG_EQUAL: return OP_NOT_EQUAL;
    case LESS: return OP_LESS_THAN;
    case LESS_EQUAL: return OP_LESS_OR_EQUAL;
    case GREATER: return OP_GREATER_THAN;
    case GREATER_EQUAL: return OP_GREATER_OR_EQUAL;
    case AND: return OP_AND;
    case OR: return OP_OR;
    default: return (OperatorKind)-1;
  }
}

typedef struct {
  const OperatorKind* ops;
  size_t count;
} OpPrecedence;

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

static Expression* parse_binary(size_t level) {
  if (level >= sizeof(precedences) / sizeof(OpPrecedence)) {
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
      if (ctx_end()) {
        error_log("Unexpected end of file after binary operator", 0, 0);
      } else {
        error_log("Expected expression after binary operator", peek()->row, peek()->column);
      }
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
      if (ctx_end()) {
        error_log("Unexpected end of file after unary operator", 0, 0);
      } else {
        error_log("Expected expression after unary operator", peek()->row, peek()->column);
      }
      return NULL;
    }
    return expr_unary(op, operand);
  }
  return parse_primary();
}

static Expression* parse_primary(void) {
  // printf("parse_primary pos=%zu size=%zu\n", ctx.position, ctx.tokens.size);
  if (match(LITERAL)) {
    Token* token = (Token*)vec_get(&ctx.tokens, ctx.position);
    advance();
    Expression* expr = expr_number(token->literal);
    return expr;
  } else if (match(IDENTIFIER)) {
    Token* token = (Token*)vec_get(&ctx.tokens, ctx.position);
    advance();
    Expression* expr = expr_identifier(token->token);
    return expr;
  } else if (match(LEFT_PARENTHESIS)) {
    advance();
    Expression* expr = parse_expression();
    if (!match(RIGHT_PARENTHESIS)) {
      if (ctx_end()) {
        error_log("Unexpected end of file, expected ')'", 0, 0);
      } else {
        error_log("Expected ')' after expression", peek()->row, peek()->column);
      }
      return NULL;
    }
    advance();
    return expr;
  }
  
  // Error handling
  if (ctx_end()) {
      error_log("Unexpected end of file", 0, 0);
  } else {
      error_log("Unexpected token", peek()->row, peek()->column);
      token_error();
  }
  return NULL;
}