idf_component_register(
  SRCS "SensorManager.c" "sensor_configuration.c" "sensor_calibration.c" "sensor_samples.c"
  INCLUDE_DIRS "include"
  REQUIRES SensorDriver freertos
)
