#ifndef _APP_UI_PID_H_
#define _APP_UI_PID_H_

/**
 * @file app_ui_pid.h
 * @brief PID控制界面用户接口模块
 * 
 * 本文件提供了PID控制系统的用户界面相关函数声明，包括定速控制和定距控制
 * 两种模式的界面绘制、参数显示、选择框控制以及实时曲线绘制功能。
 * 
 * 主要功能模块：
 * - 定速页面UI绘制和参数显示
 * - 定距页面UI绘制和参数显示  
 * - 参数选择框的交互控制
 * - 实时速度/距离曲线的动态绘制
 * 
 * @author Ferne
 * @date 2025
 * @version 1.0
 */

#include "app_ui_public.h"

/**
 * @brief 绘制定速页静态UI
 *
 * 该函数用于绘制定速页的静态用户界面元素。
 */
void ui_speed_page(void);

/**
 * @brief 绘制定速页参数值的变化
 *
 * @param p P参数值
 * @param i I参数值
 * @param d D参数值
 * @param speed 当前速度
 * @param target 目标速度
 * @param quick_update 是否快速更新
 *
 * 该函数用于在定速页上绘制参数值的变化。
 */
void ui_speed_page_value_set(float p, float i, float d, int speed, int target, int quick_update);

/**
 * @brief 绘制选择框
 *
 * @param mode 选择框的模式
 *
 * 该函数用于在定速页上绘制选择框，根据不同的模式显示不同的选择框。
 */
void ui_pid_page_select_box(int mode);

/**
 * @brief 绘制定距页静态UI
 *
 * 该函数用于绘制定距页的静态用户界面元素。
 */
void ui_distance_page(void);

/**
 * @brief 绘制定距页参数值的变化
 *
 * @param p P参数值
 * @param i I参数值
 * @param d D参数值
 * @param distance 当前距离
 * @param target 目标距离
 * @param quick_update 是否快速更新
 *
 * 该函数用于在定距页上绘制参数值的变化。
 */
void ui_distance_page_value_set(float p, float i, float d, int distance, int target, int quick_update);

/**
 * @brief 参数选择框加粗，即选中框
 *
 * @param mode 参数选择框的模式
 *
 * 该函数用于将参数选择框加粗，表示该框已被选中。
 *
 * @note 请使用文件中已定义好的宏： `SPEED_PAGE`,`DISTANCE_PAGE`,`HOME_PAGE`
 */
void ui_pid_parameter_select_box_bold(int mode);

/**
 * @brief 绘制速度曲线
 *
 * @param window_start_x 曲线窗口的起始X坐标
 * @param window_start_y 曲线窗口的起始Y坐标
 * @param window_w 曲线窗口的宽度
 * @param window_h 曲线窗口的高度
 * @param curve_color 曲线的颜色
 * @param background_color 背景颜色
 * @param rawValue 原始数据值
 * @return 绘制结果状态
 *
 * 该函数用于在指定窗口内绘制速度曲线。
 */
uint16_t draw_speed_curve(int window_start_x, int window_start_y, int window_w, int window_h, int curve_color, int background_color, short int rawValue);

/**
 * @brief 绘制距离曲线
 *
 * @param window_start_x 曲线窗口的起始X坐标
 * @param window_start_y 曲线窗口的起始Y坐标
 * @param window_w 曲线窗口的宽度
 * @param window_h 曲线窗口的高度
 * @param curve_color 曲线的颜色
 * @param background_color 背景颜色
 * @param rawValue 原始数据值
 * @return 绘制结果状态
 *
 * 该函数用于在指定窗口内绘制距离曲线。
 */
uint16_t draw_distance_curve(int window_start_x, int window_start_y, int window_w, int window_h, int curve_color, int background_color, short int rawValue);

/**
 * @brief 绘制速度曲线界面
 *
 * 该函数用于在用户界面上绘制速度曲线，包括当前编码器数值曲线和目标速度的波形点。
 * 在绘制过程中，禁止任务调度以避免界面刷新时的闪烁或异常。
 *
 * @param None
 * @return None
 */
void ui_speed_curve();

/**
 * @brief 绘制距离曲线界面
 *
 * 该函数用于在用户界面上绘制距离曲线，包括当前编码器数值曲线和目标距离的波形点。
 * 在绘制过程中，禁止任务调度以避免界面刷新时的闪烁或异常。
 *
 * @param None
 * @return None
 */
void ui_distance_curve(void);

#endif
