#ifndef COLOR_H_
#define COLOR_H_

typedef enum Color {
  ColorRED,
  ColorGRN,
  ColorYEL,
  ColorBLU,
  ColorMAG,
  ColorCYN,
  ColorWHT,
} Color;

/**
 * Colors the given string with the specified color.
 * @param string The string to colorize
 * @param string_color The color to apply
 * @return A newly allocated colored string. Caller must free.
 *         Returns NULL on allocation failure.
 */
char* color(const char* string, Color string_color);

#endif // COLOR_H_