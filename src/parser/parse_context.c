#include "parser/parse_context.h"

typedef struct ParseContext {
  Vector tokens;
  size_t position;
} ParseContext;

static ParseContext ctx;

void init_parse_context(Vector tokens) {
  ctx.tokens = tokens;
  ctx.position = 0;
}

bool match_op(OperatorKind op_kind) {
  if (ctx_end()) return false;
  Token* token = (Token*)vec_get(&ctx.tokens, ctx.position);
  return token_type_to_op(token->type) == op_kind;
}

bool match(TokenType type) {
  if (ctx_end()) return false;
  Token* token = (Token*)vec_get(&ctx.tokens, ctx.position);
  return token->type == type;
}

void advance(void) {
  if (ctx_end()) return;
  ctx.position++;
}

Token* peek(void) {
  if (ctx_end()) return NULL;
  return (Token*)vec_get(&ctx.tokens, ctx.position);
}

bool ctx_end(void) {
  return ctx.position >= ctx.tokens.size;
}
