#ifndef __APP_SYS_MODE_H__
#define __APP_SYS_MODE_H__

/**
 * @file app_sys_mode.h
 * @brief 系统模式与事件管理头文件
 *
 * 本头文件定义了系统模式与事件管理的相关数据结构和函数原型。
 * 包括页面状态、功能模式、事件类型、电机状态和长按状态等枚举类型，
 * 以及系统状态结构体和外部声明的事件管理相关函数。
 */

// 定义页面
typedef enum {
    DEFAULT_PAGE = 0, // 默认首页
    PID_PAGE,         // 定速页
    DISTANCE_PAGE,    // 定距页
    // NOTE: 中间这两个加进来的要是影响了原功能则删除
    MANUAL_PAGE,      // 手册页
    SETTINGS_PAGE,    // 设置页
    // NOTE: 中间这两个加进来的要是影响了原功能则删除
    SET_PAGE,         // 设置页
    PARAMETER_PAGE,   // 参数调整页
} SystemPageShow;

// 定义功能
typedef enum {
    NO_FUNCTION,       // 无功能
    SPEED_FUNCTION,    // 定速功能
    DISTANCE_FUNCTION, // 定距功能
} Function;

// 定义触发事件
typedef enum {
    IDLE_EVENT = 0,
    ENTER_EVENT,                // 进入事件
    QUIT_EVENT,                 // 退出事件
    MOTOR_EVENT,                // 按键事件
    LONG_PRESS_ADD_START_EVENT, // 长按加开始事件
    LONG_PRESS_SUBTRACT_START_EVENT, // 长按减停止事件
    LONG_PRESS_END_EVENT,       // 长按结束事件
} SystemEvent;

// 电机状态
typedef enum {
    MOTOR_STATUS_OFF = 0,
    MOTOR_STATUS_ON
} MotorStatus;

// 定义按键长按状态
typedef enum {
    LONG_PRESS_END = 0,
    LONG_PRESS_ADD_START,
    LONG_PRESS_SUBTRACT_START,
} LongPressStatus;

// 定义系统相关信息
typedef struct {
    SystemPageShow show_state;        // 当前界面显示页
    LongPressStatus long_press_state; // 当前按键长按状态
    int default_page_flag;            // 当前首页（默认页）选择的内容
    int set_page_flag;                // 当前设置页选择的内容
    MotorStatus motor_flag;           // 当前电机状态
    Function function_state;          // 当前功能
} SystemStatus;

/**
 * @brief 系统状态结构体
 */
extern SystemStatus system_status;

/**
 * @brief 初始化系统事件
 *
 * 该函数用于初始化系统事件管理模块。
 */
void sys_event_init(void);

/**
 * @brief 事件管理函数
 *
 * @param status 指向系统状态结构体的指针
 * @param Event  要处理的事件
 *
 * 该函数用于处理系统中的各种事件。
 */
void event_manager(SystemStatus *status, SystemEvent Event);

/**
 * @brief 获取当前显示页面状态
 *
 * @return 当前显示的页面状态
 *
 * 该函数用于获取当前显示的页面状态。
 */
SystemPageShow get_show_state(void);

/**
 * @brief 设置默认页面标志
 *
 * @param flag 默认页面标志
 *
 * 该函数用于设置默认页面标志。
 */
void set_default_page_flag(int flag);

/**
 * @brief 获取默认页面标志
 *
 * @return 默认页面标志
 *
 * 该函数用于获取默认页面标志。
 */
int get_default_page_flag(void);

/**
 * @brief 获取设置页面标志
 *
 * @return 设置页面标志
 *
 * 该函数用于获取设置页面标志。
 */
int get_set_page_flag(void);

/**
 * @brief 获取电机状态标志
 *
 * @return 电机状态标志
 *
 * 该函数用于获取电机的当前状态标志。
 */
MotorStatus get_motor_status_flag(void);

/**
 * @brief 设置电机状态标志
 *
 * @param flag 电机状态标志
 *
 * 该函数用于设置电机的状态标志。
 */
void set_motor_status_flag(MotorStatus flag);

/**
 * @brief 获取长按状态
 *
 * @return 长按状态
 *
 * 该函数用于获取当前的长按状态。
 */
LongPressStatus get_long_press_state(void);

/**
 * @brief 获取功能模式
 *
 * @return 当前功能模式
 *
 * 该函数用于获取当前的功能模式。
 */
Function get_functional_mode(void);

/**
 * @brief 设置功能模式
 *
 * @param mode 要设置的功能模式
 *
 * 该函数用于设置系统的功能模式。
 */
void set_functional_mode(Function mode);

#endif