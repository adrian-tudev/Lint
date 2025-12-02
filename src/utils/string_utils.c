#include "string_utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char** split_string(const char* s, size_t* words, char* delimiters) {
  const char** strings = NULL;
  int cnt = 0;
  int cur_len = 0;

  // count number of "words"
  size_t len = strlen(s);
  for (size_t i = 0; i < len; i++) {
    if(is_delimiter(s[i], delimiters)) {
      if (cur_len == 0) continue;
      cnt++;
      cur_len = 0;
      continue;
    }
    cur_len++;
  }
  if (cur_len > 0) cnt++;

  strings = (const char**) malloc(cnt * sizeof(const char*));
  if (strings == NULL) {
    *words = 0;
    return NULL;
  }

  *words = cnt;

  cnt = 0;
  cur_len = 0;
  for (size_t i = 0; i < len; i++) {
    if(is_delimiter(s[i], delimiters)) {
      if (cur_len == 0) continue;
      // copy word into resulting array
      char* string = malloc(cur_len + 1);
      memcpy(string, s + (i - cur_len), cur_len);
      string[cur_len] = '\0';

      strings[cnt] = string;
      cnt++;
      cur_len = 0;
      continue;
    }
    cur_len++;
  }

  if (cur_len > 0) {
      char* string = malloc(cur_len + 1);
      memcpy(string, s + (len - cur_len), cur_len);
      string[cur_len] = '\0';

      strings[++cnt] = string;
  }
  
  return strings;
}

bool is_delimiter(const char c, char* delimiters) {
  if (delimiters == NULL) {
    delimiters = " \n";
  }

  size_t len = strlen(delimiters);
  for (size_t i = 0; i < len; i++) {
    if (c == delimiters[i]) return true;
  }

  return false;
}

char* substring(const char* src, size_t start, size_t len) {
  char* token_str = malloc(len + 1);
  memcpy(token_str, src + start, len);
  token_str[len] = '\0';
  return token_str;
}

bool char_in_arr(const char c, const char* arr) {
  size_t len = sizeof(arr);
  for (size_t i = 0; i < len; i++) {
    if (c == arr[i]) return true;
  }
  return false;
}
