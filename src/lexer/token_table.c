#include "token_table.h"

#include "utils/string_utils.h"

static const char* token_names[];
static LookupEntry token_lookup[];

TokenType lookup_token_type(const char* str) {
  for (size_t i = 0; token_lookup[i].lexeme != NULL; i++) {
    if (strcmp(token_lookup[i].lexeme, str) == 0)
      return token_lookup[i].type;
  }
  return TOK_INVALID;
}

// Returns the string representation of a TokenType
const char* token_type_name(TokenType type) { 
  return token_names[type]; 
}

// for debugging purposes
static const char* token_names[] = {
  "TOK_LEFT_PARENTHESIS", "TOK_RIGHT_PARENTHESIS", "TOK_LEFT_BRACE", "TOK_RIGHT_BRACE",
  "TOK_MINUS", "TOK_PLUS", "TOK_STAR", "TOK_SLASH", "TOK_SEMICOLON", "TOK_QUOTES", "TOK_COMMA", "TOK_DOT",

  "TOK_BANG", "TOK_BANG_EQUAL",
  "TOK_EQUAL", "TOK_EQUAL_EQUAL",
  "TOK_GREATER", "TOK_GREATER_EQUAL",
  "TOK_LESS", "TOK_LESS_EQUAL",

  "TOK_STRING", "TOK_LITERAL", "TOK_IDENTIFIER",

  "TOK_AND", "TOK_OR", "TOK_IF", "TOK_ELIF", "TOK_ELSE", "TOK_TRUE", "TOK_FALSE",
  "TOK_FUNCTION", "TOK_FOR", "TOK_WHILE", "TOK_RETURN", "TOK_LET",

  "TOK_INVALID"
};

static LookupEntry token_lookup[] = {
    // Single character tokens
    { "(", TOK_LEFT_PARENTHESIS },   { ")", TOK_RIGHT_PARENTHESIS },
    { "{", TOK_LEFT_BRACE },         { "}", TOK_RIGHT_BRACE },
    { "-", TOK_MINUS },              { "+", TOK_PLUS },
    { "*", TOK_STAR },               { "/", TOK_SLASH },
    { ";", TOK_SEMICOLON },          { "\"", TOK_QUOTES },
    { ",", TOK_COMMA },              { ".", TOK_DOT },

    // Boolean / arithmetic operators
    { "&&", TOK_AND }, { "||", TOK_OR },

    // Comparison operators
    { "!", TOK_BANG },         { "!=", TOK_BANG_EQUAL },
    { "=", TOK_EQUAL },        { "==", TOK_EQUAL_EQUAL },
    { ">", TOK_GREATER },      { ">=", TOK_GREATER_EQUAL },
    { "<", TOK_LESS },         { "<=", TOK_LESS_EQUAL },

    // Keywords
    { "if", TOK_IF },     { "elif", TOK_ELIF }, { "else", TOK_ELSE },
    { "true", TOK_TRUE }, { "false", TOK_FALSE },
    { "fn", TOK_FUNCTION },
    { "for", TOK_FOR },   { "while", TOK_WHILE },
    { "ret", TOK_RETURN },
    { "let", TOK_LET },

    // Literals handled separately by scanner
    // TOK_STRING, TOK_LITERAL, TOK_IDENTIFIER discovered dynamically

    { NULL, TOK_INVALID }
};
