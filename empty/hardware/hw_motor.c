#include "hw_motor.h"
#include "mid_debug_uart.h"


// 设置fi引脚的PWM比较值
static void set_fi(uint16_t dat)
{
    DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST, dat, GPIO_PWM_MOTOR_C1_IDX);
}
// 设置bi引脚的PWM比较值
static void set_bi(uint16_t dat)
{
    DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST, dat, GPIO_PWM_MOTOR_C0_IDX);
}

// 限制输入的PWM最大值
// 仅内部使用
static void restrict_pwm_max_value(uint16_t *value)
{
    if (*value > MOTOR_PWM_MAX) {
        *value = MOTOR_PWM_MAX;
    }
}

// 设置电机速度
void set_motor(uint16_t fi_value, uint16_t bi_value)
{
    restrict_pwm_max_value(&fi_value);
    restrict_pwm_max_value(&bi_value);
    
    set_fi(fi_value);
    set_bi(bi_value);
}

// 电机停止
void stop_motor(void)
{
    set_motor(0, 0);
}
