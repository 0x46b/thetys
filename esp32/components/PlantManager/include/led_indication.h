#ifndef LED_INDICATION_H
#define LED_INDICATION_H

#include <esp_err.h>
#include <stdint.h>

typedef enum led_indication_T {
  LED_OK = 0,
  LED_BUSY = 1,
  LED_BT_CONNECTED = 2,
  LED_ERROR = 3,
  LED_FATAL = 4
} led_indication_T;

typedef uint32_t led_context_handle_T;

esp_err_t enter_led_context(led_indication_T status,
                            led_context_handle_T *handle);
esp_err_t exit_led_context(led_context_handle_T context);

#endif // LED_INDICATION_H
