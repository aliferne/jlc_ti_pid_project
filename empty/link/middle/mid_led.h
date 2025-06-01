/**
 * @file mid_debug_led.h
 * @brief 调试LED中间件头文件
 * @details 提供调试LED的控制接口，包括LED状态管理和基本控制功能
 * @version 1.0
 * @date 2024
 * @author Ferne
 */

#ifndef _MID_DEBUG_LED_H_
#define _MID_DEBUG_LED_H_

#include "ti_msp_dl_config.h"

/**
 * @brief LED状态枚举
 * @details 定义LED的两种基本状态：关闭和开启
 */
typedef enum {
    LED_OFF, /**< LED关闭状态 */
    LED_ON   /**< LED开启状态 */
} LED_STATE_ENUM;

/**
 * @brief 调试LED结构体
 * @details 用于管理调试LED的状态信息
 */
typedef struct {
    LED_STATE_ENUM state; /**< LED当前状态 */
} DEBUG_LED_STRUCT;

/**
 * @brief 打开调试LED
 * @details 将调试LED设置为开启状态
 *
 * @note 该函数会直接操作硬件GPIO，确保相应的GPIO已正确初始化
 */
void set_debug_led_on(void);

/**
 * @brief 关闭调试LED
 * @details 将调试LED设置为关闭状态
 *
 * @note 该函数会直接操作硬件GPIO，确保相应的GPIO已正确初始化
 */
void set_debug_led_off(void);

/**
 * @brief 切换调试LED状态
 * @details 如果LED当前为开启状态则关闭，如果为关闭状态则开启
 *
 * @note 该函数会直接操作硬件GPIO，确保相应的GPIO已正确初始化
 */
void set_debug_led_toggle(void);

/**
 * @brief 获取调试LED当前状态
 * @details 返回调试LED的当前状态
 *
 * @return LED_STATE_ENUM LED的当前状态
 * @retval LED_OFF LED处于关闭状态
 * @retval LED_ON LED处于开启状态
 *
 * @note 该函数返回的是软件记录的状态，可能与实际硬件状态存在差异
 */
LED_STATE_ENUM get_debug_led_state(void);

#endif /* _MID_DEBUG_LED_H_ */
