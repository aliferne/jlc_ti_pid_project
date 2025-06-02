#include "app_sys_mode.h"

SystemStatus system_status;

static void enter_event_handler(SystemStatus *status);
static void quit_event_handler(SystemStatus *status);
static void motor_state_manager(SystemStatus *status);

void sys_event_init(void)
{
    system_status.show_state        = DEFAULT_PAGE;
    system_status.default_page_flag = 0;
    system_status.set_page_flag     = 0;
}

// 设置事件
// 事件管理器
void event_manager(SystemStatus *status, SystemEvent Event)
{
    switch (Event) {
        case ENTER_EVENT: // 触发进入事件
            enter_event_handler(status);
            break;
        case QUIT_EVENT: // 触发退出事件
            quit_event_handler(status);
            break;

        case MOTOR_EVENT: // 电机动作事件
            motor_state_manager(status);
            break;

        case LONG_PRESS_ADD_START_EVENT: // 触发长按加开始事件
            status->long_press_state = LONG_PRESS_ADD_START;
            break;

        case LONG_PRESS_SUBTRACT_START_EVENT: // 触发长按减开始事件
            status->long_press_state = LONG_PRESS_SUBTRACT_START;
            break;

        case LONG_PRESS_END_EVENT: // 触发长按结束事件
            status->long_press_state = LONG_PRESS_END;
            break;

        default:
            break;
    }
}

// 获取显示界面状态
SystemPageShow get_show_state(void)
{
    return system_status.show_state;
}

// 设置默认界面的选项标志
void set_default_page_flag(int flag)
{
    system_status.default_page_flag = flag;
}
// 获取默认界面的选项标志
int get_default_page_flag(void)
{
    return system_status.default_page_flag;
}
// 获取设置界面的选项标志
int get_set_page_flag(void)
{
    return system_status.set_page_flag;
}
// 设置电机状态
void set_motor_status_flag(MotorStatus flag)
{
    system_status.motor_flag = flag;
}
// 获取电机状态
MotorStatus get_motor_status_flag(void)
{
    return system_status.motor_flag;
}

// 获取按键长按的状态
LongPressStatus get_long_press_state(void)
{
    return system_status.long_press_state;
}

// 获取之前是定速功能还是定距功能
Function get_functional_mode(void)
{
    return system_status.function_state;
}
// 设置是速度PID功能还是距离PID功能
void set_functional_mode(Function mode)
{
    system_status.function_state = mode;
}

static void enter_event_handler(SystemStatus *status)
{
    if (status->show_state == DEFAULT_PAGE) {
        switch (status->default_page_flag) {
            // FIXME: 需要修改这堆魔法数字
            case SPEED_PAGE:
                status->show_state     = SPEED_PAGE;
                status->function_state = SPEED_FUNCTION;
                break;
            case DISTANCE_PAGE:
                status->show_state     = DISTANCE_PAGE;
                status->function_state = DISTANCE_FUNCTION;
                break;
            case MANUAL_PAGE:
                status->show_state     = MANUAL_PAGE;
                status->function_state = NO_FUNCTION;
                break;
            case SET_PAGE:
                status->show_state = SET_PAGE;
                // TODO: 后续会加入设置功能（包括启用手柄/启用PID绘制UI等）
                status->function_state = NO_FUNCTION;
            default:
                break;
        }

    } else if (status->show_state == SPEED_PAGE || status->show_state == DISTANCE_PAGE) {
        // 进入设置模式
        status->show_state = SET_PAGE;
    } else if (status->show_state == SET_PAGE) {
        // 进入调参模式
        status->show_state = PARAMETER_PAGE;
    }
}

static void quit_event_handler(SystemStatus *status)
{
    switch (status->show_state) {
        case PARAMETER_PAGE: // 从调参模式退出
            status->show_state = SET_PAGE;
            break;
        case SET_PAGE: // 从设置模式退出
            if (status->function_state == SPEED_FUNCTION)
                status->show_state = SPEED_PAGE;
            else if (status->function_state == DISTANCE_FUNCTION)
                status->show_state = DISTANCE_PAGE;
            break;
        case DEFAULT_PAGE: // 从默认模式退出
            // Nowhere to go
            break;
        case SPEED_PAGE: // 从PID模式退出
            status->show_state     = DEFAULT_PAGE;
            status->motor_flag     = MOTOR_STATUS_OFF;
            status->function_state = NO_FUNCTION;
            break;
        case DISTANCE_PAGE: // 从距离模式退出
            status->show_state     = DEFAULT_PAGE;
            status->motor_flag     = MOTOR_STATUS_OFF;
            status->function_state = NO_FUNCTION;
            break;
        case MANUAL_PAGE: // 从手动模式退出
            status->show_state = DEFAULT_PAGE;
            break;
        case SETTINGS_PAGE:
            status->show_state = DEFAULT_PAGE;
            break;
        default:
            break;
    }
}

static void motor_state_manager(SystemStatus *status)
{
    if (status->motor_flag == MOTOR_STATUS_OFF) {
        status->motor_flag = MOTOR_STATUS_ON;
    } else {
        status->motor_flag = MOTOR_STATUS_OFF;
    }
}
