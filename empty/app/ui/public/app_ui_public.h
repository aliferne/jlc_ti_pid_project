#ifndef _APP_UI_PUBLIC_H_
#define _APP_UI_PUBLIC_H_

/**
 * @file app_ui_public.h
 * 
 * @brief 本文件是所有页面会使用到的公共依赖部分
 * @brief 主要包括UI界面盒子的绘制、选项框的绘制
 */

#include "app_distance_pid.h"
#include "app_speed_pid.h"
#include "app_sys_mode.h"
#include "hw_lcd.h"
#include "hw_encoder.h"
#include "mid_timer.h"
#include "string.h"
#include "stdio.h"
#include "ti_msp_dl_config.h"

#define screen_center_x (LCD_W / 2) // 屏幕中心X = 屏幕x/2
#define screen_center_y (LCD_H / 2) // 屏幕中心Y = 屏幕y/2

// home_page中的固定参数
#define PID_SELECT_BOX_HEIGHT    40 // 高度
#define PID_SELECT_LINE_LEN      10 // 外圈选择框线长度
#define PID_SELECT_LINE_INTERVAL 5  // 外圈选择框线与选择框间隔

// SPEED选项框绘制参数（ui_home_page中调用）
#define PID_SPEED_SELECT_BOX_X_START 40 // 起始x坐标
#define PID_SPEED_SELECT_BOX_WIDTH   80 // 宽度
#define PID_SPEED_SELECT_BOX_Y_START 65 // 起始y坐标

// DISTANCE选项框绘制参数（ui_home_page中调用）
#define PID_DISTANCE_SELECT_BOX_X_START    40  // 起始x坐标
#define PID_DISTANCE_SELECT_BOX_WIDTH      80  // 宽度
#define PID_DISTANCE_SELECT_BOX_Y_START    120 // 起始y坐标

#define PID_MANUAL_SELECT_BOX_X_START      (PID_SPEED_SELECT_BOX_X_START + 140) // 起始x坐标
#define PID_MANUAL_SELECT_BOX_Y_START      PID_SPEED_SELECT_BOX_Y_START         // 起始y坐标
#define PID_MANUAL_SELECT_BOX_WIDTH        PID_SPEED_SELECT_BOX_WIDTH           // 宽度

#define PID_SETTINGS_SELECT_BOX_X_START    PID_MANUAL_SELECT_BOX_X_START   // 起始x坐标
#define PID_SETTINGS_SELECT_BOX_Y_START    PID_DISTANCE_SELECT_BOX_Y_START // 起始y坐标
#define PID_SETTINGS_SELECT_BOX_WIDTH      PID_DISTANCE_SELECT_BOX_WIDTH   // 宽度

#define PID_MANUAL_STRINGS_X_START         40
#define PID_MANUAL_STRINGS_Y_START         40
#define PID_MANUAL_STRINGS_Y_INTERVAL      25

#define FONTSIZE                           16        // PID 字体大小
#define DEFAULT_PID_TITLE_Y_POSITION       87        // 默认Y轴起始位置
#define DEFAULT_CENTER_X_OFFSET            84        // 默认X轴偏移量
#define PID_NEXT_Y_START_POSITION          (60 + 44) // PID参数Y轴起始位置

#define SPEED_WAVEFORM_REDUCTION_FACTOR    2.3 // 定速波形衰减倍数
#define DISTANCE_WAVEFORM_REDUCTION_FACTOR 8.3 // 定距波形衰减倍数

#define ABS(x)                             ((x) > 0 ? (x) : -(x)) // 取绝对值

// 该结构体用于PID控制
typedef struct {
    unsigned int start_x;
    unsigned int start_y;
    unsigned int end_x;
    unsigned int end_y;
    unsigned int center;
} Motor_PID_Controller;

/**
 * @brief 在屏幕中心绘制带圆角背景的字符串
 *
 * 该函数用于在屏幕中心绘制一个带圆角背景的字符串。字符串的位置会根据其长度和字体大小自动调整，以确保它在屏幕中心。
 *
 * @param y 字符串起始Y轴坐标
 * @param str_len 字符串长度
 * @param bc 圆角矩形背景色
 * @param fontSize 字符像素大小
 * @param str 要显示的字符串
 *
 * @note 该函数假设屏幕中心坐标 `screen_center_x` 已经被正确定义。
 * @note 字符串的长度不应超过屏幕的宽度，以避免绘制超出屏幕边界。
 * @note 字符串的字体大小应与屏幕的尺寸相匹配，以确保字符串能够完全显示在屏幕内。
 * @note 背景颜色可以是预定义的颜色常量，如 GRAYBLUE 或 DARKBLUE。
 */
