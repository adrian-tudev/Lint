#include "scanner.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "utils/string_utils.h"

char* scan(Scanner scanner, const char* line, uint32_t linePos) {

  size_t i = linePos;
  size_t line_len = strlen(line);

  // skip the first character if include_begin is set
  if (scanner.include_begin && i < line_len) i++;

  while (i < line_len && !scanner.end_condition(line[i])) i++;

  size_t len = i - linePos;

  // For single-character tokens where end_condition applies at the start,
  // ensure length is at least 1.
  if (len == 0 && linePos < line_len && scanner.end_condition(line[linePos]))
    len = 1;

  if (scanner.include_end && i != line_len) len++;

  const char* lexeme = substring(line, linePos, len);
  return (char*)lexeme;
}

// Local operator/punctuator tables (must match tokenization rules)
static const char operators[] = "+-*/=<>&|!";
static const char punctuators[] = "[](){}.,;:";

// Implement scanner condition functions here
static bool init_word(const char c) { return isalpha((unsigned char)c) || c == '_'; }
static bool end_word(const char c) { return !(isalnum((unsigned char)c) || c == '_'); }
static bool init_literal(const char c) { return isdigit((unsigned char)c) || c == '.'; }
static bool end_literal(const char c) { return !(isdigit((unsigned char)c) || c == '.'); }
static bool init_string(const char c) { return c == '"'; }
static bool end_string(const char c) { return c == '"' || c == '\0'; }
static bool init_op(const char c) { return char_in_arr(c, operators); }
static bool end_op(const char c) { return !char_in_arr(c, operators); }
static bool init_punctuator(const char c) { return char_in_arr(c, punctuators); }
static bool end_punctuator(const char c) { return char_in_arr(c, punctuators); }

// Define scanner instances here
const Scanner word_scanner = {
  .init_condition = init_word,
  .end_condition = end_word,
  .include_end = false,
  .include_begin = false,
};

const Scanner literal_scanner = {
  .init_condition = init_literal,
  .end_condition = end_literal,
  .include_end = false,
  .include_begin = false,
};

const Scanner string_scanner = {
  .init_condition = init_string,
  .end_condition = end_string,
  .include_end = true,
  .include_begin = true,
};

const Scanner op_scanner = {
  .init_condition = init_op,
  .end_condition = end_op,
  .include_end = false,
  .include_begin = false,
};

const Scanner punctuation_scanner = {
  .init_condition = init_punctuator,
  .end_condition = end_punctuator,
  .include_end = false,
  .include_begin = false,
};

const Scanner* scanners[] = {
  &word_scanner, &literal_scanner, &string_scanner,
  &op_scanner, &punctuation_scanner,
  NULL // No suitable scanner found
};
