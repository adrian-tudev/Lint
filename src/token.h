#ifndef TOKEN_H_
#define TOKEN_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// auxiliary types and functions
#include "lexer/token_aux.h"
#include "utils/vector.h"

#define TOKEN_TYPE_COUNT INVALID + 1

typedef struct Token {
  // error info
  uint32_t row, column;

  // only for numerical literals
  double literal;

  const char* token;
  TokenType type;
} Token;

Vector tokenize(const char* line, const uint32_t row);
void print_token(Token* token);

#endif  // TOKEN_H_
