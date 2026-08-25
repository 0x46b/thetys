#include "RGBLEDDriver.h"
#include <driver/gpio.h>
#include <esp_log.h>
#include <led_strip.h>
#include <math.h>
#include <sdkconfig.h>
#include <stdint.h>

static led_strip_handle_t led_strip;
static uint32_t current_brightness_factor = 1;
static bool initialized = false;

static const char *TAG = "RGBLEDDriver";

uint8_t gamma_correct(uint8_t input_value) {
  if (input_value == 0)
    return 0;

  const float gamma = 2.2f;

  float normalized = (float)input_value / 255.0f;
  float corrected = powf(normalized, gamma);

  return (uint8_t)(corrected * 255.0f + 0.5f);
}

uint32_t apply_brightness(uint32_t channel) {
  return gamma_correct(channel * current_brightness_factor / 255);
}

void led_drv_initialize(void) {
  if (initialized) {
    ESP_LOGW(TAG, "RGBLEDDriver already initialized: Recurring calls to "
                  "led_drv_initialize() will get ignored.");
    return;
  }
  current_brightness_factor = 100;
  ESP_ERROR_CHECK(gpio_reset_pin(CONFIG_RGB_LED_GPIO));
  ESP_ERROR_CHECK(gpio_set_direction(CONFIG_RGB_LED_GPIO, GPIO_MODE_OUTPUT));

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
  ESP_ERROR_CHECK(led_strip_clear(led_strip));
  initialized = true;
  ESP_LOGV(TAG, "Successfully initialized RGB-LED at GPIO %i",
           CONFIG_RGB_LED_GPIO);
}

void led_drv_set_color(uint32_t red, uint32_t green, uint32_t blue) {
  if (!initialized) {
    ESP_LOGE(TAG, "Call to led_drv_set_color before initializiation. Call "
                  "led_drv_initialize() first!");
    return;
  }
  ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 0, apply_brightness(red),
                                      apply_brightness(green),
                                      apply_brightness(blue)));
  ESP_ERROR_CHECK(led_strip_refresh(led_strip));
  ESP_LOGD(TAG, "Changed color to (%i, %i, %i)", red, green, blue);
}

void led_drv_set_brightness(uint32_t brightness) {
  if (!initialized) {
    ESP_LOGE(TAG, "Call to led_drv_set_color before initializiation. Call "
                  "led_drv_initialize() first!");
    return;
  }
  current_brightness_factor = brightness;
  ESP_LOGD(TAG, "Changed brightness to %i", brightness);
}

void led_drv_off() {
  if (!initialized) {
    ESP_LOGE(TAG, "Call to led_drv_set_color before initializiation. Call "
                  "led_drv_initialize() first!");
    return;
  }
  ESP_ERROR_CHECK(led_strip_clear(led_strip));
  ESP_LOGD(TAG, "Set LED off");
}

void led_drv_set_to(rgb_color color) {
  led_drv_set_color(color.red, color.green, color.blue);
  ESP_LOGV(TAG, "Changed color to '%s'", color.name);
}
