#include "color.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

// Colors the given string with the specified color and returns it.
char* color(const char* string, Color string_color) {
  const char* color_code;
  switch (string_color) {
    case ColorRED:
      color_code = RED;
      break;
    case ColorGRN:
      color_code = GRN;
      break;
    case ColorYEL:
      color_code = YEL;
      break;
    case ColorBLU:
      color_code = BLU;
      break;
    case ColorMAG:
      color_code = MAG;
      break;
    case ColorCYN:
      color_code = CYN;
      break;
    case ColorWHT:
      color_code = WHT;
      break;
    default:
      color_code = RESET;
      break;
  }

  size_t colored_len = strlen(color_code) + strlen(string) + strlen(RESET) + 1;
  char* colored_string = (char*)malloc(colored_len);
  snprintf(colored_string, colored_len, "%s%s%s", color_code, string, RESET);
  return colored_string;
}