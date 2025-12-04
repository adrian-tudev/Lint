#ifndef TEST_SCANNER_H_
#define TEST_SCANNER_H_

#include "test.h"

bool literal_scanner_test(void);
bool word_scanner_test(void);
bool string_scanner_test(void);
bool op_scanner_test(void);
bool punctuation_scanner_test(void);

TestSuite get_scanner_suite(void);

// accessors for scanners available via test.h includes

#endif // TEST_SCANNER_H_