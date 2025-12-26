#include "parser.h"

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

  return program;
}

Statement *parse_statement(void) {
  return NULL;
}

static Expression* parse_factor(void) {
  if (match(LITERAL)) {
    Token* token = (Token*)vec_get(&ctx.tokens, ctx.position - 1);
    Expression* expr = expr_number(token->literal);
    return expr;
  } else if (match(IDENTIFIER)) {
    Token* token = (Token*)vec_get(&ctx.tokens, ctx.position - 1);
    Expression* expr = expr_identifier(token->token);
    return expr;
  } else if (match(LEFT_PARENTHESIS)) {
    Expression* expr = parse_expression();
    if (!match(RIGHT_PARENTHESIS)) {
      // error: expected ')'
      return NULL;
    }
    return expr;
  }
  return NULL;
}

static Expression* parse_term(void) {
  return NULL;
} 

/*
expr  ::= term ( ( "+" | "-" ) term )*
term  ::= factor ( ( "*" | "/" ) factor )*
factor ::= NUMBER | IDENTIFIER | "(" expr ")" | '-' factor | '+' factor | '!' factor
*/
Expression *parse_expression(void) {
  Expression* left = parse_term();

  return NULL;
}

Function *parse_function(void) {
  return NULL;
}

