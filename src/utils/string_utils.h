#ifndef STRING_UTILS_H_
#define STRING_UTILS_H_

#include <stdbool.h>
#include <stdlib.h>

/*
 * Delimiters is optional and will default to newline and whitespace
 */
const char** split_string(const char* s, size_t* len, char* delimiters);

/*
 * Checks if given character is a delimiter. Passing NULL as a delimiter
 * defaults to whitespace and newline as delimiters.
 */
bool is_delimiter(const char c, char* delimiters);

char* substring(const char* src, size_t start, size_t len);

bool char_in_arr(const char c, const char* arr);

#endif // STRING_UTILS_H_
