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

void display_string_rect(int x, int w, int y, int h, int str_len,
                         int sizey, unsigned char *str, int color)
{
    int str_center_x  = ((sizey / 2) * str_len) / 2; // 字符串x = 字体大小*字符串长度/2
    int rect_center_x = x + (w / 2);                 // 矩形中心x
    int str_center_y  = sizey / 2;                   // 字符串y=字体大小/2
    int rect_center_y = y + (h / 2);                 // 矩形中心y

    // 显示背景矩形
    LCD_ArcRect(x, y, x + w, y + h, color);
    // 显示字符串
    LCD_ShowChinese(rect_center_x - str_center_x,
                    rect_center_y - str_center_y,
                    str, WHITE, color, sizey, 1);
}

void display_ui_home()
{
    LCD_BLK_Clr(); // 清除背光

    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
    display_string_rect(10, 20, 10, 20, 1, 16, "", GRAYBLUE);

    LCD_BLK_Set(); // 打开背光
}
