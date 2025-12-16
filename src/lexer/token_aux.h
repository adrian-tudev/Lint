#ifndef TOKEN_AUX_H_
#define TOKEN_AUX_H_

#include <stdlib.h>
#include <string.h>

typedef enum TokenType {
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
  const char* lexeme;
  TokenType type;
} LookupEntry;

TokenType lookup_token_type(const char* str);
const char* token_type_name(TokenType type);

#endif // TOKEN_AUX_H_
