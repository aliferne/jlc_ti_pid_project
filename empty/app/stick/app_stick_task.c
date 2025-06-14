#include "app_distance_pid.h"
#include "app_key_task.h"
#include "app_stick_task.h"
#include "app_sys_mode.h"
#include "app_speed_pid.h"
#include "app_ui.h"
#include "hw_adc.h"
#include "hw_encoder.h"
#include "hw_motor.h"

// TODO: 可以配置DMA

static float stickVoltage = 0; // 滑块电压值

void user_stick_init(void)
{
    adc_init(); // 初始化adc
}

void stick_value_update()
{
    // 获取滑块ADC值，输出值在[0, 100]之间
    stickVoltage = adc_getValue() / 40.95;
}

float get_stick_value()
{
    return stickVoltage;
}

void stick_scan()
{
    stick_value_update(); // 更新滑块电压值
    switch (get_show_state()) {
        case HOME_PAGE: // 默认页
            if (stickVoltage == STICK_MAX_OUTPUT) {
                // 目前首页有四个选项，因此对4取模（此外页面选项从1到4）
                system_status.default_page_flag++;
                if (system_status.default_page_flag > SETTINGS_PAGE) {
                    // NOTE: 此处要求SPEED_PAGE是第一个，SETTINGS_PAGE是最后一个
                    system_status.default_page_flag = SPEED_PAGE;
                }
                ui_home_page_select(system_status.default_page_flag); // 选择首页选项框
            } else if (ABS(stickVoltage - STICK_MIN_OUTPUT) < TOLERANCE) {
                // 首页目前四个选项
                system_status.default_page_flag--;
                if (system_status.default_page_flag < SPEED_PAGE) {
                    // NOTE: 此处要求SPEED_PAGE是第一个，SETTINGS_PAGE是最后一个
                    system_status.default_page_flag = SETTINGS_PAGE;
                }
                ui_home_page_select(system_status.default_page_flag); // 选择首页选项框
            }
            break;
        case SET_PAGE:
            // 这骚操作是为了实现选项 P、I、D 和 Target 的循环
            if (stickVoltage == STICK_MAX_OUTPUT) {
                system_status.set_page_flag--;
                if (system_status.set_page_flag < P_SELECTED) {
                    // NOTE: 需要确保P_SELECTED是第一个，TARGET_SELECTED是最后一个会被选中的
                    system_status.set_page_flag = TARGET_SELECTED;
                }
                ui_pid_page_select_box(system_status.set_page_flag);
            } else if (ABS(stickVoltage - STICK_MIN_OUTPUT) < TOLERANCE) {
                system_status.set_page_flag++;
                // NOTE: 需要确保P_SELECTED是第一个，TARGET_SELECTED是最后一个会被选中的
                if (system_status.set_page_flag > TARGET_SELECTED) {
                    system_status.set_page_flag = P_SELECTED;
                }
                ui_pid_page_select_box(system_status.set_page_flag);
            }
            break;
        case PID_PARAMETER_PAGE:
            if (ABS(stickVoltage - STICK_MIN_OUTPUT) < TOLERANCE) {
                if (get_functional_mode() == SPEED_FUNCTION) { // 如果之前是定速功能
                    // 定速参数更新
                    set_speed_pid_parameter(get_speed_pid(), system_status.set_page_flag, SUBTRACT);
                } else if (get_functional_mode() == DISTANCE_FUNCTION) { // 如果是定距功能
                    // 定距参数更新
                    set_distance_pid_parameter(get_distance_pid(), system_status.set_page_flag, SUBTRACT);
                }
            } else if (stickVoltage == STICK_MAX_OUTPUT) {
                if (get_functional_mode() == SPEED_FUNCTION) { // 如果之前是定速功能
                    // 定速参数更新
                    set_speed_pid_parameter(get_speed_pid(), system_status.set_page_flag, ADD);
                } else if (get_functional_mode() == DISTANCE_FUNCTION) { // 如果是定距功能
                    // 定距参数更新
                    set_distance_pid_parameter(get_distance_pid(), system_status.set_page_flag, ADD);
                }
            }
            break;
        default:
            break;
    }
}
