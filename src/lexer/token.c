#include "lexer/token.h"

#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#include "lexer/scanner.h"
#include "utils/error.h"
#include "utils/string_utils.h"

// =====================
// Forward Declarations
// =====================

static Scanner* get_scanner_from_prefix(const char prefix);
static Token* token_from_lexeme(const char* lexeme, Scanner* scanner, uint32_t row, size_t i);

// =====================
// Public Functions
// =====================

/*
Get suitable scanner, scan lexeme until scanner end condition and construct token[s] from given lexeme.
*/
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
    Token* tok = token_from_lexeme(lexeme, scanner, row, i);

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

// =====================
// Private Functions
// =====================

static bool is_valid_string(const char* str);
static bool is_valid_literal(const char* str);

static Scanner* get_scanner_from_prefix(const char prefix) {
  for (size_t i = 0; scanners[i] != NULL; i++) {
    Scanner* scanner = (Scanner*)scanners[i];
    if (scanner->init_condition(prefix)) return scanner;
  }
  return NULL;
}

static void resolve_token(const Scanner* scanner, const char* lexeme, Token* tok, const uint32_t row, const size_t i) {
  if (scanner == &string_scanner) {
    if (!is_valid_string(lexeme)) {
      error_log("Unterminated string at row %u, col %zu\n", row, i);
    }
    tok->type = STRING;
  } else if (scanner == &literal_scanner) {
    if (!is_valid_literal(lexeme)) {
      error_log("Invalid literal format at row %u, col %zu\n", row, i);
    }
    // TODO: handle ascii to float conversion failure
    tok->literal = atof(lexeme);
    tok->type = LITERAL;
  } else if (scanner == &word_scanner) {
    // word scanner is guaranteed to produce valid identifiers
    tok->type = IDENTIFIER;
  } else if (scanner == &op_scanner) {
    // handle composite operators by trying different substrings
    size_t len = strlen(lexeme);
    for (int i = len - 1; i >= 0; i--) {
      const char* op_prefix = substring(lexeme, 0, i + 1);
      TokenType type = lookup_token_type(op_prefix);
      if (type != INVALID) {
        tok->token = op_prefix;
        tok->type = type;
        free((void*)lexeme);
        return;
      } else {
        free((void*)op_prefix);
      }
    }
    // no valid operator found
    error_log("Invalid operator at row %u, col %zu\n", row, i);
  } else {
    error_log("No suitable scanner for character at row %u, col %zu (invalid token)\n", row, i);
  }
}

static Token* token_from_lexeme(const char* lexeme, Scanner* scanner, 
    const uint32_t row, const size_t i) {
  Token* tok = malloc(sizeof(Token));
  tok->type = lookup_token_type(lexeme);
  tok->token = lexeme;
  tok->column = i;
  tok->row = row;

  // resolve strings, literals, identifiers and composite operators dynamically 
  // (defaulted to INVALID since we can't be sure yet)
  if (tok->type == INVALID) {
    resolve_token(scanner, lexeme, tok, row, i);
  }
  return tok;
}

static bool is_valid_string(const char* str) {
  size_t len = strlen(str);
  if (len < 2) return false;
  return str[0] == '"' && str[len - 1] == '"';
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

