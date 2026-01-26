#include "parser/parse_context.h"

typedef struct ParseContext {
  Vector tokens;
  size_t idx;
} ParseContext;

static ParseContext ctx;

void init_parse_context(Vector tokens) {
  ctx.tokens = tokens;
  ctx.idx = 0;
}

bool match_op(OperatorKind op_kind) {
  if (ctx_end()) return false;
  Token* token = (Token*)vec_get(&ctx.tokens, ctx.idx);
  if (token_type_to_op(token->type) == op_kind) {
    advance();
    return true;
  }
  return false;
}

bool match(TokenType type) {
  if (ctx_end()) return false;
  Token* token = (Token*)vec_get(&ctx.tokens, ctx.idx);
  if (token->type == type) {
    advance();
    return true;
  }
  return false;
}

// Return the current index the ParseContext is at
size_t get_ctx_idx(void) { return ctx.idx; }
// Set the current index of ParseContext
void set_ctx_idx(size_t idx) { ctx.idx = idx; }
 
void advance(void) {
  if (ctx_end()) return;
  ctx.idx++;
}

const Token* peek(void) {
  if (ctx_end()) return NULL;
  return (Token*)vec_get(&ctx.tokens, ctx.idx);
}

const Token* peek_next(void) {
  if (ctx.idx + 1 >= ctx.tokens.size) return NULL;
  return (Token*)vec_get(&ctx.tokens, ctx.idx + 1);
}

bool ctx_end(void) {
  return ctx.idx >= ctx.tokens.size;
}
