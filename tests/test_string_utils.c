#include "test.h"

bool split_string_test(void) {
  const char* str = "  this is  a    string  ";
  const char* gold[] = {"this", "is", "a", "string"};

  size_t len = -1;
  const char** splitted = split_string(str, &len, NULL);
  if (len != 4) {
    // free any allocated memory before returning
    for (size_t i = 0; i < len; i++) free((void*)splitted[i]);
    free(splitted);
    return false;
  }

  for (size_t i = 0; i < len; i++) {
    if (strcmp(splitted[i], gold[i]) != 0) {
      for (size_t j = 0; j < len; j++) free((void*)splitted[j]);
      free(splitted);
      return false;
    }
    free((void*)splitted[i]);
  }
  free(splitted);
  return true;
}
