#include "scanner.h"

#include <stdlib.h>
#include <string.h>

#include "utils/string_utils.h"

char* scan(TokenScanner scanner, const char* line, uint32_t linePos) {

  size_t i = linePos;
  size_t line_len = strlen(line);

  // skip the first character if include_begin is set
  if (scanner.include_begin && i < line_len) i++;

  while (i < line_len && !scanner.end_condition(line[i])) i++;

  size_t len = i - linePos;

  // For single-character tokens where end_condition applies at the start,
  // ensure length is at least 1.
  if (len == 0 && linePos < line_len && scanner.end_condition(line[linePos])) {
    len = 1;
  }
  if (scanner.include_end && i != line_len) len++;

  const char* lexeme = substring(line, linePos, len);
  return lexeme;
}
