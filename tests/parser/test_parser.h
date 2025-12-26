#ifndef TEST_PARSER_H_
#define TEST_PARSER_H_

#include "test.h"

bool numerical_expr_parsing(void);

TestSuite get_parser_suite(void) {
  static const TestCase cases[] = {
    { "numerical_expr_parsing", numerical_expr_parsing },
  };
  TestSuite s = { .name = "parser", .cases = cases, .count = (int)(sizeof(cases) / sizeof(TestCase)) };
  return s;
}

#endif // TEST_PARSER_H_
