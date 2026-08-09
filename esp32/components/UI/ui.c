#include "ui.h"
#include "screens.h"
#include "images.h"
#include "actions.h"
#include "vars.h"
#include <esp_lvgl_port.h>

#include <string.h>

static int16_t currentScreen = -1;

static lv_obj_t *getLvglObjectFromIndex(int32_t index) {
    if (index == -1) {
        return 0;
    }
    return ((lv_obj_t **)&objects)[index];
}

void loadScreen(enum ScreensEnum screenId) {
    currentScreen = screenId - 1;
    lv_obj_t *screen = getLvglObjectFromIndex(currentScreen);
    lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_FADE_IN, 200, 0, false);
}

static void eez_ui_timer_cb(lv_timer_t * timer)
{
    ui_tick();
}

void ui_init() {
    create_screens();
    if (lvgl_port_lock(0)) {
        // Create an LVGL timer running every 10ms to execute ui_tick
        lv_timer_create(eez_ui_timer_cb, 10, NULL); 
        
        lvgl_port_unlock();
    }
    loadScreen(SCREEN_ID_MAIN);

}

void ui_tick() {
    if (lvgl_port_lock(0)) {
        tick_screen(currentScreen);
        lvgl_port_unlock();
    }
}