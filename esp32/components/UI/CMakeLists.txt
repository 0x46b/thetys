idf_component_register(
  SRCS "ui.c" "screens.c" "images.c" "styles.c" "vars.c" "thetys_ui.c" "images/ui_image_pump_frame_1.c" "images/ui_image_pump_frame_2.c" "images/ui_image_pump_frame_3.c" "images/ui_image_pump_stopped_frame_1.c"
  PRIV_REQUIRES ScreenDriver
  INCLUDE_DIRS "include")
