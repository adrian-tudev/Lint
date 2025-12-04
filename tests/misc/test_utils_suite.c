#include "test.h"
#include "misc/test_utils.h"

TestSuite get_utils_suite(void) {
  static const TestCase cases[] = {
    { "split_string_test", split_string_test },
  };
  TestSuite s = { .name = "utils", .cases = cases, .count = sizeof(cases) / sizeof(TestCase) };
  return s;
}