void show_x_center(int y, int str_len, uint16_t bc, unsigned char fontSize, unsigned char *str);

/**
 * @brief 绘制彩色填充矩形，带居中的字符串
 *
 * 该函数用于在指定的位置绘制一个彩色填充的矩形，并在矩形内部居中显示一个字符串。
 *
 * @param x 矩形起始X轴坐标
 * @param w 矩形宽度
 * @param y 矩形起始Y轴坐标
 * @param h 矩形高度
 * @param str_len 字符串长度
 * @param fontSize 字符像素大小
 * @param str 要显示的字符串
 * @param color 矩形背景色
 *
 * @note 该函数假设输入的坐标、宽度和高度都是有效的，且不会导致绘制超出屏幕边界。
 * @note 字符串的长度不应超过矩形的宽度。
 * @note 字符串的字体大小应与矩形的尺寸相匹配，以确保字符串能够完全显示在矩形内。
 * @note 背景颜色可以是预定义的颜色常量，如 GRAYBLUE 或 DARKBLUE。
 */
void show_string_rect(int x, int w, int y, int h, int str_len, int fontSize, unsigned char *str, int color);

/**
 * @brief 绘制水平线框
 *
 * 该函数用于在指定区域绘制一组水平线，用于构成线框的一部分。
 *
 * @param x 线框的起始X坐标
 * @param w 线框的宽度
 * @param y 线框的起始Y坐标
 * @param h 线框的高度
 * @param line_length 每条线的长度
 * @param interval 线条之间的间隔
 * @param color 线条的颜色
 */
void ui_draw_lines_horizonal(
    int x, int w, int y, int h,
    int line_length, int interval, int color);

/**
 * @brief 绘制竖直线框
 *
 * 该函数用于在指定区域绘制一组竖直线，用于构成线框的一部分。
 *
 * @param x 线框的起始X坐标
 * @param w 线框的宽度
 * @param y 线框的起始Y坐标
 * @param h 线框的高度
 * @param line_length 每条线的长度
 * @param interval 线条之间的间隔
 * @param color 线条的颜色
 */
void ui_draw_lines_vertical(
    int x, int w, int y, int h,
    int line_length, int interval, int color);

/**
 * @brief 显示选择框
 *
 * 该函数用于在指定位置绘制一个选择框，支持静态显示和PID动画显示两种模式。
 * 选择框由水平线和垂直线组成，可以设置线条长度、间距和颜色。
 *
 * @param target_x 目标X坐标位置
 * @param w 选择框的宽度
 * @param target_y 目标Y坐标位置
 * @param h 选择框的高度
 * @param line_length 线条的长度
 * @param interval 线条间距，同时也作为选择框向外扩展的距离
 * @param color 线条颜色
 *
 * @details
 * 函数内部有两种显示模式：
 * - mode = 0：静态显示模式，直接在目标位置绘制选择框
 * - mode ≠ 0：PID动画模式，使用PID控制算法实现选择框从当前位置平滑移动到目标位置
 *
 * 在PID模式下：
 * - 使用PID参数：kp=0.4, ki=0.2, kd=0.2
 * - 动画过程中会先清除上一帧的线条，再绘制新位置的线条
 * - 当X轴和Y轴误差都小于0.5时停止动画
 * - 动画结束后会重置PID参数
 *
 * @note
 * - 选择框的实际绘制区域会比输入的w和h各边扩大interval个像素
 * - 需要依赖ui_draw_lines_horizonal()和ui_draw_lines_vertical()函数
 * - PID模式需要pid_init(), pid_calc(), pid_change_zero()等PID相关函数
 * - 需要定义LCD_W, LCD_H, BLACK等常量
 *
 * @see ui_draw_lines_horizonal()
 * @see ui_draw_lines_vertical()
 * @see pid_init()
 * @see pid_calc()
 * @see pid_change_zero()
 */
void show_select_box(
    int target_x, int w, int target_y, int h,
    int line_length, int interval, int color);

#endif