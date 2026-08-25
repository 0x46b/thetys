#include "plant_service.h"
#include "PlantManager.h"
#include "plant_mgr_type_definitions.h"
#include <cJSON.h>

/*! @todo replace with dynamic */
static humidity_data_T humidity_data[5];

/* Private functions */
cJSON *build_humidity_data_json(humidity_data_T data) {
  cJSON *humidity_json_root = cJSON_CreateObject();
  if (humidity_json_root == NULL) {
    goto end;
  }

  cJSON *plant_name = cJSON_CreateString(data.plant_name);
  if (plant_name == NULL) {
    goto end;
  }
  cJSON_AddItemToObject(humidity_json_root, "plant_name", plant_name);

  cJSON *plant_handle = cJSON_CreateNumber(data.plant_handle);
  if (plant_handle == NULL) {
    goto end;
  }
  cJSON_AddItemToObject(humidity_json_root, "plant_handle", plant_handle);

  cJSON *humidity_percentage = cJSON_CreateNumber(data.humidity_percentage);
  if (humidity_percentage == NULL) {
    goto end;
  }
  cJSON_AddItemToObject(humidity_json_root, "humidity_percentage",
                        humidity_percentage);

  cJSON *pump_running = cJSON_CreateBool(data.pump_running);
  if (pump_running == NULL) {
    goto end;
  }
  cJSON_AddItemToObject(humidity_json_root, "pump_running", pump_running);

  cJSON *is_active = cJSON_CreateBool(data.is_active);
  if (is_active == NULL) {
    goto end;
  }
  cJSON_AddItemToObject(humidity_json_root, "is_active", is_active);

  /* char *json_string = cJSON_Print(monitor); */
  /* if (json_string == NULL) { */
  /*   EPS_LOGE(TAG, "Failed to create JSON for humidity data."); */
  /* } */

end:
  cJSON_Delete(humidity_json_root);
  /* return json_string */;
  return humidity_json_root;
}

/* Public functions */
humidity_data_T *update_humidities(void) {
  humidity_data_T data = {.humidity_percentage = 10};
  return &data;
}
char *jsonify_humidities(void) {
  cJSON *humidity_json_root = cJSON_CreateObject();
  cJSON *plant_name = cJSON_CreateString("");
  return "";
}

humidity_data_T blu_get_humidity(plant_handle_T plant) {
  humidity_data_T data = {.humidity_percentage = 10};
  return data;
}
