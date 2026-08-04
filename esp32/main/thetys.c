#include "BluetoothManager.h"
#include "RGBLEDDriver.h"
#include "SensorManager.h"
#include <esp_log.h>
#include <sdkconfig.h>

static const char *TAG = "app_main";

void app_main(void) {
  led_drv_initialize();
  led_drv_set_color(0, 0, 12);
  blu_initialize();

  initialize(CONFIG_MAX_SENSOR_COUNT);
  add_sensor(10);
  add_sensor(11);

  sensor_reading reading;
  calibrate_air(10);
  calibrate_water(10);
  read_sensor_data(10, &reading);
  led_drv_set_color(0, 50, 50);
}
