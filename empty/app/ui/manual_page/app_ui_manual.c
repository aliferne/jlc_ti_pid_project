#include "app_ui_manual.h"

// 绘制提示页面的文字（对应按键的功能之类的）
void ui_manual_page()
{
    LCD_BLK_Clear();
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);

    LCD_Show_String(
        PID_MANUAL_STRINGS_X_START,
        PID_MANUAL_STRINGS_Y_START,
        (uint8_t *)"up key: Move",
        WHITE, BLACK, FONTSIZE, 0);

    LCD_Show_String(
        PID_MANUAL_STRINGS_X_START,
        PID_MANUAL_STRINGS_Y_START + PID_MANUAL_STRINGS_Y_INTERVAL * 1,
        (uint8_t *)"down key: Move",
        WHITE, BLACK, FONTSIZE, 0);

    LCD_Show_String(
        PID_MANUAL_STRINGS_X_START,
        PID_MANUAL_STRINGS_Y_START + PID_MANUAL_STRINGS_Y_INTERVAL * 2,
        (uint8_t *)"right key: Select",
        WHITE, BLACK, FONTSIZE, 0);

    LCD_Show_String(
        PID_MANUAL_STRINGS_X_START,
        PID_MANUAL_STRINGS_Y_START + PID_MANUAL_STRINGS_Y_INTERVAL * 3,
        (uint8_t *)"left key: Cancel",
        WHITE, BLACK, FONTSIZE, 0);

    LCD_Show_String(
        PID_MANUAL_STRINGS_X_START,
        PID_MANUAL_STRINGS_Y_START + PID_MANUAL_STRINGS_Y_INTERVAL * 4,
        (uint8_t *)"mid key: Set Motor",
        WHITE, BLACK, FONTSIZE, 0);

    LCD_BLK_Set();
}
