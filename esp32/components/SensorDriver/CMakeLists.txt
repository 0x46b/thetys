idf_component_register(
  SRCS "SensorDriver.c"
  PRIV_REQUIRES esp_driver_gpio esp_adc
  INCLUDE_DIRS "include")
