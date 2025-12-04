#ifndef SCANNER_H_
#define SCANNER_H_

#include <stdbool.h>
#include <stdint.h>

#include "token.h"

typedef struct TokenScanner {
  bool include_begin;
  bool (*init_condition)(const char c);

  bool include_end;
  bool (*end_condition)(const char c);
} TokenScanner;

char* scan(TokenScanner scanner, const char* line, uint32_t linePos);

#endif // SCANNER_H_
