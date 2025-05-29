#include "app_distance_pid.h"
#include "hw_encoder.h"
#include "hw_motor.h"

PID_Struct distance_pid;

void distance_pid_init(void)
{
    pid_init(
        &distance_pid, DEFAULT_DISTANCE_PID_KP,
        DEFAULT_DISTANCE_PID_KI,
        DEFAULT_DISTANCE_PID_KD,
        MOTOR_PWM_MAX, MOTOR_PWM_MAX, DEFAULT_DISTANCE_PID_ANGLE);
}

PID_Struct *get_distance_pid(void)
{
    return &distance_pid;
}

int get_distance_pid_target(void)
{
    return distance_pid.target;
}

PID_Struct motor_distance_control(int target_angle)
{
    int PWM, target_pulses;

    target_pulses = target_angle / DEGREES_PER_PULSE; // 计算目标脉冲数
    // 传入PID静态参数、目标值（目标脉冲数）、当前值（当前编码器获取的实时脉冲数）
    // PID输出的值为控制信号，传入到PWM变量中
    PWM = pid_calc(&distance_pid, target_pulses, get_temp_encoder());

    // 控制刷新速度
    delay_cycles(80000 * 5);
    set_motor_value(PWM);

    return distance_pid;
}

void set_distance_pid_parameter(PID_Struct *pid_value, int select, int add_or_subtract_flag)
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
            case MODIFY_ANGLE:
                if (pid_value->target < ANGLE_MAX) // 限制目标值最大为100
                    pid_value->target += MODIFY_TARGET_STEP;
                break;
            default:
                break;
        }
    } else {
        switch (select) {
            case MODIFY_P:
                pid_value->kp -= MODIFY_PID_STEP;
                pid_value->kp = (pid_value->kp <= -0.0f) ? 0.0f : pid_value->kp; // 消除负零
                break;
            case MODIFY_I:
                pid_value->ki -= MODIFY_PID_STEP;
                pid_value->ki = (pid_value->ki <= -0.0f) ? 0.0f : pid_value->ki; // 消除负零
                break;
            case MODIFY_D:
                pid_value->kd -= MODIFY_PID_STEP;
                pid_value->kd = (pid_value->kd <= -0.0f) ? 0.0f : pid_value->kd; // 消除负零
                break;
            case MODIFY_ANGLE:
                if (pid_value->target > ANGLE_MIN) // 限制目标值最小为-360
                    pid_value->target -= MODIFY_TARGET_STEP;
                break;
            default:
                break;
        }
    }
}
