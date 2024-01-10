// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.2.0
// Project name: HOUZZkitF1_Tester_GUI

#ifndef _HOUZZKITF1_TESTER_GUI_UI_H
#define _HOUZZKITF1_TESTER_GUI_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"
void arrowshake_Animation(lv_obj_t * TargetObject, int delay);
// SCREEN: ui_MainScreen
void ui_MainScreen_screen_init(void);
extern lv_obj_t * ui_MainScreen;
extern lv_obj_t * ui_lb_version;
extern lv_obj_t * ui_lb_sn_code;
extern lv_obj_t * ui_lb_title;
extern lv_obj_t * ui_img_arrow;
extern lv_obj_t * ui_lb_device_info;
extern lv_obj_t * ui_lb_firmware;
extern lv_obj_t * ui_lb_error_code;
extern lv_obj_t * ui_lb_wan_mac;
extern lv_obj_t * ui_lb_lan_mac;
extern lv_obj_t * ui_pb_progress;
extern lv_obj_t * ui____initial_actions0;

LV_IMG_DECLARE(ui_img_arrow_png);    // assets\arrow.png

LV_FONT_DECLARE(ui_font_SYHeiH44);
LV_FONT_DECLARE(ui_font_SYHeiH18);

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif