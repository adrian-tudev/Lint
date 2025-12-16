#include "token.h"

#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#include "lexer/scanner.h"
#include "utils/error.h"
#include "utils/string_utils.h"

// PRIVATE FUNCTION DECLARATIONS
static Scanner* get_scanner_from_prefix(const char prefix);
static Token* construct_token(const char* lexeme, Scanner* scanner, uint32_t row, size_t i);

// PUBLIC FUNCTIONS
// =============================================================================

Vector tokenize(const char* line, uint32_t row) {
  Vector tokens;
  vec_init(&tokens);

  size_t len = strlen(line);

  for (size_t i = 0; i < len; i++) {
    if (is_delimiter(line[i], NULL)) continue;

    Scanner* scanner = get_scanner_from_prefix(line[i]);
    if (scanner == NULL) {
      error_log("Unrecognized character at column %lu, row %d\n", i, row);
      continue;
    }

    const char* lexeme = scan(*scanner, line, i);

    // constructs token and validate if needed, handles errors internally
    Token* tok = construct_token(lexeme, scanner, row, i);

    vec_push(&tokens, tok);
    i += strlen(tok->token) - 1;
  }
  return tokens;
}

void print_token(Token* token) {
  const char* type_name = token_type_name(token->type);
  printf("Token(row=%u, col=%u, type=%s, lexeme=|%s|)\n",
         token->row, token->column, type_name, token->token);
}

// PRIVATE FUNCTIONS
// =============================================================================
static bool is_valid_string(const char* str);
static bool is_valid_literal(const char* str);

static Scanner* get_scanner_from_prefix(const char prefix ) {
  for (size_t j = 0; scanners[j] != NULL; j++) {
    Scanner* scanner = (Scanner*)scanners[j];
    if (scanner->init_condition(prefix)) {
      return scanner;
    }
  }
  return NULL;
}

static void validate_token(const Scanner* scanner, const char* lexeme, Token* tok, uint32_t row, size_t i) {
  if (scanner == &string_scanner) {
    if (!is_valid_string(lexeme)) {
      error_log("Unterminated string at row %u, col %zu\n", row, i);
    }
    tok->type = STRING;
  } else if (scanner == &literal_scanner) {
    if (!is_valid_literal(lexeme)) {
      error_log("Invalid literal format at row %u, col %zu\n", row, i);
    }
    tok->type = LITERAL;
  } else if (scanner == &word_scanner) {
    // word scanner is guaranteed to produce valid identifiers
    tok->type = IDENTIFIER;
  } else {
    assert(false && "Unhandled scanner type");
  }
}

static Token* construct_token(const char* lexeme, Scanner* scanner, const uint32_t row, const size_t i) {
  Token* tok = malloc(sizeof(Token));
  *tok = (Token){ .type = lookup_token_type(lexeme), 
    .token = lexeme, .column = i, .row = row };

  // handle strings, literals, identifiers dynamically (defaulted to INVALID since we don't know yet)
  if (tok->type == INVALID) {
    validate_token(scanner, lexeme, tok, row, i);
  }
  return tok;
}

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
    } else if (!isdigit(str[i])) {
      return false; // invalid character found
    }
  }
  return true;
}
