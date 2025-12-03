#include "token.h"

#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include "utils/string_utils.h"

static void read_word(Token* token, const char* line, int pos);
static void read_literal(Token* token, const char* line, int pos);
static void read_string(Token* token, const char* line, int pos);
static void read_operator(Token* token, const char* line, int pos);
static void read_punctuation(Token* token, const char* line, int pos);

static const char* token_names[];
static const char operators[] = {'+', '-', '/', '*', '=', '!', '>', '<'};
static const char punctuators[] = {'(', ')', '{', '}', ',',};

/*
 * Tokenize searches for tokens in the following order:
 * 1. Keywords & Identifiers 
 * 2. Literals
 * 3. Strings
 * 4. Operator & Punctuation
 */
Vector tokenize(const char* line, uint32_t row) {
  Vector tokens;
  vec_init(&tokens);

  size_t len = strlen(line);
  bool valid_syntax = true;

  for (size_t i = 0; i < len; i++) {
    if (is_delimiter(line[i], NULL)) continue;

    Token* tok = malloc(sizeof(Token));
    tok->token = NULL;
    tok->column = i;
    tok->row = row;

    if (isalpha(line[i]) || line[i] == '_')      read_word(tok, line, i);
    else if (isdigit(line[i]) || line[i] == '.') read_literal(tok, line, i);
    else if (line[i] == '"')                     read_string(tok, line, i);
    else if (char_in_arr(line[i], operators))    read_operator(tok, line, i);
    else if (char_in_arr(line[i], punctuators))  read_punctuation(tok, line, i);
    else tok->type = INVALID;

    // set error flag and continue
    if (tok->type == INVALID) {
      printf("Invalid character found while tokenizing!\n");
      valid_syntax = false;
      free(tok);
      continue;
    }

    assert(tok->token != NULL);

    vec_push(&tokens, tok);
    i += strlen(tok->token) - 1;
  }
  return tokens;
}

static inline char peek_next(const char *s, int pos) {
    if (!s || pos < 0) return '\0';

    size_t len = strlen(s);
    if ((size_t)pos + 1 >= len) return '\0';

    return s[pos + 1];
}

// searches for keywords and identifiers
static void read_word(Token* tok, const char* line, int linePos) {
  size_t i = linePos;

  while (isalnum(peek_next(line, i)) || peek_next(line, i) == '_') i++;

  int len = i - linePos + 1;
  char* token_str = substring(line, linePos, len);

  // TODO use lookup table
  if      (strcmp(token_str, "fn") == 0)    tok->type = FUNCTION;
  else if (strcmp(token_str, "for") == 0)   tok->type = FOR;
  else if (strcmp(token_str, "while") == 0) tok->type = WHILE;
  else if (strcmp(token_str, "ret") == 0)   tok->type = RETURN;
  else if (strcmp(token_str, "let") == 0)   tok->type = LET;
  else if (strcmp(token_str, "true") == 0)  tok->type = TRUE;
  else if (strcmp(token_str, "false") == 0) tok->type = FALSE;
  else if (strcmp(token_str, "and") == 0)   tok->type = AND;
  else if (strcmp(token_str, "or") == 0)    tok->type = OR;
  else if (strcmp(token_str, "not") == 0)   tok->type = NOT;
  else if (strcmp(token_str, "if") == 0)    tok->type = IF;
  else if (strcmp(token_str, "elif") == 0)  tok->type = ELIF;
  else if (strcmp(token_str, "else") == 0)  tok->type = ELSE;
  else                                      tok->type = IDENTIFIER;
  
  tok->token = token_str;
}

static void read_literal(Token* tok, const char* line, int linePos) {
  size_t i = linePos;

  while (isdigit(peek_next(line, i)) || peek_next(line, i) == '.') i++;

  size_t len = i - linePos + 1;
  char* token_str = substring(line, linePos, len);

  uint32_t dots = 0;
  for (size_t j = 0; j < len; j++) {
    if (token_str[j] == '.') dots++;
  }

  if (dots > 1) {
    tok->type = INVALID;
    fprintf(stderr, "Error while reading literal (too many dots)\n");
    free(token_str);
    return;
  } 

  tok->token = token_str;
  tok->type = LITERAL;
}

static void read_string(Token* tok, const char* line, int linePos) {
  size_t i = linePos + 1;
  while (peek_next(line, i) != '"' && (peek_next(line, i) != '\0')) i++;

  i++;
  if (line[i] != '"') {
    tok->type = INVALID;
    tok->column = i;
    fprintf(stderr, "Error while reading string literal (no ending string quote)\n");
    return;
  }
  size_t len = i - linePos + 1;
  char* token_str = substring(line, linePos, len);
  tok->token = token_str;
  tok->type = STRING;
}

static void read_operator(Token* tok, const char* line, int linePos) {
  size_t i = linePos;

  while (char_in_arr(peek_next(line, i), operators)) i++;

  size_t len = i - linePos + 1;
  char* op_string = substring(line, linePos, len);

  if (strcmp(op_string, "!") == 0) tok->type = BANG;

  tok->token = op_string;
}

static void read_punctuation(Token* tok, const char* line, int linePos) {
  tok->type = INVALID;
}

static const char* token_type_name(TokenType type) {
  return token_names[type];
}

static TokenType str_to_token(const char* tok_str) {
  for (size_t i = 0; i < TOKEN_TYPE_COUNT; i++) {
    if (strcmp(tok_str, token_names[i]) == 0) {
      return (TokenType)i;
    }
  }
  return INVALID;
}

void print_token(Token* token) {
    printf("Token(type=%s, lexeme=%s, row=%u, col=%u)\n",
        token_type_name(token->type),
        token->token,
        token->row,
        token->column
    );
}

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

