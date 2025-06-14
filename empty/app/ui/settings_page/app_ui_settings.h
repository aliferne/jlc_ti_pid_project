#ifndef _APP_UI_SETTINGS_H_
#define _APP_UI_SETTINGS_H_

#include "app_ui.h"

void ui_settings_page_show();
void ui_settings_page_show_pid_on_ui(const char *pid_on_ui_mode_str);
void ui_settings_page_change_pid_on_ui();
void ui_settings_page_select_box(int mode);
void ui_settings_parameter_select_box_bold(int mode); // 参数选择框加粗，即选中框

#endif