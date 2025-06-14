#ifndef _APP_UI_H_
#define _APP_UI_H_

/**
 * @file app_ui.h
 * @brief 用户界面绘制与更新
 *
 * 本文件提供在各个文件下定义的页面函数的接口。
 * 包括静态UI的绘制、参数值的显示、选择框的绘制以及波形曲线的绘制等。
 */

#include "app_ui_home.h"
#include "app_ui_settings.h"
#include "app_ui_manual.h"
#include "app_ui_pid.h"

/**
 * @brief 根据选择确定显示哪一个页面
 *
 * @param page 要显示的页面编号
 *
 * 该函数用于根据用户的选择显示相应的页面。
 */
void ui_home_select_page_show(int page);

#endif