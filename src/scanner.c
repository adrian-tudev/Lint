#include "scanner.h"

#include <stdlib.h>
#include <string.h>

#include "utils/string_utils.h"

static inline char peek_next(const char *s, int pos);

void scan(TokenScanner scanner, const char* line, 
    uint32_t linePos, uint32_t row) {
  size_t i = linePos;
  while (!scanner.end_condition(peek_next(line, i))) i++;

  size_t len = i - linePos + 1;
  scanner.lexeme = substring(line, linePos, len);
}

static inline char peek_next(const char *s, int pos) {
  if (!s || pos < 0) return '\0';

  size_t len = strlen(s);
  if ((size_t)pos + 1 >= len) return '\0';

  return s[pos + 1];
}
