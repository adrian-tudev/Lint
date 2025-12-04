#include "test.h"

bool string_scanner_test(void) {
  // basic quoted string, include end should include closing quote
  const char* s1 = "\"hello\" rest";
  char* a = scan(string_scanner, s1, 0);
  bool ok = strcmp(a, "\"hello\"") == 0; free(a); if (!ok) return false;

  // string terminated at end of line
  const char* s2 = "\"world\"";
  char* b = scan(string_scanner, s2, 0);
  ok = strcmp(b, "\"world\"") == 0; free(b); if (!ok) return false;

  // start mid-line
  const char* s3 = "prefix \"abc\" suffix";
  char* c = scan(string_scanner, s3, 7);
  ok = strcmp(c, "\"abc\"") == 0; free(c); if (!ok) return false;

  // empty string
  const char* s4 = "\"\" nothing";
  char* d = scan(string_scanner, s4, 0);
  ok = strcmp(d, "\"\"") == 0; free(d); if (!ok) return false;

  // invalid string (no closing quote) should scan to end of line
  const char* s5 = "\"incomplete string";
  char* e = scan(string_scanner, s5, 0);
  ok = strcmp(e, "\"incomplete string") == 0; free(e); if (!ok) return false;

  return true;
}
