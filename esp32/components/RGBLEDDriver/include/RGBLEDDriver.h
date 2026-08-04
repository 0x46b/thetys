#ifndef RGBLEDDRIVER_H
#define RGBLEDDRIVER_H
/*! @file
 * Wrapper methods for controlling the on-board RGB-LED
 */
#include "led_colors.h"
#include <stdint.h>
#include <sys/types.h>

/*! Initialize the RGB-LED-driver. Has to get called before using the other
 * methods.
 */
void led_drv_initialize(void);

/*! Sets the overall brightness of the LED
 * @param brightness Brightness of the LED (0..255)
 */
void led_drv_set_brightness(uint32_t brightness);

/*! Sets the color of the LED to mix every possible color via RGB
 * @param red The red portion of the color
 * @param green The green portion of the color
 * @param blue The blue portion of the color
 */
void led_drv_set_color(uint32_t red, uint32_t green, uint32_t blue);

/*! Same as led_drv_set_color, but a color struct could be used (So we can
predefine colors)
* @param color rgb_color-defined color
*/
void led_drv_set_to(rgb_color color);

/*! Shut the LED off */
void led_drv_off();

#endif // RGBLEDDRIVER_H
