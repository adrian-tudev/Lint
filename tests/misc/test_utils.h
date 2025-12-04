#ifndef TEST_UTILS_H_
// #define TEST_UTILS_H_

#include "test.h"

bool split_string_test(void);

TestSuite get_utils_suite(void) {
  static const TestCase cases[] = {
    { "split_string", split_string_test },
  };
  TestSuite s = { .name = "utils", .cases = cases, .count = sizeof(cases) / sizeof(TestCase) };
  return s;
}

#endif // TEST_UTILS_H_