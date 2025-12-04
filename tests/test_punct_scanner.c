#include "test.h"

bool punctuation_scanner_test(void) {
  const char* p1 = "(){}.,";
  char* a = scan(punctuation_scanner, p1, 0);
  bool ok = strcmp(a, "(") == 0; free(a); if (!ok) return false;

  char* b = scan(punctuation_scanner, p1, 1);
  ok = strcmp(b, ")") == 0; free(b); if (!ok) return false;

  char* c = scan(punctuation_scanner, p1, 2);
  ok = strcmp(c, "{") == 0; free(c); if (!ok) return false;

  char* d = scan(punctuation_scanner, p1, 3);
  ok = strcmp(d, "}") == 0; free(d); if (!ok) return false;

  char* e = scan(punctuation_scanner, p1, 4);
  ok = strcmp(e, ".") == 0; free(e); if (!ok) return false;

  char* f = scan(punctuation_scanner, p1, 5);
  ok = strcmp(f, ",") == 0; free(f); if (!ok) return false;

  return true;
}
