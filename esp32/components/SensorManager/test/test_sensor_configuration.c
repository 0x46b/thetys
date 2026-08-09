#include "sensor_configuration.h"
#include <unity.h>

TEST_CASE("Insert without initialization should fail",
          "[sensor_configuration]") {
  sensor_configuration test_configuration = {
      .sensor_id = 0,
      .sensor_gpio = 12,
      .calibration_data = {.air_factor = 17, .water_factor = 18}};
  SENSOR_CONFIGURATION_RESULT result = insert_configuration(test_configuration);

  TEST_ASSERT_EQUAL(UNINITIALIZED, result);
  free_configurations();
}

TEST_CASE("Adding a sensor should store the sensor correctly",
          "[sensor_configuration]") {
  init_configurations(1);
  sensor_configuration config_to_store = {
      .sensor_id = 0,
      .sensor_gpio = 12,
      .calibration_data = {.air_factor = 17, .water_factor = 18}};

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
      .calibration_data = {.air_factor = 17, .water_factor = 18}};

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

  TEST_ASSERT(result == INITIALIZATION_ERROR);
  free_configurations();
}

// TEST_CASE("Inserting a NULL element should not be allowed",
//           "[sensor_configuration]") {
//   sensor_configuration *null_config_ptr = NULL;

//   init_configurations(2);
//   SENSOR_CONFIGURATION_RESULT result =
//   insert_configuration(*null_config_ptr);

//   TEST_ASSERT(result == PARAMETER_NULL);
// }

TEST_CASE("Getting a configuration that is not known returns an error",
          "[sensor_configuration]") {
  sensor_configuration test_configuration = {
      .sensor_id = 0,
      .sensor_gpio = 12,
      .calibration_data = {.air_factor = 17, .water_factor = 18}};
  sensor_configuration retrieved_result;

  init_configurations(4);
  SENSOR_CONFIGURATION_RESULT result = insert_configuration(test_configuration);

  result = get_configuration_for_sensor_id(&retrieved_result, 2);
  TEST_ASSERT(result == UNKNOWN_SENSOR_ID);
  free_configurations();
}

TEST_CASE("Retrieving stored configuration should correctly get stored values",
          "[sensor_configuration]") {
  sensor_configuration test_configuration1 = {
      .sensor_id = 0,
      .sensor_gpio = 12,
      .calibration_data = {.air_factor = 17, .water_factor = 18}};
  sensor_configuration test_configuration2 = {
      .sensor_id = 0,
      .sensor_gpio = 12,
      .calibration_data = {.air_factor = 17, .water_factor = 18}};

  sensor_configuration retrieved_result;

  init_configurations(2);
  insert_configuration(test_configuration1);
  insert_configuration(test_configuration2);
  SENSOR_CONFIGURATION_RESULT result =
      get_configuration_for_sensor_id(&retrieved_result, 1);
  TEST_ASSERT_EQUAL(SUCCESS, result);
  TEST_ASSERT_EQUAL(test_configuration2.sensor_gpio,
                    retrieved_result.sensor_gpio);
  TEST_ASSERT_EQUAL(test_configuration2.sensor_id, retrieved_result.sensor_id);
  TEST_ASSERT_EQUAL(test_configuration2.calibration_data.air_factor,
                    retrieved_result.calibration_data.air_factor);
  TEST_ASSERT_EQUAL(test_configuration2.calibration_data.water_factor,
                    retrieved_result.calibration_data.water_factor);

  free_configurations();
}

TEST_CASE("Retrieving the number of sensors returns the correct count",
          "[sensor_configuration]") {
  init_configurations(5);
  sensor_configuration test_configuration = {
      .sensor_id = 0,
      .sensor_gpio = 12,
      .calibration_data = {.air_factor = 17, .water_factor = 18}};

  insert_configuration(test_configuration);
  insert_configuration(test_configuration);
  uint32_t count = 0;
  SENSOR_CONFIGURATION_RESULT result = get_number_of_configurations(&count);
  TEST_ASSERT_ARE_EQUAL(SENSOR_CFG_OK, result);
  TEST_ASSERT_ARE_EQUAL(2, count);
}
