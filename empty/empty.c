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

void display_string_rect(int x, int w, int y, int h, int str_len,
                         int sizey, unsigned char *str, int color)
{
    int str_center_x  = ((sizey / 2) * str_len) / 2; // �ַ���x = �����С*�ַ�������/2
    int rect_center_x = x + (w / 2);                 // ��������x
    int str_center_y  = sizey / 2;                   // �ַ���y=�����С/2
    int rect_center_y = y + (h / 2);                 // ��������y

    // ��ʾ��������
    LCD_ArcRect(x, y, x + w, y + h, color);
    // ��ʾ�ַ���
    LCD_ShowChinese(rect_center_x - str_center_x,
                    rect_center_y - str_center_y,
                    str, WHITE, color, sizey, 1);
}

void display_ui_home()
{
    LCD_BLK_Clr(); // �������

    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
    display_string_rect(10, 20, 10, 20, 1, 16, "", GRAYBLUE);

    LCD_BLK_Set(); // �򿪱���
}
