#include "test.h"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

typedef bool (*test_fn_t)(void);

static int run_case(const char* name, test_fn_t fn) {
  bool ok = fn();
  if (ok) {
    printf(GRN "PASS: %s\n" RESET, name);
    fflush(stdout);
    return 1;
  } else {
    printf(RED "FAIL: %s\n" RESET, name);
    fflush(stdout);
    return 0;
  }
}

void runTests(void) {
  int total = 0;
  int passed = 0;

  test_fn_t tests[] = {
    split_string_test,
    word_scanner_test,
    literal_scanner_test,
    string_scanner_test,
    op_scanner_test,
    punctuation_scanner_test,
    tokenizer_test,
    tokenizer_error_cases_test,
    tokenizer_edge_cases_test,
  };

  const char* test_names[] = {
    "split_string",
    "word_scanner",
    "literal_scanner",
    "string_scanner",
    "op_scanner",
    "punctuation_scanner",
    "tokenizer",
    "tokenizer_error_cases",
    "tokenizer_edge_cases",
  };

  size_t num_tests = sizeof(tests) / sizeof(test_fn_t);
  for (size_t i = 0; i < num_tests; i++) {
    total++;
    passed += run_case(test_names[i], tests[i]);
  }

  printf("\nTests passed: %d/%d\n", passed, total);
}

int main(void) {
  // Ensure deterministic interleaving of stdout (colored) vs stderr (errors)
  setvbuf(stdout, NULL, _IONBF, 0);
  runTests();
  return 0;
}
