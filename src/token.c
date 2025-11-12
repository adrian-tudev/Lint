#include "token.h"

#include <stdbool.h>

static bool isDelimiter(const char);

// splits input at whitespaces
Token* tokenize(const char* line, int* out_count) {
  Token* tokens = NULL;
  int word_cnt = 0;
  int word_len = 0;

  size_t len = strlen(line);
  for (size_t i = 0; i < len; i++) {
    if (isDelimiter(line[i])) {
      if (word_len) {
        word_cnt++;
        word_len = 0;
      }
      continue;
    }
    word_len++;
  }
  // count the last word
  if (word_len) word_cnt++;

  tokens = (Token*) malloc(sizeof(Token) * word_cnt);
  *out_count = word_cnt;

  word_cnt = 0;
  word_len = 0;
  for (size_t i = 0; i < len; i++) {
    if (isDelimiter(line[i])) {
      if (word_len) {
        char* token = malloc(word_len + 1);
        memcpy(token, line + (i - word_len), word_len);
        token[word_len] = '\0';

        tokens[word_cnt] = (Token) { .token = token, .type = kLiteral };
        word_cnt++;
        word_len = 0;
      }
      continue;
    }
    word_len++;
  }

  if (word_len) {
    char* token = malloc(++word_len + 1);
    memcpy(token, line + (len - word_len), word_len);
    token[word_len] = '\0';

    tokens[++word_cnt] = (Token) { .token = token, .type = kLiteral };
  }

  return tokens;
}

static bool isLiteral(const char* s) {

  return true;
}

static const char delimiters[] = {' ', '\n', ';'};
static bool isDelimiter(const char c) {
  for (size_t i = 0; i < sizeof(delimiters) / sizeof(char); i++) {
    if (c == delimiters[i]) 
      return true;
  }

  return false;
}
