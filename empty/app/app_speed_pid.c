#include "app_speed_pid.h"
#include "hw_encoder.h"
#include "hw_motor.h"

PID_Struct speed_pid;

void speed_pid_init(void)
{
    pid_init(
        &speed_pid, DEFAULT_SPEED_PID_KP, DEFAULT_SPEED_PID_KI,
        DEFAULT_SPEED_PID_KD,
        MOTOR_PWM_MAX, MOTOR_PWM_MAX, DEFAULT_SPEED_PID_TARGET);
}

PID_Struct *get_speed_pid(void)
{
    return &speed_pid;
}

int get_speed_pid_target(void)
{
    return speed_pid.target;
}

PID_Struct motor_speed_control(int target)
{
    int PWM;

    PWM = pid_calc(&speed_pid, target, get_encoder_count());

    // 控制刷新速度
    delay_cycles(80000 * 5);

    set_motor_value(PWM);

    return speed_pid;
}

void set_speed_pid_parameter(PID_Struct *pid_value, int select, int add_or_subtract_flag)
{
    if (add_or_subtract_flag == ADD) {
        switch (select) {
            case MODIFY_P:
                pid_value->kp += MODIFY_PID_STEP;
                break;
            case MODIFY_I:
                pid_value->ki += MODIFY_PID_STEP;
                break;
            case MODIFY_D:
                pid_value->kd += MODIFY_PID_STEP;
                break;
            case MODIFY_TARGET:
                if (pid_value->target < TARGET_MAX) // 限制目标值最大为100
                    pid_value->target += MODIFY_TARGET_STEP;
                break;
            default:
                break;
        }
    } else {
        switch (select) {
            case MODIFY_P:
                if (pid_value->kp > 0) // 限制不为负数
                    pid_value->kp -= MODIFY_PID_STEP;
                pid_value->kp = (pid_value->kp <= -0.0f) ? 0.0f : pid_value->kp; // 消除负零
                break;
            case MODIFY_I:
                if (pid_value->ki > 0) // 限制不为负数
                    pid_value->ki -= MODIFY_PID_STEP;
                pid_value->ki = (pid_value->ki <= -0.0f) ? 0.0f : pid_value->ki; // 消除负零
                break;
            case MODIFY_D:
                if (pid_value->kd > 0) // 限制不为负数
                    pid_value->kd -= MODIFY_PID_STEP;
                pid_value->kd = (pid_value->kd <= -0.0f) ? 0.0f : pid_value->kd; // 消除负零
                break;
            case MODIFY_TARGET:
                if (pid_value->target > TARGET_MIN) // 限制目标值最小为-100
                    pid_value->target -= MODIFY_TARGET_STEP;
                break;
            default:
                break;
        }
    }
}