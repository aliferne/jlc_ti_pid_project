#ifndef _HW_MOTOR_H_
#define _HW_MOTOR_H_

#include "ti_msp_dl_config.h"

/**
 * @brief 电机最大输入的PWM值
 */
#define MOTOR_PWM_MAX 9999

/**
 * @brief 设置电机值
 * 
 * @param value 电机输入值，范围从 -MOTOR_PWM_MAX 到 MOTOR_PWM_MAX
 */
void set_motor_value(int16_t value);

/**
 * @brief 设置电机的前后值
 * 
 * @param fi_value 电机的前值
 * @param bi_value电机的后值
 */
void set_motor_fi_bi_value(int16_t fi_value, int16_t bi_value);

/**
 * @brief 停止电机
 */
void stop_motor(void);

#endif
