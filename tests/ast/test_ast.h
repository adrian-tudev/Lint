#ifndef TEST_AST_H_
#define TEST_AST_H_

#include "test.h"

bool num_expr(void);
bool bool_expr(void);

TestSuite get_ast_suite(void) {
  static const TestCase cases[] = {
    { "num_expr", num_expr },
    { "bool_expr", bool_expr },
  };
  TestSuite s = { .name = "ast", .cases = cases, .count = (int)(sizeof(cases) / sizeof(TestCase)) };
  return s;
}

#endif // TEST_AST_H_
