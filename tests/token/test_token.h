#ifndef TEST_TOKEN_H_
#define TEST_TOKEN_H_

#include "test.h"

// tokenizer tests
bool tokenizer_test(void);
bool tokenizer_error_cases_test(void);
bool tokenizer_edge_cases_test(void);

TestSuite get_token_suite(void);

#endif // TEST_TOKEN_H_