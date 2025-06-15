#include "app_ui_settings.h"
#include "app_ui_home.h"

/**
 * @brief 判断某项功能是否开启
 *
 * @param status: 一个表示某项功能是否开启的布尔值
 * 比如说PID_ON_UI功能对应的就是 `pid_on_ui_mode`
 *
 * @return `"on"` 或者 `"off"`， 用来显示到TFT屏幕上
 */
static inline char *isOn(bool status)
{
    return status ? "on" : "off";
}

void ui_settings_page_show()
{
    ui_settings_page_show_pid_on_ui(isOn(pid_on_ui_mode));
    // TODO: 加入诸如开启摇杆/开启天问ASRPro模块控制的代码
}

void ui_settings_page_show_pid_on_ui(const char *pid_on_ui_mode_str)
{
    // 关闭背光
    LCD_BLK_Clear();
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);

    LCD_Show_String(
        PID_ON_UI_STR_X_START, PID_ON_UI_STR_Y_START,
        (const uint8_t *)"PID for UI: ",
        WHITE, BLACK, FONTSIZE, 1);

    show_string_rect(
        PID_ON_UI_BOX_X_START,
        PID_ON_UI_BOX_WIDTH,
        PID_ON_UI_BOX_Y_START,
        PID_ON_UI_BOX_HEIGHT, 2,
        FONTSIZE, (uint8_t *)pid_on_ui_mode_str, BLUE);

    // 开启背光
    LCD_BLK_Set();
}

void ui_settings_page_change_pid_on_ui()
{
    pid_on_ui_mode = !pid_on_ui_mode;
    ui_settings_page_show_pid_on_ui(isOn(pid_on_ui_mode));
}

// TODO: 增加更多功能
static void ui_settings_page_draw_box(int pid_on_ui_color)
{
    char select_box_interval = 3;
    // PID-ON-UI
    show_select_box(
        PID_ON_UI_BOX_X_START,
        PID_ON_UI_BOX_WIDTH,
        PID_ON_UI_BOX_Y_START,
        PID_ON_UI_BOX_HEIGHT,
        PID_SETTINGS_SELECT_LINE_LENGTH,
        select_box_interval, pid_on_ui_color);
}

static void ui_settings_page_draw_bold_box(int pid_on_ui_color)
{
    char select_box_size = 3;

    LCD_Draw_Rectangle(
        PID_ON_UI_BOX_X_START - select_box_size,
        PID_ON_UI_BOX_Y_START - select_box_size,
        PID_ON_UI_BOX_X_START + PID_ON_UI_BOX_WIDTH + select_box_size,
        PID_ON_UI_BOX_Y_START + PID_ON_UI_BOX_HEIGHT + select_box_size,
        pid_on_ui_color);
}

void ui_settings_parameter_page_select_box(int mode)
{
    switch (mode) {
        case UI_ON_PID_SELECTED:
            ui_settings_page_draw_box(WHITE);
            break;
            // TODO: 增加更多功能
        case SETTINGS_PARAMETER_ALL_CLEAN:
            ui_settings_page_draw_box(BLACK);
            break;
    }
}

void ui_settings_parameter_page_select_box_bold(int mode) // 参数选择框加粗，即选中框
{
    switch (mode) {
        case UI_ON_PID_SELECTED:
            ui_settings_page_draw_bold_box(WHITE);
            break;
            // TODO: 增加更多功能
        case SETTINGS_PARAMETER_ALL_CLEAN:
            ui_settings_page_draw_bold_box(BLACK);
            break;
    }
}