#include "token.h"

#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#include "scanner.h"
#include "utils/string_utils.h"

// FORWARDING
// =============================================================================
static TokenType lookup_token_type(const char* str);
static const char* token_type_name(TokenType type);

static bool is_valid_string(const char* str);
static bool is_valid_literal(const char* str);

static const char* token_names[];

// LOCAL TYPES
// =============================================================================
typedef struct {
  const char* lexeme;
  TokenType type;
} LookupEntry;

static LookupEntry token_lookup[];

// PUBLIC FUNCTIONS
// =============================================================================

/*
 * Tokenize searches for tokens in the following order:
 * 1. Keywords & Identifiers 
 * 2. Literals
 * 3. Strings
 * 4. Operators
 * 5. Punctuation
 */
Vector tokenize(const char* line, uint32_t row) {
  Vector tokens;
  vec_init(&tokens);

  size_t len = strlen(line);

  for (size_t i = 0; i < len; i++) {
    if (is_delimiter(line[i], NULL)) continue;

    Scanner* scanner = NULL;
    // scanners[] is defined in scanner.c; iterate until a matching init_condition is found.
    for (size_t j = 0; scanners[j] != NULL; j++) {
      if (scanners[j]->init_condition(line[i])) {
        scanner = (Scanner*)scanners[j];
        break;
      }
    }

    if (scanner == NULL) {
      printf("No suitable scanner found! (unrecognized starting character)\n");
      printf("at column %lu, row %d\n", i, row);
      continue;
    }

    const char* lexeme = scan(*scanner, line, i);

    Token* tok = malloc(sizeof(Token));
    *tok = (Token){ .type = lookup_token_type(lexeme), 
      .token = lexeme, .column = i, .row = row };

    // valid token found directly
    if (tok->type != INVALID) goto insert_token;

    // handle strings, literals, identifiers dynamically
    if (scanner == &string_scanner) {
      if (!is_valid_string(lexeme)) {
        fprintf(stderr, "Error: Unterminated string at row %u, col %u\n", row, i);
      }
      tok->type = STRING;
    } else if (scanner == &literal_scanner) {
      if (!is_valid_literal(lexeme)) {
        fprintf(stderr, "Error: Invalid literal format at row %u, col %u\n", row, i);
      }
      tok->type = LITERAL;
    } else if (scanner == &word_scanner) {
      tok->type = IDENTIFIER;
    }
    
insert_token:
    assert(tok->token != NULL);
    vec_push(&tokens, tok);
    i += strlen(tok->token) - 1;
  }
  return tokens;
}

void print_token(Token* token) {
  printf("Token(type=%s, lexeme=%s, row=%u, col=%u)\n",
    token_type_name(token->type),
    token->token,
    token->row,
    token->column
  );
}

// PRIVATE FUNCTIONS
// =============================================================================

static bool is_valid_string(const char* str) {
  size_t len = strlen(str);
  if (len < 2) return false;
  if (str[0] != '"' || str[len - 1] != '"') return false;
  return true;
}

static bool is_valid_literal(const char* str) {
  size_t len = strlen(str);
  bool dot_found = false;
  for (size_t i = 0; i < len; i++) {
    if (str[i] == '.') {
      if (dot_found) return false; // multiple dots not allowed
      dot_found = true;
    } else if (!isdigit((unsigned char)str[i])) {
      return false; // invalid character found
    }
  }
  return true;
}

static TokenType lookup_token_type(const char* str) {
  for (size_t i = 0; token_lookup[i].lexeme != NULL; i++) {
    if (strcmp(token_lookup[i].lexeme, str) == 0) {
      return token_lookup[i].type;
    }
  }
  return INVALID; // default fallback when not recognized
}

static const char* token_type_name(TokenType type) {
  return token_names[type];
}

// PRIVATE VARIABLES
// =============================================================================

/* Single + multi-char operators, punctuation, keywords */
static LookupEntry token_lookup[] = {
    // Single character tokens
    { "(", LEFT_PARENTHESIS },   { ")", RIGHT_PARENTHESIS },
    { "{", LEFT_BRACE },         { "}", RIGHT_BRACE },
    { "-", MINUS },              { "+", PLUS },
    { "*", STAR },               { "/", SLASH },
    { ";", SEMICOLON },          { "\"", QUOTES },
    { ",", COMMA },              { ".", DOT },

    // Boolean / arithmetic operators
    { "and", AND }, { "or", OR }, { "not", NOT },

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

// for debugging purposes
static const char* token_names[] = {
  "LEFT_PARENTHESIS", "RIGHT_PARENTHESIS", "LEFT_BRACE", "RIGHT_BRACE",
  "MINUS", "PLUS", "STAR", "SLASH", "SEMICOLON", "QUOTES", "COMMA", "DOT",

  "BANG", "BANG_EQUAL",
  "EQUAL", "EQUAL_EQUAL",
  "GREATER", "GREATER_EQUAL",
  "LESS", "LESS_EQUAL",

  "STRING", "LITERAL", "IDENTIFIER",

  "AND", "OR", "NOT", "IF", "ELIF", "ELSE", "TRUE", "FALSE",
  "FUNCTION", "FOR", "WHILE", "RETURN", "LET",

  "INVALID"
};
