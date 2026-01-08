#ifndef TEST_EXECUTION_H_
#define TEST_EXECUTION_H_

#include "test.h"

// From test_if_stmt.c
bool test_if_true_execution(void);
bool test_if_false_execution(void);
bool test_if_no_else_execution(void);

TestSuite get_execution_suite(void) {
  static const TestCase cases[] = {
    { "test_if_true_execution", test_if_true_execution },
    { "test_if_false_execution", test_if_false_execution },
    { "test_if_no_else_execution", test_if_no_else_execution },
  };

  TestSuite suite = {
    .name = "Execution Tests",
    .cases = cases,
    .count = sizeof(cases) / sizeof(TestCase),
  };

  return suite;
}

#endif // TEST_EXECUTION_H_
