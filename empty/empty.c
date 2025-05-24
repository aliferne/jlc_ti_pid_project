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

#include <stdlib.h> //随机数使用

// 生成范围在 0 到 88 之间的随机整数
int generate_random(void)
{
    return (rand() % 89);
}

int main(void)
{

    int sys_time = 0;

    SYSCFG_DL_init();

    // DEBUG串口初始化
    debug_uart_init();

    // 按键任务初始化
    user_button_init();

    // 编码器初始化
    encoder_init();

    // 定时器初始化
    timer_init();

    // LCD初始化
    LCD_Init();

    // // LCD显示UI
    // ui_home_page();

    // // 暂停2秒
    // delay_cycles(80000000 * 2);

    // 显示定速界面
    ui_speed_page();
    // 显示定速界面的PID参数
    ui_speed_page_value_set(12.1, 2.2, 3.12, 50, 30, 1);
    // I值位置显示选择框
    ui_speed_page_select_box(1);
    // D值位置显示选中框
    ui_parameter_select_box_bold(2);

    while (1) {
        sys_time++;

        if (sys_time % 10 == 0) // 2*10=20ms刷新一次屏幕
        {
            // 使用随机数刷新波形
            draw_speed_curve(0, 0, 319, 88, GREEN, BLACK, generate_random());
        }

        delay_cycles(CPUCLK_FREQ / 1000 * 2); // 时间基准2ms
    }
}
