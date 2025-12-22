#ifndef SUITE_H_
#define SUITE_H_

#include <stdbool.h>

typedef bool (*test_fn_t)(void);

typedef struct {
  const char *name;
  test_fn_t fn;
} TestCase;

typedef struct {
  const char *name;
  const TestCase *cases;
  int count;
} TestSuite;

int run_suite(TestSuite suite);
bool test_assert(bool condition, const char* message);

#endif  // SUITE_H_