#include "ti_msp_dl_config.h"
#include "mid_debug_led.h"
#include "mid_debug_uart.h"
#include "string.h"
#include "stdio.h"
#include "hw_lcd.h"
#include "mid_button.h"
#include "app_key_task.h"
#include "hw_encoder.h"
#include "mid_timer.h"
#include "app_ui.h"
#include "app_sys_mode.h"

#define SPEED_ENCODER_VALUE_MAX         (-(SPEED_ENCODER_VALUE_MIN))
#define SPEED_ENCODER_VALUE_MID         0
#define SPEED_ENCODER_VALUE_MIN         (-100)
#define SPEED_WAVEFORM_REDUCTION_FACTOR 10

#define DISTANCE_ENCODER_VALUE_MAX      (-(DISTANCE_ENCODER_VALUE_MIN))
#define DISTANCE_ENCODER_VALUE_MID      0
#define DISTANCE_ENCODER_VALUE_MIN      (-360)

int main(void)
{
    int curve_x = 0;

    SYSCFG_DL_init();

    // DEBUG串口初始化
    debug_uart_init();

    // 按键任务初始化
    user_button_init();

    // 编码器初始化
    encoder_init();

    // 定时器初始化
    timer_init();

    // 系统参数初始化
    sys_event_init();

    // LCD初始化
    LCD_Init();

    // LCD显示UI
    ui_home_page();

    while (1) {

        if (get_motor_status_flag() == MOTOR_STATUS_ON) {
            switch (get_functional_mode()) {
                case SPEED_FUNCTION:

                    // 防止任务冲突，再判断一次
                    if (get_motor_status_flag() == MOTOR_STATUS_ON) {

                        if (curve_x < 319) curve_x++;

                        // 绘制目标速度的波形点
                        LCD_Draw_Point(curve_x, 88 - ((50 + SPEED_ENCODER_VALUE_MAX) / SPEED_WAVEFORM_REDUCTION_FACTOR), RED);
                    }
                    break;

                case DISTANCE_FUNCTION:

                    // 防止任务冲突，再判断一次
                    if (get_motor_status_flag() == MOTOR_STATUS_ON) {
                        if (curve_x > 0) curve_x--;

                        // 绘制目标速度的波形点
                        LCD_Draw_Point(curve_x, 88 - ((50 + SPEED_ENCODER_VALUE_MAX) / SPEED_WAVEFORM_REDUCTION_FACTOR), RED);
                    }
                    break;
            }
        }

        if (get_show_state() == PARAMETER_PAGE) {
            // 长按按键时的屏幕显示参数变化
            // ui_speed_page_value_set(temp_pid->kp, temp_pid->ki, temp_pid->kd, encoder_value, temp_pid->target, 1);
        }
    }
}
