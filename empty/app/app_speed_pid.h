#ifndef __APP_SPEED_PID_H__
#define __APP_SPEED_PID_H__

#include "mid_pid.h"

#define ADD                0    // 增加
#define SUBTRACT           1    // 减少
#define MODIFY_PID_STEP    0.1f // 调节Kp等参数的步长
#define MODIFY_TARGET_STEP 1    // 电机PWM最大值
#define TARGET_MAX         100  // 目标值最大限制

#define MODIFY_P           0 // 调节P参数
#define MODIFY_I           1 // 调节I参数
#define MODIFY_D           2 // 调节D参数
#define MODIFY_TARGET      3 // 调节目标值

void speed_pid_init(void);
PID_Struct *get_speed_pid(void);
int get_speed_pid_target(void);
PID_Struct motor_speed_control(int target);
void set_speed_pid_parameter(PID_Struct *pid_value, int select, int add_or_subtract_flag);

#endif
