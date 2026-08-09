#include "sensor_calibration.h"
#include <unity.h>

TEST_CASE("Humiditypercentage should get calculated correctly for 0%",
          "[sensor_calibration]") {
  sensor_calibration_data test_calibration = {.air_measurement = 4095,
                                              .air_reference = 4095,
                                              .water_measurement = 2000,
                                              .water_reference = 0};

  uint32_t raw_value = 4095;
  uint32_t humidity = get_humidity(raw_value, test_calibration);

  TEST_ASSERT_EQUAL(0, humidity);
}

TEST_CASE("Humiditypercentage should get calculated correctly for 100%",
          "[sensor_calibration]") {
  sensor_calibration_data test_calibration = {.air_measurement = 4095,
                                              .air_reference = 4095,
                                              .water_measurement = 2000,
                                              .water_reference = 0};
  uint32_t raw_value = 2000;
  uint32_t humidity = get_humidity(raw_value, test_calibration);

  TEST_ASSERT_EQUAL(100, humidity);
}

TEST_CASE("Humiditypercentage should get calculated correctly for 50%",
          "[sensor_calibration]") {
  sensor_calibration_data test_calibration = {.air_measurement = 4095,
                                              .air_reference = 4095,
                                              .water_measurement = 2000,
                                              .water_reference = 0};

  uint32_t raw_value = 3000;
  uint32_t humidity = get_humidity(raw_value, test_calibration);

  TEST_ASSERT_EQUAL(50, humidity);
}
