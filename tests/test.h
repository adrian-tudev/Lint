#ifndef TEST_H_
#define TEST_H_

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "utils/string_utils.h"

void runTests() {
  const char* str = "this is a string";

  int len = -1;
  const char** splitted = split_string(str, &len, NULL);

  assert(len == 4);
  for (int i = 0; i < len; i++) {
    printf("%s ", splitted[i]);
  }
  printf("\n");
}

#endif // TEST_H_
