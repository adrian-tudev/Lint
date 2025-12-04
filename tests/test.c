#include "test.h"

#include "utils/color.h"
#include "suite.h"
#include "scanners/test_scanner.h"
#include "token/test_token.h"
#include "misc/test_utils.h"

void runTests(void) {
  printf("\n");

  TestSuite suites[] = {
    get_utils_suite(),
    get_scanner_suite(),
    get_token_suite(),
  };

  size_t num_suites = sizeof(suites) / sizeof(TestSuite);
  size_t passed_suites = 0;
  for (size_t s = 0; s < num_suites; s++) {
    printf("%s %s\n", color("Suite:", ColorBLU), suites[s].name);
    int suite_passed = run_suite(suites[s]);
    if (suite_passed == suites[s].count) {
      passed_suites++;
    }
  }

  printf("\nSuites passed: %zu/%zu\n", passed_suites, num_suites);
  if (passed_suites == num_suites)
    printf("🎉 %s 🎉\n", color("All suites passed!", ColorGRN));
  else
    printf("%s\n", color("Some suites failed :/", ColorRED));
}

int main(void) {
  // Ensure deterministic interleaving of stdout (colored) vs stderr (errors)
  setvbuf(stdout, NULL, _IONBF, 0);
  runTests();
  return 0;
}
