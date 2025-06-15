#ifndef _APP_UI_SETTINGS_H_
#define _APP_UI_SETTINGS_H_

#include "app_ui.h"

// 选择框的线长
#define PID_SETTINGS_SELECT_LINE_LENGTH 10

// PID_ON_UI提示字样的起始 X 坐标
#define PID_ON_UI_STR_X_START 40
// PID_ON_UI提示字样的起始 Y 坐标
#define PID_ON_UI_STR_Y_START 60

#define PID_ON_UI_BOX_X_START 160 // PID_ON_UI设置框的起始 X 坐标
#define PID_ON_UI_BOX_WIDTH   40  // PID_ON_UI设置框的宽度
#define PID_ON_UI_BOX_Y_START 50  // PID_ON_UI设置框的起始 Y 坐标
#define PID_ON_UI_BOX_HEIGHT  30  // PID_ON_UI设置框的高度

/**
 * @brief 展示设置页面
 */
void ui_settings_page_show();

/**
 * @brief 设置页面中负责PID_ON_UI的版块
 */
void ui_settings_page_show_pid_on_ui(const char *pid_on_ui_mode_str);

/**
 * @brief 设置是否使用PID来绘制UI
 *
 * @note 此函数依赖于以下全局变量和函数：
 *
 * - pid_on_ui_mode: 全局布尔变量，表示是否使用PID来绘制UI
 *
 * - ui_settings_page_show_pid_on_ui(): 用于更新UI显示的函数
 *
 * @warning 调用此函数前请确保相关的全局变量已正确初始化
 *
 * @see ui_settings_page_show_pid_on_ui()
 */
void ui_settings_page_change_pid_on_ui();

/**
 * @brief 给当前选择到的选项画一个外框
 */
void ui_settings_parameter_page_select_box(int mode);

/**
 * @brief 给当前选择到的选项画一个粗框，表示选中
 */
void ui_settings_parameter_page_select_box_bold(int mode); // 参数选择框加粗，即选中框

#endif