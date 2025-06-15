#include "app_key_task.h"
#include "app_ui.h"
#include "app_sys_mode.h"
#include "app_speed_pid.h"
#include "app_distance_pid.h"
#include "hw_encoder.h"
#include "hw_motor.h"
#include "mid_uart.h"

/**
 * @file app_key_task.c
 * @brief 按键事件处理任务
 *
 * 本文件包含按键事件的处理函数，用于响应用户的按键操作。
 * 根据不同的按键和事件类型，执行相应的逻辑，如页面切换、参数调整、电机控制等。
 */

// TODO: 可以尝试将这里所有过多的if else 使用 switch case 或查表优化

static void enter_event_manager();
static void quit_event_manager();

void btn_up_cb(flex_button_t *btn)
{
    switch (btn->event) {
        case FLEX_BTN_PRESS_CLICK: // 单击事件
            if (get_show_state() == HOME_PAGE) {
                // 目前首页有四个选项，因此对4取模（此外页面选项从1到4）
                system_status.default_page_flag++;
                if (system_status.default_page_flag > SETTINGS_PAGE) {
                    // NOTE: 此处要求SPEED_PAGE是第一个，SETTINGS_PAGE是最后一个
                    system_status.default_page_flag = SPEED_PAGE;
                }
                ui_home_page_select(system_status.default_page_flag); // 选择首页选项框
            }
            if (get_show_state() == SETTINGS_PAGE) {
                // TODO: 设置页面目前只有一个选项，后续功能支持将增加到三个选项
                // system_status.settings_page_flag; // TODO: Operating on this one
                ui_settings_parameter_page_select_box(system_status.settings_page_flag);
            }
            if (get_show_state() == SET_PAGE) {
                system_status.set_page_flag--;
                if (system_status.set_page_flag < P_SELECTED) {
                    // NOTE: 需要确保P_SELECTED是第一个，TARGET_SELECTED是最后一个会被选中的
                    system_status.set_page_flag = TARGET_SELECTED;
                }
                ui_pid_page_select_box(system_status.set_page_flag);
            }
            if (get_show_state() == PID_PARAMETER_PAGE) {
                if (get_functional_mode() == SPEED_FUNCTION) { // 如果之前是定速功能
                    // 定速参数更新
                    set_speed_pid_parameter(get_speed_pid(), system_status.set_page_flag, ADD);
                } else if (get_functional_mode() == DISTANCE_FUNCTION) { // 如果是定距功能
                    // 定距参数更新
                    set_distance_pid_parameter(get_distance_pid(), system_status.set_page_flag, ADD);
                }
            }
            break;
        case FLEX_BTN_PRESS_LONG_HOLD: // 长按保持事件
            // 触发长按加事件
            event_manager(&system_status, LONG_PRESS_ADD_START_EVENT);
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
    switch (btn->event) {
        case FLEX_BTN_PRESS_CLICK: // 单击事件
            if (get_show_state() == HOME_PAGE) {
                // 首页目前四个选项
                system_status.default_page_flag--;
                if (system_status.default_page_flag < SPEED_PAGE) {
                    // NOTE: 此处要求SPEED_PAGE是第一个，SETTINGS_PAGE是最后一个
                    system_status.default_page_flag = SETTINGS_PAGE;
                }
                ui_home_page_select(system_status.default_page_flag); // 选择首页选项框
            }
            if (get_show_state() == SETTINGS_PAGE) {
                // TODO: 设置页面目前只有一个选项，后续功能支持将增加到三个选项
                // system_status.settings_page_flag; // TODO: Operating on this one
                ui_settings_parameter_page_select_box(system_status.settings_page_flag);
            }
            if (get_show_state() == SETTINGS_PARAMETER_PAGE) {
                // 
                ui_settings_page_change_pid_on_ui();
            }
            if (get_show_state() == SET_PAGE) {
                system_status.set_page_flag++;
                // NOTE: 需要确保P_SELECTED是第一个，TARGET_SELECTED是最后一个会被选中的
                if (system_status.set_page_flag > TARGET_SELECTED) {
                    system_status.set_page_flag = P_SELECTED;
                }
                ui_pid_page_select_box(system_status.set_page_flag);
            }
            if (get_show_state() == PID_PARAMETER_PAGE) {
                if (get_functional_mode() == SPEED_FUNCTION) { // 如果之前是定速功能
                    // 定速参数更新
                    set_speed_pid_parameter(get_speed_pid(), system_status.set_page_flag, SUBTRACT);
                } else if (get_functional_mode() == DISTANCE_FUNCTION) { // 如果是定距功能
                    // 定距参数更新
                    set_distance_pid_parameter(get_distance_pid(), system_status.set_page_flag, SUBTRACT);
                }
            }
            break;
        case FLEX_BTN_PRESS_LONG_HOLD: // 长按保持事件
            // 触发长按减事件
            event_manager(&system_status, LONG_PRESS_SUBTRACT_START_EVENT);
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
    switch (btn->event) {
        case FLEX_BTN_PRESS_CLICK: // 单击事件
            quit_event_manager();
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
    switch (btn->event) {
        case FLEX_BTN_PRESS_CLICK: // 单击事件
            enter_event_manager();
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
    switch (btn->event) {
        case FLEX_BTN_PRESS_CLICK: // 单击事件
            // 如果是定速页或者定距页
            if (get_show_state() == SPEED_PAGE || get_show_state() == DISTANCE_PAGE) {
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

// FIXME: 摇杆Z轴无法正常工作
void btn_stick_z_cb(flex_button_t *btn)
{
    switch (btn->event) {
        case FLEX_BTN_PRESS_CLICK: // 单击事件
            enter_event_manager();
            break;
        case FLEX_BTN_PRESS_LONG_HOLD: // 长按保持事件
            quit_event_manager();
            break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP: // 长按保持后抬起事件
            break;
        default:
            break;
    }
}

static void enter_event_manager()
{
    // 如果是首页
    if (get_show_state() == HOME_PAGE) {
        // 触发进入事件
        event_manager(&system_status, ENTER_EVENT);
        // 根据首页选项显示对应功能页
        ui_home_select_page_show(get_default_page_flag());
    }
    // 如果是首页的设置页面
    else if (get_show_state() == SETTINGS_PAGE) {
        // 触发进入事件
        event_manager(&system_status, ENTER_EVENT);
        // 显示框
        ui_settings_parameter_page_select_box(system_status.settings_page_flag);
    }
    // 如果是首页设置界面内部
    else if (get_show_state() == SETTINGS_PARAMETER_PAGE) {
        // 触发进入事件
        event_manager(&system_status, ENTER_EVENT);
        // 显示选中框
        ui_settings_parameter_page_select_box_bold(system_status.settings_page_flag);
    }
    // 如果是定速页或者定距页
    else if (get_show_state() == SPEED_PAGE || get_show_state() == DISTANCE_PAGE) {
        // 触发进入事件
        event_manager(&system_status, ENTER_EVENT);
        // 显示选择框
        ui_pid_page_select_box(system_status.set_page_flag);
    }
    // 如果是设置页
    else if (get_show_state() == SET_PAGE) {
        // 触发进入事件
        event_manager(&system_status, ENTER_EVENT);
        // 显示选中框
        ui_pid_parameter_select_box_bold(system_status.set_page_flag);
    }
}

static void quit_event_manager()
{
    if (get_show_state() == SPEED_PAGE || get_show_state() == DISTANCE_PAGE) {
        ui_home_select_page_show(HOME_PAGE);     // 显示主页面
        set_motor_status_flag(MOTOR_STATUS_OFF); // 设置电机状态为关闭
        stop_motor();                            // 停掉电机
        event_manager(&system_status, QUIT_EVENT);

    }
    // 如果是手册页
    else if (get_show_state() == MANUAL_PAGE) {
        ui_home_select_page_show(HOME_PAGE); // 显示主页面
        event_manager(&system_status, QUIT_EVENT);
    }
    // 如果是首页的设置页面内部
    else if (get_show_state() == SETTINGS_PAGE) {
        ui_home_select_page_show(HOME_PAGE); // 显示主页面
        event_manager(&system_status, QUIT_EVENT);
    }
    // 如果是首页的设置页面内部，调参页面
    else if (get_show_state() == SETTINGS_PARAMETER_PAGE) {
        // TODO:
        event_manager(&system_status, QUIT_EVENT);
        ui_settings_parameter_page_select_box_bold(SETTINGS_PARAMETER_ALL_CLEAN);
    }
    // 如果是PID设置页面
    else if (get_show_state() == SET_PAGE) {
        // 触发退出事件
        event_manager(&system_status, QUIT_EVENT);
        // 擦除全部选择框
        ui_pid_page_select_box(PID_PARAMETER_ALL_CLEAN);
    }
    // 如果当前是调参页
    else if (get_show_state() == PID_PARAMETER_PAGE) {
        // 触发退出事件
        event_manager(&system_status, QUIT_EVENT);
        // 显示设置页的选择框，退出界面之后擦除所有粗框框
        ui_pid_parameter_select_box_bold(PID_PARAMETER_ALL_CLEAN);
    }
}
