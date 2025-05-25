#include "app_key_task.h"
#include "hw_motor.h"
#include "app_ui.h"
#include "app_sys_mode.h"

/**
 * @file app_key_task.c
 * @brief 按键事件处理任务
 * 
 * 本文件包含按键事件的处理函数，用于响应用户的按键操作。
 * 根据不同的按键和事件类型，执行相应的逻辑，如页面切换、参数调整、电机控制等。
 */

void btn_up_cb(flex_button_t *btn)
{
    SystemPageShow page_state = get_show_state();
    Function using_function;

    switch (btn->event) {
        case FLEX_BTN_PRESS_CLICK: // 单击事件
            if (page_state == DEFAULT_PAGE) {
                // 首页总共就两个选项
                system_status.default_page_flag = (system_status.default_page_flag + 1) % 2;
                ui_home_page_select(system_status.default_page_flag); // 选择首页选项框
            }
            if (page_state == SET_PAGE) {
                // 这骚操作是为了实现选项 P、I、D 和 Target 的循环
                system_status.set_page_flag--;
                if (system_status.set_page_flag < 0) {
                    system_status.set_page_flag = 3;
                }
                ui_speed_page_select_box(system_status.set_page_flag);
            }
            if (page_state == PARAMETER_PAGE) {
                using_function = get_functional_mode();
                if (using_function == SPEED_FUNCTION) { // 如果之前是定速功能
                    // 定速参数显示
                    ui_speed_page_value_set(12.12, 11.123, 1.223, 50, 50, 0);
                } else if (using_function == DISTANCE_FUNCTION) { // 如果是定距功能
                    // 定距参数显示
                    ui_distance_page_value_set(12.12, 11.123, 1.223, 90, 90, 0);
                }
            }
            break;
        case FLEX_BTN_PRESS_LONG_HOLD: // 长按保持事件

            // 触发长按加事件
            event_manager(&system_status, LONG_PRESS_ADD_START);
            break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP: // 长按保持后抬起事件
            // 触发长按停止事件
            event_manager(&system_status, LONG_PRESS_END_EVENT);
            break;
        default:
            break;
    }
}

void btn_down_cb(flex_button_t *btn)
{
    SystemPageShow page_state = get_show_state();
    Function using_function;

    switch (btn->event) {
        case FLEX_BTN_PRESS_CLICK: // 单击事件
            if (page_state == DEFAULT_PAGE) {
                // 首页总共就两个选项
                system_status.default_page_flag--;
                if (system_status.default_page_flag < 0) {
                    system_status.default_page_flag = 1;
                }
                ui_home_page_select(system_status.default_page_flag); // 选择首页选项框
            }
            if (page_state == SET_PAGE) {
                // 这骚操作是为了实现选项 P、I、D 和 Target 的循环
                system_status.set_page_flag = (system_status.set_page_flag + 1) % 4;
                ui_speed_page_select_box(system_status.set_page_flag);
            }
            if (page_state == PARAMETER_PAGE) {
                using_function = get_functional_mode();
                if (using_function == SPEED_FUNCTION) { // 如果之前是定速功能
                                                        // 定速参数显示
                    ui_speed_page_value_set(12.12, 11.123, 1.223, 90, 90, 0);
                } else if (using_function == DISTANCE_FUNCTION) { // 如果是定距功能
                    // 定距参数显示
                    ui_distance_page_value_set(12.12, 11.123, 1.223, 90, 90, 0);
                }
            }
            break;
        case FLEX_BTN_PRESS_LONG_HOLD: // 长按保持事件

            // 触发长按减事件
            event_manager(&system_status, LONG_PRESS_SUB_START);
            break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP: // 长按保持后抬起事件
            // 触发长按停止事件
            event_manager(&system_status, LONG_PRESS_END_EVENT);
            break;
        default:
            break;
    }
}

void btn_left_cb(flex_button_t *btn)
{
    SystemPageShow page_state = get_show_state();

    switch (btn->event) {
        case FLEX_BTN_PRESS_CLICK: // 单击事件
            if (page_state == PID_PAGE || page_state == DISTANCE_PAGE) {
                ui_select_page_show(HOME_PAGE);          // 显示主页面
                stop_motor();                            // 停掉电机
                set_motor_status_flag(MOTOR_STATUS_OFF); // 设置电机状态为关闭
                event_manager(&system_status, QUIT_EVENT);
            }
            if (page_state == SET_PAGE) {
                // 触发退出事件
                event_manager(&system_status, QUIT_EVENT);
                // 擦除全部选择框
                ui_speed_page_select_box(ALL_CLEAN);
            }
            // 如果当前是调参页
            if (page_state == PARAMETER_PAGE) {
                // 触发退出事件
                event_manager(&system_status, QUIT_EVENT);
                // 显示设置页的选择框
                ui_parameter_select_box_bold(get_set_page_flag() + 4);
            }
            break;
        case FLEX_BTN_PRESS_LONG_HOLD: // 长按保持事件
            break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP: // 长按保持后抬起事件
            break;
        default:
            break;
    }
}

void btn_right_cb(flex_button_t *btn)
{
    SystemPageShow page_state = get_show_state();

    switch (btn->event) {
        case FLEX_BTN_PRESS_CLICK: // 单击事件

            // 如果是首页
            if (page_state == DEFAULT_PAGE) {
                // 触发进入事件
                event_manager(&system_status, ENTER_EVENT);
                // 根据首页选项显示对应功能页
                ui_select_page_show(get_default_page_flag());
                // 如果下一个是定速页
                if (page_state == PID_PAGE) {
                    // 显示定速页的参数
                    ui_speed_page_value_set(12.12, 11.123, 1.223, 90, 90, 0);
                }
                // 如果下一个是定距页
                if (page_state == DISTANCE_PAGE) {
                    // 显示定距页的参数
                    ui_distance_page_value_set(12.12, 11.123, 1.223, 90, 90, 0);
                }
            }
            // 如果是定速页或者定距页
            else if (page_state == PID_PAGE || page_state == DISTANCE_PAGE) {
                // 触发进入事件
                event_manager(&system_status, ENTER_EVENT);
                // 显示选择框
                ui_speed_page_select_box(system_status.set_page_flag);
            }
            // 如果是设置页
            else if (page_state == SET_PAGE) {
                // 触发进入事件
                event_manager(&system_status, ENTER_EVENT);
                // 显示选中框
                ui_parameter_select_box_bold(system_status.set_page_flag);
            }
            break;
        case FLEX_BTN_PRESS_LONG_HOLD: // 长按保持事件
            break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP: // 长按保持后抬起事件
            break;
        default:
            break;
    }
}

void btn_mid_cb(flex_button_t *btn)
{
    SystemPageShow page_state = get_show_state();

    switch (btn->event) {
        case FLEX_BTN_PRESS_CLICK: // 单击事件
            // 如果是定速页或者定距页
            if (page_state == PID_PAGE || page_state == DISTANCE_PAGE) {
                // 触发电机事件
                event_manager(&system_status, MOTOR_EVENT);
                // 如果电机状态是关
                if (get_motor_status_flag() == MOTOR_STATUS_OFF) {
                    // 停止电机
                    stop_motor();
                }
            }
            break;
        case FLEX_BTN_PRESS_LONG_HOLD: // 长按保持事件
            break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP: // 长按保持后抬起事件
            break;
        default:
            break;
    }
}
