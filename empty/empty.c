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

// void ui_home_page(void);            // ��ҳҳ���ʼ��
// void ui_encoder_value_update(void); // ��������������ʾ
void display_ui_home();

int main(void)
{
    int sys_time = 0;

    SYSCFG_DL_init();
    // DEBUG���ڳ�ʼ��
    debug_uart_init();
    // ���������ʼ��
    user_button_init();
    // ��������ʼ��
    encoder_init();
    // ��ʱ����ʼ��
    timer_init();
    // LCD��ʼ��
    lcd_init();
    // LCD��ʾUI
    display_ui_home();
    while (1) {
        delay_cycles(CPUCLK_FREQ / 1000 * 2); // ʱ���׼2ms
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
