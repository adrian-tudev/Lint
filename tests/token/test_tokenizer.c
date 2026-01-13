#include "test.h"
#include "lexer/token.h"
#include "utils/vector.h"

static void free_tokens(Vector *tokens) {
  for (size_t i = 0; i < tokens->size; i++) {
    Token *tok = (Token *)vec_get(tokens, i);
    if (tok) {
      if (tok->token) free((void*)tok->token);
      free(tok);
    }
  }
  vec_free(tokens);
}

static bool expect_tokens(const char *input,
                          const TokenType *types,
                          const char *const *lexemes,
                          size_t count) {
  Vector v = tokenize(input, 1);
  if (v.size != count) {
    free_tokens(&v);
    return false;
  }
  for (size_t i = 0; i < count; i++) {
    Token *t = (Token *)vec_get(&v, i);
    if (!t) { free_tokens(&v); return false; }
    if (t->type != types[i]) { free_tokens(&v); return false; }
    if (strcmp(t->token, lexemes[i]) != 0) { free_tokens(&v); return false; }
  }
  free_tokens(&v);
  return true;
}

bool tokenizer_test(void) {
  // Case 1: let x = 123 + 4.5;
  {
    const char *input = "let x = 123 + 4.5;";
    const TokenType types[] = {
      TOK_LET, TOK_IDENTIFIER, TOK_EQUAL, TOK_LITERAL, TOK_PLUS, TOK_LITERAL, TOK_SEMICOLON
    };
    const char *lexemes[] = {
      "let", "x", "=", "123", "+", "4.5", ";"
    };
    if (!expect_tokens(input, types, lexemes, 7)) return false;
  }

  // Case 2: print("hi")
  {
    const char *input = "print(\"hi\")";
    const TokenType types[] = {
      TOK_IDENTIFIER, TOK_LEFT_PARENTHESIS, TOK_STRING, TOK_RIGHT_PARENTHESIS
    };
    const char *lexemes[] = {
      "print", "(", "\"hi\"", ")"
    };
    if (!expect_tokens(input, types, lexemes, 4)) return false;
  }

  // Case 3: if a >= 10 && b != 0 { }
  {
    const char *input = "if a >= 10 && b != 0 { }";
    const TokenType types[] = {
      TOK_IF, TOK_IDENTIFIER, TOK_GREATER_EQUAL, TOK_LITERAL, TOK_AND, TOK_IDENTIFIER, TOK_BANG_EQUAL, TOK_LITERAL, TOK_LEFT_BRACE, TOK_RIGHT_BRACE
    };
    const char *lexemes[] = {
      "if", "a", ">=", "10", "&&", "b", "!=", "0", "{", "}"
    };
    if (!expect_tokens(input, types, lexemes, 10)) return false;
  }

  return true;
}

bool tokenizer_error_cases_test(void) {
  // Unterminated string: opening quote without closing
  {
    const char *input = "\"hi";
    const TokenType types[] = { TOK_STRING };
    const char *lexemes[] = { "\"hi" };
    if (!expect_tokens(input, types, lexemes, 1)) return false;
  }

  // Invalid literal: multiple dots
  {
    const char *input = "12.34.56";
    const TokenType types[] = { TOK_LITERAL };
    const char *lexemes[] = { "12.34.56" };
    if (!expect_tokens(input, types, lexemes, 1)) return false;
  }

  // Invalid literal: only dots
  {
    const char *input = "...";
    const TokenType types[] = { TOK_LITERAL };
    const char *lexemes[] = { "..." };
    if (!expect_tokens(input, types, lexemes, 1)) return false;
  }

  // Mixed number and letters: splits into literal then identifier
  {
    const char *input = "12a3";
    const TokenType types[] = { TOK_LITERAL, TOK_IDENTIFIER };
    const char *lexemes[] = { "12", "a3" };
    if (!expect_tokens(input, types, lexemes, 2)) return false;
  }

  // Unrecognized character is skipped
  {
    const char *input = "x @ y";
    const TokenType types[] = { TOK_IDENTIFIER, TOK_IDENTIFIER };
    const char *lexemes[] = { "x", "y" };
    if (!expect_tokens(input, types, lexemes, 2)) return false;
  }

  // Single unknown character only: produces no tokens
  {
    const char *input = "@";
    if (!expect_tokens(input, NULL, NULL, 0)) return false;
  }

  return true;
}

bool tokenizer_edge_cases_test(void) {
  // Empty input -> no tokens
  {
    const char *input = "";
    if (!expect_tokens(input, NULL, NULL, 0)) return false;
  }

  // Whitespace only -> no tokens
  {
    const char *input = "  \t  \n";
    if (!expect_tokens(input, NULL, NULL, 0)) return false;
  }

  // Identifier with underscores and digits
  {
    const char *input = "foo_bar123";
    const TokenType types[] = { TOK_IDENTIFIER };
    const char *lexemes[] = { "foo_bar123" };
    if (!expect_tokens(input, types, lexemes, 1)) return false;
  }

  // Numeric edge cases: leading/trailing dot
  {
    const char *input = ".5 12.";
    const TokenType types[] = { TOK_LITERAL, TOK_LITERAL };
    const char *lexemes[] = { ".5", "12." };
    if (!expect_tokens(input, types, lexemes, 2)) return false;
  }

  // Multi-character comparison operators
  {
    const char *input = "== <= >= !=";
    const TokenType types[] = { TOK_EQUAL_EQUAL, TOK_LESS_EQUAL, TOK_GREATER_EQUAL, TOK_BANG_EQUAL };
    const char *lexemes[] = { "==", "<=", ">=", "!=" };
    if (!expect_tokens(input, types, lexemes, 4)) return false;
  }

  // Adjacent punctuation
  {
    const char *input = "())";
    const TokenType types[] = { TOK_LEFT_PARENTHESIS, TOK_RIGHT_PARENTHESIS, TOK_RIGHT_PARENTHESIS };
    const char *lexemes[] = { "(", ")", ")" };
    if (!expect_tokens(input, types, lexemes, 3)) return false;
  }

  return true;
}

bool tokenizer_no_delimiter_expr(void) {
  {
    const char* input = "123+456";
    const TokenType types[] = { TOK_LITERAL, TOK_PLUS, TOK_LITERAL };
    const char* lexemes[] = { "123", "+", "456" };
    if (!expect_tokens(input, types, lexemes, 3)) return false;
    printf("passed 123+456\n");
  }

  { // "1<-123"
    const char* input = "1<-123";
    const TokenType types[] = { TOK_LITERAL, TOK_LESS, TOK_MINUS, TOK_LITERAL };
    const char* lexemes[] = { "1", "<", "-", "123" };
    if (!expect_tokens(input, types, lexemes, 4)) return false;
    printf("passed 1<-123\n");
  }

  { // true&&!!!false
    const char* input = "true&&!!!false";
    const TokenType types[] = { TOK_TRUE, TOK_AND, TOK_BANG, TOK_BANG, TOK_BANG, TOK_FALSE };
    const char* lexemes[] = { "true", "&&", "!", "!", "!", "false" };
    if (!expect_tokens(input, types, lexemes, 6)) return false;
    printf("passed true&&!!!false\n");
  }

  { // 1<=+-2
    const char* input = "1<=+-.2";
    const TokenType types[] = { TOK_LITERAL, TOK_LESS_EQUAL, TOK_PLUS, TOK_MINUS, TOK_LITERAL };
    const char* lexemes[] = { "1", "<=", "+", "-", ".2" };
    if (!expect_tokens(input, types, lexemes, 5)) return false;
    printf("passed 1<=+-2\n");
  }

  return true;
}