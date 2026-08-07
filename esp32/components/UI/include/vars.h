#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations

// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE__PLANT_ID = 0,
    FLOW_GLOBAL_VARIABLE__HUMIDITY_LEVEL = 1,
    FLOW_GLOBAL_VARIABLE__PUMP_RUNNING = 2
};

// Native global variables

extern const char *get_var__plant_id();
extern void set_var__plant_id(const char *value);
extern int32_t get_var__humidity_level();
extern void set_var__humidity_level(int32_t value);
extern bool get_var__pump_running();
extern void set_var__pump_running(bool value);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/