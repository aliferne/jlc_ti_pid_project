/**
 * @file hw_motor.c
 * @brief ���������غ���
 */

#include "hw_motor.h"

/**
 * @brief ����fi���ŵ�PWM�Ƚ�ֵ
 * @param dat PWMֵ
 */
static inline void set_fi(uint16_t dat)
{
    DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST, dat, GPIO_PWM_MOTOR_C1_IDX);
}

/**
 * @brief ����bi���ŵ�PWM�Ƚ�ֵ
 * @param dat PWMֵ
 */
static inline void set_bi(uint16_t dat)
{
    DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST, dat, GPIO_PWM_MOTOR_C0_IDX);
}

/**
 * @brief ���������PWM���ֵ
 * @param value ָ��PWMֵ��ָ��
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
 * @brief ���õ���ٶȣ���Ҫ����һ���ٶ�ֵ
 * @param value �ٶ�ֵ�������ɸ�����Ϊ˳ʱ�룬��Ϊ��ʱ��
 */
void set_motor_value(int16_t value)
{
    restrict_pwm_max_value(&value);

    if (value > 0) { // ˳ʱ��
        set_fi(0);
        set_bi(value);
    } else { // ��ʱ��
        set_fi(-value);
        set_bi(0);
    }
}

/**
 * @brief ���õ��fi��bi���ŵ�PWMֵ
 * @param fi_value fi���ŵ�PWMֵ
 * @param bi_value bi���ŵ�PWMֵ
 */
void set_motor_fi_bi_value(int16_t fi_value, int16_t bi_value)
{
    restrict_pwm_max_value(&fi_value);
    restrict_pwm_max_value(&bi_value);

    set_fi(fi_value);
    set_bi(bi_value);
}

/**
 * @brief ֹͣ���
 */
void stop_motor(void)
{
    set_motor_fi_bi_value(0, 0);
}
