#ifndef LED_COLORS_H
#define LED_COLORS_H

#include <stdint.h>

typedef struct rgb_color {
  uint32_t red;
  uint32_t green;
  uint32_t blue;
} rgb_color;

#define WHITE {255, 255, 255}
#define RED {255, 0, 0}
#define GREEN {0, 255, 0}
#define BLUE {0, 0, 255}
#define YELLOW {255, 255, 0}
#define CYAN {0, 255, 255}
#define MAGENTA {255, 0, 255}
#define ORANGE {255, 90, 0}
#define PURPLE {255, 65, 255}
#define PINK {255, 25, 125}

#endif // RGB_COLORS_H
