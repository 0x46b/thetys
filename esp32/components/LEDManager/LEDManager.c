#include "LEDManager.h"
#include "RGBLEDDriver.h"
#include "include/LEDManager.h"
#include <esp_log.h>

/* Colordefinitions for the available contexts */
#define LED_STATE_OK GREEN
#define LED_STATE_BUSY MAGENTA
#define LED_STATE_BLUETOOTH_CONNECTED BLUE
#define LED_STATE_RECOVERABLE_ERROR ORANGE
#define LED_STATE_FATAL RED

static int context_counters[CTX_COUNT] = {0};
static const char *TAG = "LEDManager";

/* Private methods */
rgb_color get_color_for_context(context_id context) {
  switch (context) {
  case CTX_BLUETOOTH:
    return LED_STATE_BLUETOOTH_CONNECTED;
  case CTX_BUSY:
    return LED_STATE_BUSY;
  case CTX_ERROR:
    return LED_STATE_RECOVERABLE_ERROR;
  case CTX_FATAL:
    return LED_STATE_FATAL;
  default:
    ESP_LOGW(TAG, "No context, setting LED to OK");
    return LED_STATE_OK;
  };
}

static void led_manager_update(void) {
  /* Check from highest to lowest priority */
  for (int i = CTX_COUNT - 1; i > 0; i--) {
    /* We found something, so use this state for the indication light */
    if (context_counters[i] > 0) {
      led_drv_set_to(get_color_for_context(i));
      return;
    }
  }

  led_drv_set_to(LED_STATE_OK);
}

/* Public methods */
void led_context_enter(context_id ctx) {
  if (ctx <= 0 || ctx >= CTX_COUNT)
    return;

  context_counters[ctx]++;
  led_manager_update();
}

void led_context_leave(context_id ctx) {
  if (ctx <= 0 || ctx >= CTX_COUNT)
    return;

  if (context_counters[ctx] > 0) {
    context_counters[ctx]--;
  }

  led_manager_update();
}

void led_context_init() {
  led_drv_initialize();
  led_drv_set_to(LED_STATE_OK);
}
