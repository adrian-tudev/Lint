#ifndef SCANNER_H_
#define SCANNER_H_

#include <stdbool.h>
#include <stdint.h>

#include "token.h"

typedef struct {
  bool (*init_condition)(const char c);
  bool (*end_condition)(const char c);
  char* lexeme;
} TokenScanner;

void scan(TokenScanner scanner, const char* line, 
    uint32_t linePos, uint32_t row);

#endif // SCANNER_H_
