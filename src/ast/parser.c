#include "parser.h"

#include "ast/grammar_init.h"
#include "utils/error.h"
#include "token.h"

typedef struct ParseContext {
  Vector tokens;
  size_t position;
} ParseContext;

static ParseContext ctx;

static void init_parse_context(Vector tokens) {
  ctx.tokens = tokens;
  ctx.position = 0;
}

__always_inline bool ctx_end() {
  return ctx.position >= ctx.tokens.size;
}

bool match(TokenType type) {
  if (ctx_end()) return false;

  Token* token = (Token*)vec_get(&ctx.tokens, ctx.position);
  if (token->type != type) return false;
  ctx.position++;
  return true;
}

Token* peek() {
  if (ctx_end()) return NULL;
  return (Token*)vec_get(&ctx.tokens, ctx.position);
}

/*
Simple recursive descent parser
*/
Program* parse(Vector tokens) {
  init_parse_context(tokens);

  Program* program = program_new();

  Statement* stmt = stmt_expr(parse_expression());
  program_add_statement(program, stmt);

  return program;
}

Statement *parse_statement(void) {
  return NULL;
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
    default: return OP_ADD;
  }
}

// factor ::= NUMBER | IDENTIFIER | "(" expr ")"
static Expression* parse_factor(void) {
  if (match(LITERAL)) {
    Token* token = (Token*)vec_get(&ctx.tokens, ctx.position - 1);
    Expression* expr = expr_number(token->literal);
    return expr;
  } else if (match(IDENTIFIER)) {
    Token* token = (Token*)vec_get(&ctx.tokens, ctx.position - 1);
    Expression* expr = expr_identifier(token->token);
    return expr;
  } else if (match(PLUS)) {
    Expression* right = parse_factor();
    Expression* unary_expr = expr_unary(OP_ADD, right);
    return unary_expr;
  } else if (match(MINUS)) {
    Expression* right = parse_factor();
    Expression* unary_expr = expr_unary(OP_SUB, right);
    return unary_expr;
  } else if (match(BANG)) {
    Expression* right = parse_factor();
    Expression* unary_expr = expr_unary(OP_NOT, right);
    return unary_expr;
  } else if (match(LEFT_PARENTHESIS)) {
    Expression* expr = parse_expression();
    if (!match(RIGHT_PARENTHESIS)) {
      error_log("Expected ')' after expression", peek()->row, peek()->column);
      return NULL;
    }
    return expr;
  }
  return NULL;
}

// term ::= factor ( ( "*" | "/" ) factor )*
static Expression* parse_term(void) {
  Expression* left = parse_factor();

  if (match(STAR) || match(SLASH)) {
    Token* operator_token = (Token*)vec_get(&ctx.tokens, ctx.position - 1);
    Expression* right = parse_factor();

    Expression* expr = expr_binary(token_type_to_op(operator_token->type), left, right);
    return expr;
  }
  
  return left;
} 

/*
expr  ::= term ( ( "+" | "-" ) term )*
term  ::= factor ( ( "*" | "/" ) factor )*
factor ::= NUMBER | IDENTIFIER | "(" expr ")" | '+' factor | '-' factor | '!' factor
*/
Expression *parse_expression(void) {
  Expression* left = parse_term();

  if (match(PLUS) || match(MINUS)) {
    Token* operator_token = (Token*)vec_get(&ctx.tokens, ctx.position - 1);
    Expression* right = parse_term();

    Expression* expr = expr_binary(token_type_to_op(operator_token->type), left, right);
    return expr;
  }

  return left;
}

Function *parse_function(void) {
  return NULL;
}

