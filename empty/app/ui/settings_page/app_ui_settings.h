#ifndef _APP_UI_SETTINGS_H_
#define _APP_UI_SETTINGS_H_

#include "app_ui.h"

// 选择框的线长
#define PID_SETTINGS_SELECT_LINE_LENGTH 10

// PID_ON_UI提示字样的起始 X 坐标
#define PID_ON_UI_STR_X_START 40
// PID_ON_UI提示字样的起始 Y 坐标
#define PID_ON_UI_STR_Y_START 40
// PID_ON_UI设置框的起始 X 坐标
#define PID_ON_UI_BOX_X_START 200
// PID_ON_UI设置框的宽度
#define PID_ON_UI_BOX_WIDTH 40
// PID_ON_UI设置框的起始 Y 坐标
#define PID_ON_UI_BOX_Y_START 20
// PID_ON_UI设置框的高度
#define PID_ON_UI_BOX_HEIGHT 40

#define STR_INTERVAL 45 // 文字之间的间距
#define BOX_INTERVAL 50 // 选项框之间的间隔

// 操纵杆提示字样的起始 X 坐标，与PID_ON_UI提示字样相同
#define STICK_STR_X_START PID_ON_UI_STR_X_START
// 操纵杆提示字样的起始 Y 坐标，距离PID_ON_UI提示字样STR_INTERVAL个单位
#define STICK_STR_Y_START (PID_ON_UI_STR_Y_START + STR_INTERVAL)
// 操纵杆设置框的起始 X 坐标，与PID_ON_UI设置框相同
#define STICK_BOX_X_START PID_ON_UI_BOX_X_START
// 操纵杆设置框的起始 Y 坐标，距离PID_ON_UI设置框BOX_INTERVAL个单位
#define STICK_BOX_Y_START (PID_ON_UI_BOX_Y_START + BOX_INTERVAL)
// 操纵杆设置框的宽度，与PID_ON_UI设置框相同
#define STICK_BOX_WIDTH PID_ON_UI_BOX_WIDTH
// 操纵杆设置框的高度，与PID_ON_UI设置框相同
#define STICK_BOX_HEIGHT PID_ON_UI_BOX_HEIGHT

// ASRPRO TTS提示字样的起始 X 坐标，与PID_ON_UI提示字样相同
#define ASRPRO_TTS_STR_X_START PID_ON_UI_STR_X_START
// ASRPRO TTS提示字样的起始 Y 坐标，距离操纵杆提示字样STR_INTERVAL个单位
#define ASRPRO_TTS_STR_Y_START (STICK_STR_Y_START + STR_INTERVAL)
// ASRPRO TTS设置框的起始 X 坐标，与PID_ON_UI设置框相同
#define ASRPRO_TTS_BOX_X_START PID_ON_UI_BOX_X_START
// ASRPRO TTS设置框的起始 Y 坐标，距离操纵杆设置框BOX_INTERVAL个单位
#define ASRPRO_TTS_BOX_Y_START (STICK_BOX_Y_START + BOX_INTERVAL)
// ASRPRO TTS设置框的宽度，与PID_ON_UI设置框相同
#define ASRPRO_TTS_BOX_WIDTH PID_ON_UI_BOX_WIDTH
// ASRPRO TTS设置框的高度，与PID_ON_UI设置框相同
#define ASRPRO_TTS_BOX_HEIGHT PID_ON_UI_BOX_HEIGHT

/**
 * @brief 展示设置页面
 */
void ui_settings_page_show();

/**
 * @brief 更新设置页面上的参数显示块
 *
 * @param str 参数显示的字符串
 * @param str_x_start 字符串显示的起始X坐标
 * @param str_y_start 字符串显示的起始Y坐标
 * @param str_len 字符串的长度
 * @param box_x_start 设置框的起始X坐标
 * @param box_width 设置框的宽度
 * @param box_y_start 设置框的起始Y坐标
 * @param box_height 设置框的高度
 * @param box_color 设置框的颜色
 * @param mode_str 模式字符串
 */
void ui_settings_page_show_parameters_block(
    const char *str,
    int str_x_start,
    int str_y_start,
    int str_len,
    int box_x_start,
    int box_width,
    int box_y_start,
    int box_height,
    int box_color,
    const char *mode_str);

/**
 * @brief 设置是否使用PID来绘制UI
 *
 * @note 此函数依赖于以下全局变量和函数：
 *
 * - pid_on_ui_mode: 全局布尔变量，表示是否使用PID来绘制UI
 *
 * - ui_settings_page_show_parameters_block(): 用于更新UI显示的函数
 *
 * @warning 调用此函数前请确保相关的全局变量已正确初始化
 *
 * @see ui_settings_page_show_parameters_block()
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