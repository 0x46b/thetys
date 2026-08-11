#include "ui_data.h"
#include "misc/lv_anim.h"
#include "screens.h"
#include <lvgl.h>

esp_err_t apply_plant_state(plant_state state) {
  lv_label_set_text(objects.txt_plant_name, state.plant_name);
  lv_label_set_text(objects.txt_configured_threshold,
                    state.configured_watering_threshold);
  lv_label_set_text(objects.txt_last_watered_at, state.last_watering);
  lv_label_set_text(objects.txt_last_measured_at,
                    state.newest_measurement.timestamp);

  lv_bar_set_value(objects.bar_humidity_level,
                   state.newest_measurement->humidity_in_percent, LV_ANIM_ON);
}
