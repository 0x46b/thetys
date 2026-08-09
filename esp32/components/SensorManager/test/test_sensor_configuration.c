#include "sensor_configuration.h"
#include <unity.h>

TEST_CASE("Insert without initialization should fail",
          "[sensor_configuration]") {
  sensor_configuration test_configuration = {
      .sensor_id = 0,
      .sensor_gpio = 12,
      .calibration_data = {.air_measurement = 17, .water_measurement = 18}};
  SENSOR_CONFIGURATION_RESULT result = insert_configuration(test_configuration);

  TEST_ASSERT_EQUAL(SENSOR_CFG_UNINITIALIZED, result);
  free_configurations();
}

TEST_CASE("Adding a sensor should store the sensor correctly",
          "[sensor_configuration]") {
  init_configurations(1);
  sensor_configuration config_to_store = {
      .sensor_id = 0,
      .sensor_gpio = 12,
      .calibration_data = {.air_measurement = 17, .water_measurement = 18}};

  insert_configuration(config_to_store);
  sensor_configuration retrieved_configuration;
  get_configuration_for_sensor_id(&retrieved_configuration, 0);
  TEST_ASSERT_EQUAL_MEMORY(&config_to_store, &retrieved_configuration,
                           sizeof(config_to_store));
  free_configurations();
}

TEST_CASE(
    "Adding more sensors than initialized should increase the array silently",
    "[sensor_configuration]") {
  sensor_configuration test_configuration = {
      .sensor_id = 0,
      .sensor_gpio = 12,
      .calibration_data = {.air_measurement = 17, .water_measurement = 18}};

  init_configurations(1);
  insert_configuration(test_configuration);
  insert_configuration(test_configuration);
  insert_configuration(test_configuration);

  sensor_configuration retrieved_configuration;
  get_configuration_for_sensor_id(&retrieved_configuration, 2);
  TEST_ASSERT_EQUAL_MEMORY(&test_configuration, &retrieved_configuration,
                           sizeof(test_configuration));
  free_configurations();
}

TEST_CASE("Initialization with 0 elements should fail",
          "[sensor_configuration]") {
  SENSOR_CONFIGURATION_RESULT result = init_configurations(0);

  TEST_ASSERT_EQUAL(SENSOR_CFG_INITIALIZATION_ERROR, result);
  free_configurations();
}

TEST_CASE("Getting a configuration that is not known returns an error",
          "[sensor_configuration]") {
  sensor_configuration test_configuration = {
      .sensor_id = 0,
      .sensor_gpio = 12,
      .calibration_data = {.air_measurement = 17, .water_measurement = 18}};
  sensor_configuration retrieved_result;

  init_configurations(4);
  SENSOR_CONFIGURATION_RESULT result = insert_configuration(test_configuration);

  result = get_configuration_for_sensor_id(&retrieved_result, 2);
  TEST_ASSERT_EQUAL(SENSOR_CFG_UNKNOWN_SENSOR_ID, result);
  free_configurations();
}

TEST_CASE("Retrieving stored configuration should correctly get stored values",
          "[sensor_configuration]") {
  sensor_configuration test_configuration1 = {
      .sensor_id = 0,
      .sensor_gpio = 0,
      .calibration_data = {.air_measurement = 0,
                           .water_measurement = 0,
                           .air_reference = 0,
                           .water_reference = 0}};
  sensor_configuration test_configuration2 = {
      .sensor_id = 0,
      .sensor_gpio = 12,
      .calibration_data = {.air_measurement = 17,
                           .water_measurement = 18,
                           .air_reference = 19,
                           .water_reference = 20}};

  sensor_configuration retrieved_result;

  init_configurations(2);
  insert_configuration(test_configuration1);
  insert_configuration(test_configuration2);
  SENSOR_CONFIGURATION_RESULT result =
      get_configuration_for_sensor_id(&retrieved_result, 1);
  TEST_ASSERT_EQUAL(SENSOR_CFG_OK, result);
  TEST_ASSERT_EQUAL(test_configuration2.sensor_gpio,
                    retrieved_result.sensor_gpio);
  TEST_ASSERT_EQUAL(test_configuration2.sensor_id, retrieved_result.sensor_id);

  TEST_ASSERT_EQUAL(test_configuration2.calibration_data.air_measurement,
                    retrieved_result.calibration_data.air_measurement);

  TEST_ASSERT_EQUAL(test_configuration2.calibration_data.water_measurement,
                    retrieved_result.calibration_data.water_measurement);

  TEST_ASSERT_EQUAL(test_configuration2.calibration_data.air_reference,
                    retrieved_result.calibration_data.air_reference);

  TEST_ASSERT_EQUAL(test_configuration2.calibration_data.water_reference,
                    retrieved_result.calibration_data.water_reference);
  free_configurations();
}

TEST_CASE("Retrieving the number of sensors returns the correct count",
          "[sensor_configuration]") {
  init_configurations(5);
  sensor_configuration test_configuration = {
      .sensor_id = 0,
      .sensor_gpio = 12,
      .calibration_data = {.air_measurement = 17, .water_measurement = 18}};

  insert_configuration(test_configuration);
  insert_configuration(test_configuration);
  uint32_t count = 0;
  SENSOR_CONFIGURATION_RESULT result = get_number_of_configurations(&count);
  TEST_ASSERT_EQUAL(SENSOR_CFG_OK, result);
  TEST_ASSERT_EQUAL(2, count);
}
