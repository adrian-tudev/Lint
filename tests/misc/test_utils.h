#ifndef TEST_UTILS_H_
// #define TEST_UTILS_H_

#include "test.h"

bool split_string_test(void);
bool test_hm_copy(void);
bool test_hm_get_keys(void);
bool test_value_copy(void);
bool test_hm_copy_with_functions(void);
bool test_callstack_init_and_free(void);
bool test_callstack_push_pop(void);
bool test_callstack_peek(void);
bool test_callstack_is_empty(void);
bool test_callstack_pop_empty(void);
bool test_callstack_grow(void);

TestSuite get_utils_suite(void) {
  static const TestCase cases[] = {
    { "split_string", split_string_test },
    { "hm_copy", test_hm_copy },
    { "hm_get_keys", test_hm_get_keys },
    { "value_copy", test_value_copy },
    { "hm_copy_with_functions", test_hm_copy_with_functions },
    { "callstack_init_and_free", test_callstack_init_and_free },
    { "callstack_push_pop", test_callstack_push_pop },
    { "callstack_peek", test_callstack_peek },
    { "callstack_is_empty", test_callstack_is_empty },
    { "callstack_pop_empty", test_callstack_pop_empty },
    { "callstack_grow", test_callstack_grow },
  };
  TestSuite s = { .name = "utils", .cases = cases, .count = sizeof(cases) / sizeof(TestCase) };
  return s;
}

#endif // TEST_UTILS_H_