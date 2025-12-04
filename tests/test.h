#ifndef TEST_H_
#define TEST_H_

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "scanner.h"
#include "utils/string_utils.h"

bool split_string_test(void);
bool word_scanner_test(void);
void runTests(void);

// accessors for scanners defined in the implementation
extern const Scanner word_scanner;
extern const Scanner literal_scanner;
extern const Scanner string_scanner;
extern const Scanner op_scanner;
extern const Scanner punctuation_scanner;
extern const Scanner new_scanner; // Example of an additional scanner

// additional scanner tests
bool literal_scanner_test(void);
bool string_scanner_test(void);
bool op_scanner_test(void);
bool punctuation_scanner_test(void);

#endif // TEST_H_
