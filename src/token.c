#include "token.h"
#include <stdbool.h>
#include <ctype.h>
#include "utils/string_utils.h"

static void read_word(Token* token, const char* line, int pos);
static void read_literal(Token* token, const char* line, int pos);
static void read_string(Token* token, const char* line, int pos);
static void read_operator(Token* token, const char* line, int pos);
static void read_punctuation(Token* token, const char* line, int pos);
static bool char_in_arr(const char c, const char* arr);

static const char operators[] = {'+', '-', '/', '*', '=', '!', '>', '<'};
static const char punctuators[] = {'(', ')', '{', '}', ',',};


/*
 * Tokenize checks searches for tokens in the following order:
 * 1. Keywords & Identifiers 
 * 2. Literals
 * 3. Strings
 * 4. Operator & Punctuation
 */
bool tokenize(const char* line, Vector* tokens) {
  size_t len = strlen(line);
  bool valid_syntax = true;

  for (size_t i = 0; i < len; i++) {
    if (is_delimiter(line[i], NULL)) continue;

    Token* tok = malloc(sizeof(Token));

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

    vec_push(tokens, tok);
    i += strlen(tok->token);
  }

  return valid_syntax;
}

static inline char peek_next(const char* s, int pos) { 
  return s[pos] != '\0' ? s[pos + 1] : '\0'; 
}

// searches for keywords and identifiers
static void read_word(Token* tok, const char* line, int pos) {
  size_t i = pos;

  while (isalnum(peek_next(line, i)) || peek_next(line, i) == '_') i++;

  int len = i - pos + 1;
  char* token_str = malloc(len + 1);
  memcpy(token_str, line + pos, len);
  token_str[len] = '\0';

  // use lookup table instead?
  if      (strcmp(token_str, "fn") == 0)    tok->type = FUNCTION;
  else if (strcmp(token_str, "for") == 0)   tok->type = FOR;
  else if (strcmp(token_str, "while") == 0) tok->type = WHILE;
  else if (strcmp(token_str, "ret") == 0)   tok->type = RETURN;
  else if (strcmp(token_str, "let") == 0)   tok->type = LET;
  else if (strcmp(token_str, "true") == 0)  tok->type = TRUE;
  else if (strcmp(token_str, "false") == 0) tok->type = FALSE;
  else if (strcmp(token_str, "and") == 0)   tok->type = AND;
  else if (strcmp(token_str, "or") == 0)    tok->type = OR;
  else if (strcmp(token_str, "if") == 0)    tok->type = IF;
  else if (strcmp(token_str, "elif") == 0)  tok->type = ELIF;
  else if (strcmp(token_str, "else") == 0)  tok->type = ELSE;
  else                                      tok->type = IDENTIFIER;
  
  tok->token = token_str;
}

static void read_literal(Token* tok, const char* line, int linePos) {
  tok->type = INVALID;
}

static void read_string(Token* tok, const char* line, int linePos) {
  tok->type = INVALID;
}

static void read_operator(Token* tok, const char* line, int linePos) {
  tok->type = INVALID;
}

static void read_punctuation(Token* tok, const char* line, int linePos) {
  tok->type = INVALID;
}

static bool char_in_arr(const char c, const char* arr) {
  size_t len = sizeof(arr);
  for (size_t i = 0; i < len; i++) {
    if (c == arr[i]) return true;
  }
  return false;
}

static const char* token_type_name(TokenType type) {
    switch (type) {
        case LEFT_PARENTHESIS:   return "LEFT_PARENTHESIS";
        case RIGHT_PARENTHESIS:  return "RIGHT_PARENTHESIS";
        case LEFT_BRACE:         return "LEFT_BRACE";
        case RIGHT_BRACE:        return "RIGHT_BRACE";
        case MINUS:              return "MINUS";
        case PLUS:               return "PLUS";
        case STAR:               return "STAR";
        case SLASH:              return "SLASH";
        case SEMICOLON:          return "SEMICOLON";
        case QUOTES:             return "QUOTES";
        case COMMA:              return "COMMA";
        case DOT:                return "DOT";

        case BANG:               return "BANG";
        case BANG_EQUAL:         return "BANG_EQUAL";

        case EQUAL:              return "EQUAL";
        case EQUAL_EQUAL:        return "EQUAL_EQUAL";

        case GREATER:            return "GREATER";
        case GREATER_EQUAL:      return "GREATER_EQUAL";

        case LESS:               return "LESS";
        case LESS_EQUAL:         return "LESS_EQUAL";

        case IDENTIFIER:         return "IDENTIFIER";
        case STRING:             return "STRING";
        case NUMBER:             return "NUMBER";

        case AND:                return "AND";
        case OR:                 return "OR";
        case IF:                 return "IF";
        case ELIF:               return "ELIF";
        case ELSE:               return "ELSE";
        case TRUE:               return "TRUE";
        case FALSE:              return "FALSE";
        case FUNCTION:           return "FUNCTION";
        case FOR:                return "FOR";
        case WHILE:              return "WHILE";
        case RETURN:             return "RETURN";
        case LET:                return "LET";

        case INVALID:            return "INVALID";

        default:                 return "UNKNOWN_TOKEN";
    }
}

void print_token(Token* token) {
    printf("Token(type=%s, lexeme=\"%s\", row=%u, col=%u)\n",
        token_type_name(token->type),
        token->token,
        token->row,
        token->column
    );
}

