#include "test.h"

#include "suite.h"
#include "utils/color.h"
#include "utils/error.h"

// test suites
#include "ast/test_ast.h"
#include "scanners/test_scanner.h"
#include "token/test_token.h"
#include "misc/test_utils.h"
#include "parser/test_parser.h"
#include "execution/test_execution.h"

void runTests(void) {
  printf("\n%s\n====================\n\n", 
    color("     UNIT TESTS    ", ColorMAG));

  // OBS: the suites should only be included here to avoid multiple definition errors
  TestSuite suites[] = {
    get_ast_suite(),
    get_scanner_suite(),
    get_token_suite(),
    get_utils_suite(),
    get_parser_suite(),
    get_execution_suite(),
  };

  size_t num_suites = sizeof(suites) / sizeof(TestSuite);
  size_t passed_suites = 0;
  for (size_t t = 0; t < num_suites; t++) {
    printf("%s %s\n", color("Suite:", ColorBLU), suites[t].name);
    int suite_passed = run_suite(suites[t]);
    if (suite_passed == suites[t].count) {
      passed_suites++;
    }
    printf("\n");
  }

  printf("Suites passed: %zu/%zu\n", passed_suites, num_suites);
  if (passed_suites == num_suites)
    printf("🎉 %s 🎉\n", color("All suites passed!", ColorGRN));
  else
    printf("%s\n", color("Some suites failed :/", ColorRED));
  
  printf("\n");
}

int main(void) {
  // Ensure deterministic interleaving of stdout (colored) vs stderr (errors)
  setvbuf(stdout, NULL, _IONBF, 0);
  // Disable error logging during unit tests for cleaner output
  error_set_enabled(false);
  runTests();
  return 0;
}
