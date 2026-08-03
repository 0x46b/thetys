#include "sensor_calibration.h"
#include <stdint.h>
#include <unity.h>

TEST_CASE("clean_samples should remove runaways", "[Calibration]") {
  const int number_of_samples = 15;
  float dirty_samples[15] = {1, 1, 1, 1, -100, 100, 10, 1, 1, 1, 1, 1, 1, 1, 1};
  float *clean_samples;
  uint32_t number_of_cleaned_samples;

  CALIB_RESULT result =
      clean_samples(&dirty_samples, number_of_samples, &clean_samples,
                    &number_of_cleaned_samples);
  TEST_ASSERT(result == CALIB_SUCCESS);
  TEST_ASSERT(*number_of_cleaned_samples == 12);
}
