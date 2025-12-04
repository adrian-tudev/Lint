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

const char* color(const char* string, Color string_color);

#endif // COLOR_H_