#include "app_distance_pid.h"
#include "app_key_task.h"
#include "app_stick_task.h"
#include "app_sys_mode.h"
#include "app_speed_pid.h"
#include "app_ui.h"
#include "hw_adc.h"
#include "hw_motor.h"
#include "mid_debug_led.h"

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
        // 首页总共就两个选项
        // 这里只可能存在 0 和 1
        // FIXME: 这堆逆天算法全都改掉，要的是方便读！！！
        case DEFAULT_PAGE: // 默认页
            if (stickVoltage == STICK_MAX_OUTPUT || (ABS(stickVoltage - STICK_MIN_OUTPUT) < TOLERANCE)) {
                system_status.default_page_flag = (system_status.default_page_flag + 1) % 2;
                ui_home_page_select(system_status.default_page_flag); // 选择首页选项框
                delay_cycles(10000000);                               // 延时，防止抖动
            }
            break;
        case SET_PAGE:
            // 这骚操作是为了实现选项 P、I、D 和 Target 的循环
            if (stickVoltage > STICK_TO_LEFT_DOWN_CHECKER) {
                system_status.set_page_flag--;
                if (system_status.set_page_flag < 0) {
                    system_status.set_page_flag = 3;
                }
                delay_cycles(10000000); // 延时，防止抖动
            } else if (stickVoltage < STICK_TO_RIGHT_UP_CHECKER) {
                system_status.set_page_flag = (system_status.set_page_flag + 1) % 4;
                ui_page_select_box(system_status.set_page_flag);
                delay_cycles(10000000); // 延时，防止抖动
            }
            break;
        case PARAMETER_PAGE:
            if (get_functional_mode() == SPEED_FUNCTION) { // 如果之前是定速功能
                // 定速参数更新
                set_speed_pid_parameter(get_speed_pid(), system_status.set_page_flag, SUBTRACT);
            } else if (get_functional_mode() == DISTANCE_FUNCTION) { // 如果是定距功能
                // 定距参数更新
                set_distance_pid_parameter(get_distance_pid(), system_status.set_page_flag, SUBTRACT);
            }
            break;
        default:
            break;
    }
}

void btn_stick_z_cb(flex_button_t *btn)
{
    switch (btn->event) {
        case FLEX_BTN_PRESS_CLICK: // 单击事件
            // 如果是定速页或者定距页
            if (get_show_state() == PID_PAGE || get_show_state() == DISTANCE_PAGE) {
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
            set_debug_led_on();        // 打开调试灯
            break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP: // 长按保持后抬起事件
            break;
        default:
            break;
    }
}
