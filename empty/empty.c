#include "app_distance_pid.h"
#include "app_key_task.h"
#include "app_sys_mode.h"
#include "app_speed_pid.h"
#include "app_ui.h"
#include "app_uart_task.h"
#include "hw_adc.h"
#include "hw_encoder.h"
#include "hw_lcd.h"
#include "hw_motor.h"
#include "mid_button.h"
#include "mid_debug_led.h"
#include "mid_uart.h"
#include "mid_timer.h"
#include "string.h"
#include "stdio.h"
#include "ti_msp_dl_config.h"

void ui_speed_or_distance_page_value_set_quick(LongPressStatus update_status)
{
    if (update_status == LONG_PRESS_END)
        return;

    if (update_status == LONG_PRESS_ADD_START) {
        if (get_functional_mode() == SPEED_FUNCTION)
            set_speed_pid_parameter(get_speed_pid(), system_status.set_page_flag, 0);
        else
            set_distance_pid_parameter(get_distance_pid(), system_status.set_page_flag, 0);
    } else if (update_status == LONG_PRESS_SUBTRACT_START) {
        if (get_functional_mode() == SPEED_FUNCTION)
            set_speed_pid_parameter(get_speed_pid(), system_status.set_page_flag, 1);
        else
            set_distance_pid_parameter(get_distance_pid(), system_status.set_page_flag, 1);
    }
}

void uart_debugger()
{
    debug_uart_init();
    char sbuff[100];
    while (1) {
        sprintf(sbuff, "adc value: %.2f\n", (adc_getValue() / 40.95));
        debug_uart_send_string(sbuff);
        delay_cycles(1000000);
    }
}

int main(void)
{
    int curve_x = 0;

    SYSCFG_DL_init();
    // 串口初始化
    uart_interface_init();
    // 按键任务初始化
    user_button_init();
    // adc初始化
    adc_init();
    // 编码器初始化
    encoder_init();
    // 定时器初始化
    timer_init();
    // 系统参数初始化
    sys_event_init();
    // 定速PID初始化
    speed_pid_init();
    // 定距PID初始化
    distance_pid_init();
    // LCD初始化
    LCD_Init();
    // LCD显示UI
    ui_home_page();

    while (1) {
        if (get_motor_status_flag() == MOTOR_STATUS_ON) {
            switch (get_functional_mode()) {
                case SPEED_FUNCTION:
                    // 计算定速PID
                    motor_speed_control(get_speed_pid_target());
                    // 防止任务冲突，再判断一次
                    if (get_motor_status_flag() == MOTOR_STATUS_ON) {
                        // 显示PID波形和参数
                        ui_speed_curve();
                    } else if (get_motor_status_flag() == MOTOR_STATUS_OFF) {
                        // 停止电机
                        stop_motor();
                        pid_change_zero(get_speed_pid());
                    }
                    break;
                case DISTANCE_FUNCTION:
                    // 计算定距PID
                    motor_distance_control(get_distance_pid_target());
                    // 防止任务冲突，再次判断
                    if (get_motor_status_flag() == MOTOR_STATUS_ON) {
                        // 显示PID波形和参数
                        ui_distance_curve();
                    } else if (get_motor_status_flag() == MOTOR_STATUS_OFF) {
                        // 停止电机
                        stop_motor();
                        pid_change_zero(get_distance_pid());
                    }
                    break;
                default:
                    enable_task_interrupt(); // 开启任务调度
                    pid_change_zero(get_speed_pid());
                    set_motor_status_flag(MOTOR_STATUS_OFF);
                    break;
            }
        }
        if (get_show_state() == PARAMETER_PAGE) {
            // 数值的快速加减
            ui_speed_or_distance_page_value_set_quick(get_long_press_state());
            // 屏幕显示参数变化
            PID_Struct *temp_pid = (get_functional_mode() == SPEED_FUNCTION) ? get_speed_pid() : get_distance_pid();
            int encoder_value    = (get_functional_mode() == SPEED_FUNCTION) ? get_encoder_count() : (get_temp_encoder() * DEGREES_PER_PULSE);
            ui_speed_page_value_set(temp_pid->kp, temp_pid->ki, temp_pid->kd, encoder_value, temp_pid->target, 1);
        }
    }
    
    // NOTE: 此部分为天问ASRPro的扩展部分，暂时不用，先把项目完结
    // while (1) {
    //     recv_cmd(); // 接收串口指令
    //     send_recv_data();
    // }
}
