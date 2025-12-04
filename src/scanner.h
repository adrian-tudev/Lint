#ifndef SCANNER_H_
#define SCANNER_H_

#include <stdbool.h>
#include <stdint.h>

#include "token.h"

typedef struct Scanner {
  bool include_begin;
  bool (*init_condition)(const char c);

  bool include_end;
  bool (*end_condition)(const char c);
} Scanner;

char* scan(Scanner scanner, const char* line, uint32_t linePos);

// Extern declarations for scanners
extern const Scanner* scanners[];
extern const Scanner word_scanner;
extern const Scanner literal_scanner;
extern const Scanner string_scanner;
extern const Scanner op_scanner;
extern const Scanner punctuation_scanner;

#endif // SCANNER_H_
