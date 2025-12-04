#include "test.h"
#include "token/test_token.h"

TestSuite get_token_suite(void) {
  static const TestCase cases[] = {
    {"tokenizer_general_cases", tokenizer_test},
    {"tokenizer_error_cases", tokenizer_error_cases_test},
    {"tokenizer_edge_cases", tokenizer_edge_cases_test},
  };
  TestSuite s = { .name = "token", .cases = cases, .count = sizeof(cases) / sizeof(TestCase) };
  return s;
}
