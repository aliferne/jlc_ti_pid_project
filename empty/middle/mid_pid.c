#include "mid_pid.h"

void pid_init(PID_Struct *pid, float p, float i, float d, float maxI, float maxOut, int target)
{
    pid->kp           = p;
    pid->ki           = i;
    pid->kd           = d;
    pid->max_change_i = maxI;
    pid->max_output   = maxOut;
    pid->target       = target;
}

void pid_change_zero(PID_Struct *pid)
{
    pid->change_p = 0;
    pid->change_i = 0;
    pid->change_d = 0;
}

float pid_calc(PID_Struct *pid, float target, float current)
{
    pid->last_error = pid->error;       // 更新之前误差
    pid->error      = target - current; // 计算新的误差

    pid->change_p = pid->error;                   // P
    pid->change_i += pid->error;                  // I
    pid->change_d = pid->error - pid->last_error; // D

    if (pid->change_i > pid->max_change_i) {
        pid->change_i = pid->max_change_i; // 限制积分I
    } else if (pid->change_i < -(pid->max_change_i)) {
        pid->change_i = -(pid->max_change_i); // 限制积分I
    }

    // PID输出
    pid->output = (pid->kp * pid->change_p) + (pid->ki * pid->change_i) + (pid->kd * pid->change_d);

    if (pid->output > pid->max_output) {
        pid->output = pid->max_output; // 限制输出
    } else if (pid->output < -(pid->max_output)) {
        pid->output = -(pid->max_output); // 限制输出
    }

    return pid->output;
}

void pid_cascade_calc(CascadePID_Struct *pid, float outerRef, float outerFdb, int innerFdb)
{
    pid_calc(&pid->outer, outerRef, outerFdb);          // 计算外环
    pid_calc(&pid->inner, pid->outer.output, innerFdb); // 计算内环
    // 内环输出就是串级PID的输出
    pid->output = pid->inner.output;
}
