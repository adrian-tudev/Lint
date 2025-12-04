#include "suite.h"

#include <stdbool.h>
#include <stdio.h>

#include "utils/color.h"

static int run_case(const char* name, test_fn_t fn) {
  bool ok = fn();
  printf("  ");
  if (ok) {
    printf("%s %s\n", color("PASS:", ColorGRN), name);
    fflush(stdout);
    return 1;
  } else {
    printf("%s %s\n", color("FAIL:", ColorRED), name);
    fflush(stdout);
    return 0;
  }
}

int run_suite(TestSuite suite) {
  int passed = 0;

  for (int i = 0; i < suite.count; i++) {
    passed += run_case(suite.cases[i].name, suite.cases[i].fn);
  }

  return passed;
}