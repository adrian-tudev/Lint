#ifndef TEST_UTILS_H_
// #define TEST_UTILS_H_

#include "test.h"

bool split_string_test(void);
bool test_hm_copy(void);
bool test_hm_get_keys(void);
bool test_value_copy(void);

TestSuite get_utils_suite(void) {
  static const TestCase cases[] = {
    { "split_string", split_string_test },
    { "hm_copy", test_hm_copy },
    { "hm_get_keys", test_hm_get_keys },
    { "value_copy", test_value_copy },
  };
  TestSuite s = { .name = "utils", .cases = cases, .count = sizeof(cases) / sizeof(TestCase) };
  return s;
}

#endif // TEST_UTILS_H_