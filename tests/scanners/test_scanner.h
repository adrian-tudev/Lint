#ifndef TEST_SCANNER_H_
#define TEST_SCANNER_H_

#include "test.h"

bool literal_scanner_test(void);
bool word_scanner_test(void);
bool string_scanner_test(void);
bool op_scanner_test(void);
bool punctuation_scanner_test(void);

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

// accessors for scanners available via test.h includes

#endif // TEST_SCANNER_H_