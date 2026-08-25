#ifndef LED_COLORS_H
#define LED_COLORS_H

#include <stdint.h>

typedef struct rgb_color {
  const char *name;
  uint32_t red;
  uint32_t green;
  uint32_t blue;
} rgb_color;

extern const rgb_color WHITE;
extern const rgb_color RED;
extern const rgb_color GREEN;
extern const rgb_color BLUE;
extern const rgb_color YELLOW;
extern const rgb_color CYAN;
extern const rgb_color MAGENTA;
extern const rgb_color ORANGE;
extern const rgb_color PURPLE;
extern const rgb_color PINK;
#define LED_COLOR_

#endif // LED_COLORS_H
