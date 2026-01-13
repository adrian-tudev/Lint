#ifndef TOKEN_AUX_H_
#define TOKEN_AUX_H_

#include <stdlib.h>
#include <string.h>

typedef enum TokenType {
  // Single character
  TOK_LEFT_PARENTHESIS, TOK_RIGHT_PARENTHESIS, TOK_LEFT_BRACE, TOK_RIGHT_BRACE,
  TOK_MINUS, TOK_PLUS, TOK_STAR, TOK_SLASH, TOK_SEMICOLON, TOK_QUOTES, TOK_COMMA, TOK_DOT, 

  // Comparison
  TOK_BANG, TOK_BANG_EQUAL,
  TOK_EQUAL, TOK_EQUAL_EQUAL,
  TOK_GREATER, TOK_GREATER_EQUAL,
  TOK_LESS, TOK_LESS_EQUAL,
  
  // Literals
  TOK_STRING, TOK_LITERAL, TOK_IDENTIFIER, 

  // Keywords
  TOK_AND, TOK_OR, TOK_IF, TOK_ELIF, TOK_ELSE, TOK_TRUE, TOK_FALSE, TOK_FUNCTION, TOK_FOR, TOK_WHILE,
  TOK_RETURN, TOK_LET,

  TOK_INVALID
} TokenType;

typedef struct {
  const char* lexeme;
  TokenType type;
} LookupEntry;

TokenType lookup_token_type(const char* str);
const char* token_type_name(TokenType type);

#endif // TOKEN_AUX_H_
