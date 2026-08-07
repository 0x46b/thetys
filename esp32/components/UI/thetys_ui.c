#include "thetys_ui.h"
#include "screens.h"
#include "ui.h"
#include <esp_log.h>
#include <esp_lvgl_port.h>

UI_RESULT start_thetys_ui() {
  if (lvgl_port_lock(0)) {
    ui_init();
    lvgl_port_unlock();
  } else {
    ESP_LOGE("UI", "LVGL-Lock could not get acquired!");
    return UI_LVGL_LOCK_FAILED;
  }
  return UI_SUCCESS;
}

UI_RESULT add_plant() {
  if (lvgl_port_lock(0)) {
    create_user_widget_plant_view(objects.plant_list, 3);
    lvgl_port_unlock();
  } else {
    ESP_LOGE("UI", "LVGL-Lock could not get acquired!");
    return UI_LVGL_LOCK_FAILED;
  }
  return UI_SUCCESS;
}
