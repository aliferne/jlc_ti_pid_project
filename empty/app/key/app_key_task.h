#include "ti_msp_dl_config.h"
#include "mid_button.h"

#ifndef _APP_KEY_TASK_H_
#define _APP_KEY_TASK_H_

/**
 * @file app_key_task.h
 * @brief 按键任务处理头文件
 *
 * 本头文件声明了按键事件回调函数，用于处理不同按键（上、下、左、右、中）的事件。
 * 这些函数会在用户按下或操作按键时被调用，以执行相应的逻辑。
 */

/**
 * @brief 上按键事件回调函数
 *
 * @param btn 指向按键结构体的指针
 */
void btn_up_cb(flex_button_t *btn);

/**
 * @brief 左按键事件回调函数
 *
 * @param btn 指向按键结构体的指针
 */
void btn_left_cb(flex_button_t *btn);

/**
 * @brief 右按键事件回调函数
 *
 * @param btn 指向按键结构体的指针
 */
void btn_right_cb(flex_button_t *btn);

/**
 * @brief 下按键事件回调函数
 *
 * @param btn 指向按键结构体的指针
 */
void btn_down_cb(flex_button_t *btn);

/**
 * @brief 中间按键事件回调函数
 *
 * @param btn 指向按键结构体的指针
 */
void btn_mid_cb(flex_button_t *btn);



#endif // _APP_KEY_TASK_H_
