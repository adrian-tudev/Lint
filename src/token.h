#ifndef TOKEN_H_
#define TOKEN_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/vector.h"

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
  IDENTIFIER, STRING, NUMBER,

  // Keywords
  AND, OR, IF, ELIF, ELSE, TRUE, FALSE, FUNCTION, FOR, WHILE,
  RETURN, LET,

  INVALID
} TokenType;

typedef struct {
  char* token;
  TokenType type;
  uint32_t row, column;
  uint32_t literal;
} Token;

bool tokenize(const char* line, Vector* tokens);
void print_token(Token* token);

#endif  // TOKEN_H_
