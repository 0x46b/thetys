#include "vars.h"
#include <esp_log.h>
#include <esp_lvgl_port.h>
#include <stdint.h>

static uint32_t humidity = 0;
static bool pump_running;

static const char *TAG = "UI:vars.c";

const char *get_var__plant_id() { return "PFLANZE 1"; }

void set_var__plant_id(const char *value) {}
int32_t get_var__humidity_level() { return 4095 - humidity; }
void set_var__humidity_level(int32_t value) {
  ESP_LOGI(TAG, "Updated humidity to %i", value);
  humidity = value;
}
bool get_var__pump_running() { return pump_running; }
void set_var__pump_running(bool value) { pump_running = value; }
bool get_var_pump_stopped() { return !pump_running; }
void set_var_pump_stopped(bool value) {}
