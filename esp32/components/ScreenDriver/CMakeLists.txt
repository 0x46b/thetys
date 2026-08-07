idf_component_register(
  SRCS "ScreenDriver.c"
  PRIV_REQUIRES espressif__esp_lvgl_port esp_driver_spi espressif__esp_lcd_ili9341
  INCLUDE_DIRS "include")
