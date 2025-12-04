#include "test.h"
#include "scanners/test_scanner.h"

TestSuite get_scanner_suite(void) {
  static const TestCase cases[] = {
    { "literal_scanner", literal_scanner_test },
    { "word_scanner", word_scanner_test },
    { "string_scanner", string_scanner_test },
    { "op_scanner", op_scanner_test },
    { "punctuation_scanner", punctuation_scanner_test },
  };
  TestSuite s = { .name = "scanner", .cases = cases, .count = sizeof(cases) / sizeof(TestCase) };
  return s;
}
