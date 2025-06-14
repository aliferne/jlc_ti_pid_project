#ifndef _APP_UI_HOME_H_
#define _APP_UI_HOME_H_

#include "app_ui_public.h"

/**
 * @brief 显示主页面界面
 *
 * 该函数负责绘制主页面的完整界面，包括：
 * - 设置黑色背景
 * - 显示标题文字（立创开发板、简易PID入门套件、Ferne）
 * - 绘制四个选择框（Speed、Distance、Manual、Settings）
 * - 根据当前选择状态高亮显示对应选项
 * - 控制LCD背光的开关以提供更好的视觉体验
 *
 * @note 函数会先关闭背光再重新打开，以提供更好的刷新视觉效果
 */
void ui_home_page(void);

/**
 * @brief 绘制主页面选择框
 *
 * 该函数用于绘制四个选项的选择框，可以为每个选项设置不同的颜色
 * 来表示选中状态。通常被 ui_home_page_select() 函数调用。
 *
 * @param speed_color    Speed选项的选择框颜色
 * @param distance_color Distance选项的选择框颜色
 * @param manual_color   Manual选项的选择框颜色
 * @param settings_color Settings选项的选择框颜色
 *
 * @note 选择框大小固定为5个像素宽度
 */
void ui_home_page_draw_select_box(
    int speed_color,
    int distance_color,
    int manual_color,
    int settings_color);

/**
 * @brief 根据模式绘制主页面选择状态
 *
 * 该函数根据传入的模式参数，高亮显示对应的选项，
 * 其他选项则显示为未选中状态（黑色）。
 *
 * @param mode 选择模式，可选值：
 *             - SPEED_PAGE: 高亮显示Speed选项
 *             - DISTANCE_PAGE: 高亮显示Distance选项
 *             - MANUAL_PAGE: 高亮显示Manual选项
 *             - SETTINGS_PAGE: 高亮显示Settings选项
 *
 * @see SPEED_PAGE, DISTANCE_PAGE, MANUAL_PAGE, SETTINGS_PAGE
 */
void ui_home_page_select(int mode);

#endif /* APP_UI_HOME_H */
