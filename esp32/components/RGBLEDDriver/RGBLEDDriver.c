#include "RGBLEDDriver.h"
#include <driver/gpio.h>
#include <esp_log.h>
#include <led_strip.h>
#include <sdkconfig.h>
#include <stdint.h>

static led_strip_handle_t led_strip;
static uint32_t current_brightness;
static bool initialized = false;

static const char *TAG = "RGBLEDDriver";

void led_drv_initialize(void) {
  gpio_reset_pin(CONFIG_RGB_LED_GPIO);
  gpio_set_direction(CONFIG_RGB_LED_GPIO, GPIO_MODE_OUTPUT);

  led_strip_config_t strip_config = {
      .strip_gpio_num = CONFIG_RGB_LED_GPIO,
      .max_leds = 1,
  };

  led_strip_rmt_config_t rmt_config = {
      .resolution_hz = 10 * 1000 * 1000, // 10MHz
      .flags.with_dma = false,
  };

  ESP_ERROR_CHECK(
      led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
  led_strip_clear(led_strip);
  initialized = true;
  ESP_LOGI(TAG, "Successfully initialized RGB-LED at GPIO %i",
           CONFIG_RGB_LED_GPIO);
}

void led_drv_set_color(uint32_t red, uint32_t green, uint32_t blue) {
  if (!initialized) {
    ESP_LOGE(TAG, "Call to led_drv_set_color before initializiation. Call "
                  "led_drv_initialize() first!");
    return;
  }
  led_strip_set_pixel(led_strip, 0, red, green, blue);
  led_strip_refresh(led_strip);
  ESP_LOGI(TAG, "Changed color to (%i, %i, %i)", red, green, blue);
}

void led_drv_set_brightness(uint32_t brightness) {
  if (!initialized) {
    ESP_LOGE(TAG, "Call to led_drv_set_color before initializiation. Call "
                  "led_drv_initialize() first!");
    return;
  }
  current_brightness = brightness;
  ESP_LOGI(TAG, "Changed brightness to %i", brightness);
}

void led_drv_off() {
  if (!initialized) {
    ESP_LOGE(TAG, "Call to led_drv_set_color before initializiation. Call "
                  "led_drv_initialize() first!");
    return;
  }
  led_strip_clear(led_strip);
  ESP_LOGI(TAG, "Set LED off");
}
