#ifndef TEST_TOKEN_H_
#define TEST_TOKEN_H_

#include "test.h"

// tokenizer tests
bool tokenizer_test(void);
bool tokenizer_error_cases_test(void);
bool tokenizer_edge_cases_test(void);

TestSuite get_token_suite(void) {
  static const TestCase cases[] = {
    { "tokenize_general_cases", tokenizer_test },
    { "tokenize_error_cases", tokenizer_error_cases_test },
    { "tokenize_edge_cases", tokenizer_edge_cases_test },
  };
  TestSuite s = { .name = "token", .cases = cases, .count = sizeof(cases) / sizeof(TestCase) };
  return s;
}

#endif // TEST_TOKEN_H_