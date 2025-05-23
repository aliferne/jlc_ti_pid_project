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

// void ui_home_page(void);            // 首页页面初始化
// void ui_encoder_value_update(void); // 编码器的数据显示
void display_ui_home();

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
    lcd_init();
    // LCD显示UI
    display_ui_home();
    while (1) {
        delay_cycles(CPUCLK_FREQ / 1000 * 2); // 时间基准2ms
    }
}

void display_ui_home()
{
    LCD_BLK_Set();
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
    LCD_ShowChinese(0, 0, "Hello World", WHITE, BLACK, 16, 1);
}

void display_rectbox()
{
}
