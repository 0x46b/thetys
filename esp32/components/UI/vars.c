#include "vars.h"
#include <stdint.h>
static uint32_t humidity = 0;

const char *get_var__plant_id() { return "PFLANZE 1"; }

void set_var__plant_id(const char *value) {}
int32_t get_var__humidity_level() { return humidity; }
void set_var__humidity_level(int32_t value) { humidity = 4095 - value; }
bool get_var__pump_running() { return false; }
void set_var__pump_running(bool value) {}
