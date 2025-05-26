#ifndef __APP_DISTANCE_PID_H__
#define __APP_DISTANCE_PID_H__

#include "mid_pid.h"

#define MOTOR_REDUCTION_RATIO 48       // 电机减速比 1：48
#define ENCODER_RESOLUTION    1248     // 电机一圈的编码器脉冲数 1248脉冲/转
#define DEGREES_PER_PULSE     0.28846f // 每脉冲度数 360 / ENCODER_RESOLUTION
#define DEFAULT_ANGLE         90       // 起始默认旋转角度


void distance_pid_init(void);
PID_Struct *get_distance_pid(void);
int get_distance_pid_target(void);
PID_Struct motor_distance_control(int target_angle);
void set_distance_pid_parameter(PID_Struct *pid_value, int select, int add_or_subtract_flag);
#endif
