idf_component_register(
  SRCS "RGBLEDDriver.c" "led_colors.c"
  INCLUDE_DIRS "include"
  REQUIRES led_strip esp_driver_gpio)
