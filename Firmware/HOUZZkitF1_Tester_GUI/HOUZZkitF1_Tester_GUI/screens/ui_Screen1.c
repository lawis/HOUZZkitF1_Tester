// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.2.0
// Project name: HOUZZkitF1_Tester_GUI

#include "../ui.h"

void ui_Screen1_screen_init(void)
{
ui_Screen1 = lv_obj_create(NULL);
lv_obj_clear_flag( ui_Screen1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_Screen1, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_Screen1, lv_color_hex(0x2D6298), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Screen1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_lb_version = lv_label_create(ui_Screen1);
lv_obj_set_width( ui_lb_version, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_lb_version, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_lb_version, -93 );
lv_obj_set_y( ui_lb_version, 129 );
lv_obj_set_align( ui_lb_version, LV_ALIGN_CENTER );
lv_label_set_text(ui_lb_version,"Ver. 1.0");
lv_obj_set_style_text_color(ui_lb_version, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_lb_version, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

}