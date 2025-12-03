#include "test.h"

static int run_case(const char* name, bool (*fn)()) {
  bool ok = fn();
  if (ok) {
    printf("PASS: %s\n", name);
    return 1;
  } else {
    printf("FAIL: %s\n", name);
    return 0;
  }
}

void runTests(void) {
  int total = 0;
  int passed = 0;

  bool (*tests[])() = {
    split_string_test,
    word_scanner_test,
    literal_scanner_test,
    string_scanner_test,
    op_scanner_test,
    punctuation_scanner_test,
  };

  const char* test_names[] = {
    "split_string",
    "word_scanner",
    "literal_scanner",
    "string_scanner",
    "op_scanner",
    "punctuation_scanner",
  };

  size_t num_tests = sizeof(tests) / sizeof(tests[0]);
  for (size_t i = 0; i < num_tests; i++) {
    total++;
    passed += run_case(test_names[i], tests[i]);
  }

  printf("\nTests passed: %d/%d\n", passed, total);
}

int main(void) {
  runTests();
  return 0;
}
