#ifndef __MID_PID_H__
#define __MID_PID_H__

typedef struct
{
    float kp, ki, kd;                   // 三个静态系数
    float change_p, change_i, change_d; // 三个动态参数
    float error, last_error;            // 误差、之前误差
    float max_change_i;                 // 积分限幅
    float output, max_output;           // 输出、输出限幅
    int target;                         // 目标
} PID_Struct;

// 串级PID的结构体，包含两个单级PID
typedef struct
{
    PID_Struct inner; // 内环
    PID_Struct outer; // 外环
    float output;     // 串级输出
} CascadePID_Struct;

void pid_init(PID_Struct *pid, float p, float i, float d, float maxI, float maxOut, int target);
void pid_change_zero(PID_Struct *pid);
float pid_calc(PID_Struct *pid, float target, float current);
void pid_cascade_calc(CascadePID_Struct *pid, float outerRef, float outerFdb, int innerFdb);

#endif