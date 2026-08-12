idf_component_register(
  SRCS "RelaisDriver.c" "relais_register.c"
  REQUIRES esp_driver_gpio
  INCLUDE_DIRS "include")
