#ifndef TOKEN_H_
#define TOKEN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  kKeyword,
  kLiteral,
} TokenType;

typedef struct {
  char* token;
  TokenType type;
} Token;

Token* tokenize(const char* line, int* out_count);

#endif  // TOKEN_H_
