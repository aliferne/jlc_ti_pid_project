/**
 * @file hw_motor.c
 * @brief 电机控制相关函数
 */

#include "hw_motor.h"

/**
 * @brief 设置fi引脚的PWM比较值
 * @param dat PWM值
 */
static inline void set_fi(uint16_t dat)
{
    DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST, dat, GPIO_PWM_MOTOR_C1_IDX);
}

/**
 * @brief 设置bi引脚的PWM比较值
 * @param dat PWM值
 */
static inline void set_bi(uint16_t dat)
{
    DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST, dat, GPIO_PWM_MOTOR_C0_IDX);
}

/**
 * @brief 限制输入的PWM最大值
 * @param value 指向PWM值的指针
 */
static void restrict_pwm_max_value(int16_t *value)
{
    if (*value > MOTOR_PWM_MAX) {
        *value = MOTOR_PWM_MAX;
    } else if (*value < -MOTOR_PWM_MAX) {
        *value = -MOTOR_PWM_MAX;
    }
}

/**
 * @brief 设置电机速度，需要传入一个速度值
 * @param value 速度值，可正可负，正为顺时针，负为逆时针
 */
void set_motor_value(int16_t value)
{
    restrict_pwm_max_value(&value);

    if (value > 0) { // 顺时针
        set_fi(0);
        set_bi(value);
    } else { // 逆时针
        set_fi(-value);
        set_bi(0);
    }
}

/**
 * @brief 设置电机fi和bi引脚的PWM值
 * @param fi_value fi引脚的PWM值
 * @param bi_value bi引脚的PWM值
 */
void set_motor_fi_bi_value(int16_t fi_value, int16_t bi_value)
{
    restrict_pwm_max_value(&fi_value);
    restrict_pwm_max_value(&bi_value);

    set_fi(fi_value);
    set_bi(bi_value);
}

/**
 * @brief 停止电机
 */
void stop_motor(void)
{
    set_motor_fi_bi_value(0, 0);
}
