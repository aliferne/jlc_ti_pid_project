#ifndef __APP_DISTANCE_PID_H__
#define __APP_DISTANCE_PID_H__

#include "mid_pid.h"

#define MOTOR_REDUCTION_RATIO 48       // 电机减速比 1：48
#define ENCODER_RESOLUTION    1248     // 电机一圈的编码器脉冲数 1248脉冲/转
#define DEGREES_PER_PULSE     0.28846f // 每脉冲度数 360 / ENCODER_RESOLUTION

#define DEFAULT_KP            75 // 起始默认Kp
#define DEFAULT_KI            2  // 起始默认Ki
#define DEFAULT_KD            10 // 起始默认Kd
#define DEFAULT_ANGLE         90 // 起始默认旋转角度

#define ADD                   0    // 增加
#define SUBTRACT              1    // 减少
#define MODIFY_PID_STEP       0.1f // 调节Kp等参数的步长
#define MODIFY_TARGET_STEP    1    // 调节电机PWM的步长
#define ANGLE_MAX             360  // 角度最大限制

#define MODIFY_P              0 // 调节P参数
#define MODIFY_I              1 // 调节I参数
#define MODIFY_D              2 // 调节D参数
#define MODIFY_ANGLE          3 // 调节角度

/**
 * @brief 初始化距离PID控制器
 *
 * 初始化距离控制相关的PID参数和结构体，为后续的距离控制做准备。
 *
 * @return void
 */
void distance_pid_init(void);

/**
 * @brief 获取距离PID控制器结构体指针
 *
 * 返回指向距离PID控制器结构体的指针，用于访问PID控制器的参数和状态。
 *
 * @return PID_Struct* 指向距离PID控制器结构体的指针
 */
PID_Struct *get_distance_pid(void);

/**
 * @brief 获取距离PID控制器的目标值
 *
 * 返回当前距离PID控制器设定的目标距离值。
 *
 * @return int 距离PID控制器的目标值
 */
int get_distance_pid_target(void);

/**
 * @brief 电机距离控制函数
 *
 * 根据目标角度进行电机距离控制，通过PID算法计算并返回控制结果。
 *
 * @param target_angle 目标角度值
 * @return PID_Struct 返回PID控制结构体，包含控制计算结果
 */
PID_Struct motor_distance_control(int target_angle);

/**
 * @brief 设置距离PID控制器参数
 *
 * 根据选择的参数类型和操作标志，对距离PID控制器的参数进行设置或调整。
 *
 * @param pid_value 指向PID结构体的指针，包含要设置的PID参数
 * @param select 参数选择标志，指定要修改的参数类型（如P、I、D参数）
 * @param add_or_subtract_flag 操作标志，指定是增加还是减少参数值
 * @return void
 */
void set_distance_pid_parameter(PID_Struct *pid_value, int select, int add_or_subtract_flag);

#endif
