#include "test.h"

bool word_scanner_test(void) {
  // Test 1: simple words separated by space
  const char* line1 = "hello world";
  char* lex1 = scan(word_scanner, line1, 0);
  bool ok = strcmp(lex1, "hello") == 0;
  free(lex1);
  if (!ok) return false;

  char* lex2 = scan(word_scanner, line1, 6);
  ok = strcmp(lex2, "world") == 0;
  free(lex2);
  if (!ok) return false;

  // Test 2: identifier with underscore and digits
  const char* line2 = "_var123=4";
  char* lex3 = scan(word_scanner, line2, 0);
  ok = strcmp(lex3, "_var123") == 0;
  free(lex3);
  if (!ok) return false;

  // Test 3: word followed by punctuation
  const char* line3 = "foo,bar";
  char* lex4 = scan(word_scanner, line3, 0);
  ok = strcmp(lex4, "foo") == 0;
  free(lex4);
  if (!ok) return false;

  // Test 4: starting at non-zero position inside word
  const char* line4 = "prefixIdentifier rest";
  // start at 6 -> 'I' of Identifier
  char* lex5 = scan(word_scanner, line4, 6);
  ok = strcmp(lex5, "Identifier") == 0;
  free(lex5);
  return ok;
}
