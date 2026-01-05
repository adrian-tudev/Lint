#include "token_table.h"

static const char* token_names[];
static LookupEntry token_lookup[];

TokenType lookup_token_type(const char* str) {
  for (size_t i = 0; token_lookup[i].lexeme != NULL; i++) {
    if (strcmp(token_lookup[i].lexeme, str) == 0) {
      return token_lookup[i].type;
    }
  }
  return INVALID; // default fallback when not recognized
}

// Returns the string representation of a TokenType
const char* token_type_name(TokenType type) { 
  return token_names[type]; 
}

// for debugging purposes
static const char* token_names[] = {
  "LEFT_PARENTHESIS", "RIGHT_PARENTHESIS", "LEFT_BRACE", "RIGHT_BRACE",
  "MINUS", "PLUS", "STAR", "SLASH", "SEMICOLON", "QUOTES", "COMMA", "DOT",

  "BANG", "BANG_EQUAL",
  "EQUAL", "EQUAL_EQUAL",
  "GREATER", "GREATER_EQUAL",
  "LESS", "LESS_EQUAL",

  "STRING", "LITERAL", "IDENTIFIER",

  "AND", "OR", "IF", "ELIF", "ELSE", "TRUE", "FALSE",
  "FUNCTION", "FOR", "WHILE", "RETURN", "LET",

  "INVALID"
};

static LookupEntry token_lookup[] = {
    // Single character tokens
    { "(", LEFT_PARENTHESIS },   { ")", RIGHT_PARENTHESIS },
    { "{", LEFT_BRACE },         { "}", RIGHT_BRACE },
    { "-", MINUS },              { "+", PLUS },
    { "*", STAR },               { "/", SLASH },
    { ";", SEMICOLON },          { "\"", QUOTES },
    { ",", COMMA },              { ".", DOT },

    // Boolean / arithmetic operators
    { "&&", AND }, { "||", OR },

    // Comparison operators
    { "!", BANG },         { "!=", BANG_EQUAL },
    { "=", EQUAL },        { "==", EQUAL_EQUAL },
    { ">", GREATER },      { ">=", GREATER_EQUAL },
    { "<", LESS },         { "<=", LESS_EQUAL },

    // Keywords
    { "if", IF },     { "elif", ELIF }, { "else", ELSE },
    { "true", TRUE }, { "false", FALSE },
    { "fn", FUNCTION },
    { "for", FOR },   { "while", WHILE },
    { "ret", RETURN },
    { "let", LET },

    // Literals handled separately by scanner
    // STRING, LITERAL, IDENTIFIER discovered dynamically

    { NULL, INVALID }
};
