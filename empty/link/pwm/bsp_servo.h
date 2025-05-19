#ifndef __BSP_SG90_H__
#define __BSP_SG90_H__

#include "../../ti_msp_dl_config.h"

/* *****************************************************************
 * 配置占空比 范围 0 ~ (per-1)
 * t = 0.5ms-------------------舵机会转动 0 °
 * t = 1.0ms-------------------舵机会转动 45°
 * t = 1.5ms-------------------舵机会转动 90°
 * t = 2.0ms-------------------舵机会转动 135°
 * t = 2.5ms-------------------舵机会转动180°
 * 此为SG90（180°Ver）的数据
 * SG90（360°Ver）没有角度限制。
 * 此外这里的函数中angle指定为int32_t是为了方便有存在调节角度的情况（按键按下后angle--）
 ***************************************************************** */

/*
 * 计算PWM占空比
 * 照着[文档](https://wiki.lckfb.com/zh-hans/tmx-mspm0g3507/module/control/sg90-steering-engine.html)
 * 配置之后，频率是50Hz，周期是25ms
 * 不论定时器是TIMG还是TIMA，都是支持PWM的，所以只要保证 f = 50Hz 即可
 */

#define MIN_COUNT 10.0f                   // 0.5ms对应的计数 = 10
#define MAX_COUNT 50.0f                   // 2.5ms对应的计数 = 50
#define RANGE     (MAX_COUNT - MIN_COUNT) // 计数范围

/**
 * @brief Set the angle of SG90 servo motor.
 * @param angle The desired angle in degrees (0° to 180°).
 * @return uint32_t The actual angle set.
 * @warning You should define PORT SG90 first
 */
int32_t Set_SG90_Angle(int32_t angle);

/**
 * @brief Set the angle of SG90S servo motor.
 * @param angle The desired angle in degrees (0° to 360°).
 * @return uint32_t The actual angle set.
 * @warning You should define PORT SG90S first
 */
int32_t Set_SG90S_Angle(int32_t angle);

/**
 * @brief Set the angle of MG90 servo motor.
 * @param angle The desired angle in degrees (0° to 360°).
 * @return uint32_t The actual angle set.
 * @warning You should define PORT MG90 first
 */
// int32_t Set_MG90_Angle(int32_t angle);

/**
 * @brief Set the servo angle.
 * @param ServoAngle The calculated servo angle in timer ticks.
 * @param gptimer The GPTIMER instance.
 * @param ccIndex The capture/compare index.
 */
static void Set_Servo_Angle(uint32_t ServoAngle, GPTIMER_Regs *gptimer, DL_TIMER_CC_INDEX ccIndex);

#endif