#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_OVERVIEW_PAGE = 1,
    SCREEN_ID_PLANT_DETAIL_PAGE = 2,
    _SCREEN_ID_LAST = 2
};

typedef struct _objects_t {
    lv_obj_t *overview_page;
    lv_obj_t *plant_detail_page;
    lv_obj_t *plant_detail_chart;
    lv_obj_t *bar_humidity_level;
    lv_obj_t *txt_plant_name;
    lv_obj_t *txt_configured_threshold;
    lv_obj_t *txt_last_measured_at;
    lv_obj_t *txt_last_watered_at;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
} objects_t;

extern objects_t objects;

void create_screen_overview_page();
void tick_screen_overview_page();

void create_screen_plant_detail_page();
void tick_screen_plant_detail_page();

void create_user_widget_plant_view(lv_obj_t *parent_obj, int startWidgetIndex);
void tick_user_widget_plant_view(int startWidgetIndex);

void create_user_widget_sensor_list_item(lv_obj_t *parent_obj, int startWidgetIndex);
void tick_user_widget_sensor_list_item(int startWidgetIndex);

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/