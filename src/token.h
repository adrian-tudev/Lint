#ifndef TOKEN_H_
#define TOKEN_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/vector.h"

#define TOKEN_TYPE_COUNT INVALID + 1

typedef enum {
  // Single character
  LEFT_PARENTHESIS, RIGHT_PARENTHESIS, LEFT_BRACE, RIGHT_BRACE,
  MINUS, PLUS, STAR, SLASH, SEMICOLON, QUOTES, COMMA, DOT, 

  // Comparison
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,
  
  // Literals
  STRING, LITERAL, IDENTIFIER, 

  // Keywords
  AND, OR, NOT, IF, ELIF, ELSE, TRUE, FALSE, FUNCTION, FOR, WHILE,
  RETURN, LET,

  INVALID
} TokenType;

typedef struct {
  // error info
  uint32_t row, column;

  // only for numerical literals
  uint32_t literal;

  char* token;
  TokenType type;
} Token;

Vector tokenize(const char* line, uint32_t row);
void print_token(Token* token);

#endif  // TOKEN_H_
