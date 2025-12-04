#include "test.h"

bool literal_scanner_test(void) {
  // digits
  const char* l1 = "1234";
  char* a = scan(literal_scanner, l1, 0);
  bool ok = strcmp(a, "1234") == 0; free(a); if (!ok) return false;

  // decimals starting with dot and containing digits
  const char* l2 = ".4567 ";
  char* b = scan(literal_scanner, l2, 0);
  ok = strcmp(b, ".4567") == 0; free(b); if (!ok) return false;

  // mixed: number followed by punctuation
  const char* l3 = "42,foo";
  char* c = scan(literal_scanner, l3, 0);
  ok = strcmp(c, "42") == 0; free(c); if (!ok) return false;

  // non-literal start should not initialize literal_scanner; simulate by starting inside
  const char* l4 = "abc123";
  char* d = scan(literal_scanner, l4, 3); // starts at '1'
  ok = strcmp(d, "123") == 0; free(d); if (!ok) return false;

  return true;
}
