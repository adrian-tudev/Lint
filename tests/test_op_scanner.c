#include "test.h"

bool op_scanner_test(void) {
  // single operators separated by spaces to avoid merging into one long lexeme
  const char* o1 = "+ - * / = < > ! ";
  char* a = scan(op_scanner, o1, 0);
  bool ok = strcmp(a, "+") == 0; free(a); if (!ok) return false;

  char* b = scan(op_scanner, o1, 2);
  ok = strcmp(b, "-") == 0; free(b); if (!ok) return false;

  char* c = scan(op_scanner, o1, 4);
  ok = strcmp(c, "*") == 0; free(c); if (!ok) return false;

  char* d = scan(op_scanner, o1, 6);
  ok = strcmp(d, "/") == 0; free(d); if (!ok) return false;

  // multi-char operators should be recognized as two-character lexemes
  const char* o2 = "== != >= <=";
  char* e = scan(op_scanner, o2, 0);
  ok = strcmp(e, "==") == 0; free(e); if (!ok) return false;

  char* f = scan(op_scanner, o2, 3);
  ok = strcmp(f, "!=") == 0; free(f); if (!ok) return false;

  char* g = scan(op_scanner, o2, 6);
  ok = strcmp(g, ">=") == 0; free(g); if (!ok) return false;

  char* h = scan(op_scanner, o2, 9);
  ok = strcmp(h, "<=") == 0; free(h); if (!ok) return false;

  return true;
}
