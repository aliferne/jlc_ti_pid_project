#include "app_sys_mode.h"

/**
 * @file app_sys_mode.c
 * @brief 系统模式与事件管理
 * 
 * 本文件包含系统状态和事件管理的实现，包括系统事件的初始化、处理以及状态获取和设置。
 * 通过事件管理函数，系统可以根据不同的事件执行相应的状态转换和逻辑处理。
 */

SystemStatus system_status;

void sys_event_init(void)
{
    system_status.show_state        = DEFAULT_PAGE;
    system_status.default_page_flag = 0;
    system_status.set_page_flag     = 0;
}

void event_manager(SystemStatus *status, SystemEvent Event)
{
    switch (Event) {
        case ENTER_EVENT:
            // 这一段的逻辑感觉可以使用查表法
            // 需要根据页面情况判断

            // 首页
            if (status->show_state == DEFAULT_PAGE) {
                // 此时默认进入 速度 模式
                if (status->default_page_flag == 0) {
                    status->show_state     = PID_PAGE;
                    status->function_state = SPEED_FUNCTION;
                    // 此时进入 距离 模式
                } else if (status->default_page_flag == 1) {
                    status->show_state     = DISTANCE_PAGE;
                    status->function_state = DISTANCE_FUNCTION;
                }
                // 定速页或者定距页
            } else if (status->show_state == PID_PAGE || status->show_state == DISTANCE_PAGE) {
                // 进入设置模式
                status->show_state = SET_PAGE;
                // 设置页
            } else if (status->show_state == SET_PAGE) {
                // 进入调参模式
                status->show_state = PARAMETER_PAGE;
            }
            break;
        case QUIT_EVENT:
            // 触发退出事件
            // 设置页
            if (status->show_state == SET_PAGE) {
                // 根据当前执行的任务确认需要回退到哪个页面
                if (status->function_state == SPEED_FUNCTION)
                    status->show_state = PID_PAGE;
                else if (status->function_state == DISTANCE_FUNCTION)
                    status->show_state = DISTANCE_PAGE;
                // 定速页或者定距页
            } else if (status->show_state == PID_PAGE || status->show_state == DISTANCE_PAGE) {
                status->show_state     = DEFAULT_PAGE;
                status->motor_flag     = MOTOR_STATUS_OFF;
                status->function_state = NO_FUNCTION;
                // 调参页
            } else if (status->show_state == PARAMETER_PAGE) {
                status->show_state = SET_PAGE;
            }
            break;
        case MOTOR_EVENT:   // 设置电机
            if (status->motor_flag == MOTOR_STATUS_OFF) {
                status->motor_flag = MOTOR_STATUS_ON;
            } else {
                status->motor_flag = MOTOR_STATUS_OFF;
            }
        case LONG_PRESS_ADD_START_EVENT:
            status->long_press_state = LONG_PRESS_ADD_START;
            break;
        case LONG_PRESS_SUB_START_EVENT:
            status->long_press_state = LONG_PRESS_SUB_START;
            break;
        case LONG_PRESS_END_EVENT:
            status->long_press_state = LONG_PRESS_END;
            break;
        default:
            break;
    }
}

SystemPageShow get_show_state(void)
{
    return system_status.show_state;
}

void set_default_page_flag(int flag)
{
    system_status.default_page_flag = flag;
}

int get_default_page_flag(void)
{
    return system_status.default_page_flag;
}

int get_set_page_flag(void)
{
    return system_status.set_page_flag;
}

MotorStatus get_motor_status_flag(void)
{
    return system_status.motor_flag;
}

void set_motor_status_flag(MotorStatus flag)
{
    system_status.motor_flag = flag;
}

LongPressStatus get_long_press_state(void)
{
    return system_status.long_press_state;
}

Function get_functional_mode(void)
{
    return system_status.function_state;
}

void set_functional_mode(Function mode)
{
    system_status.function_state = mode;
}