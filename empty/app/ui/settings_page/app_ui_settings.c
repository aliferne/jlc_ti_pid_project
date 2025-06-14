#include "app_ui_settings.h"
#include "app_ui_home.h"

void ui_settings_page_show()
{
    char *pid_on_ui_mode_str = pid_on_ui_mode ? "true" : "false";
    ui_settings_page_show_pid_on_ui(pid_on_ui_mode_str);
}

void ui_settings_page_show_pid_on_ui(const char *pid_on_ui_mode_str)
{
    // 关闭背光
    LCD_BLK_Clear();
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);

    LCD_Show_String(40, 60, (const uint8_t *)"PID for UI: ", WHITE, BLACK, FONTSIZE, 1);
    show_string_rect(160, 60, 50, 30, 3, FONTSIZE, (uint8_t *)pid_on_ui_mode_str, BLUE);

    // 开启背光
    LCD_BLK_Set();
}

void ui_settings_page_change_pid_on_ui()
{
    pid_on_ui_mode           = !pid_on_ui_mode;
    char *pid_on_ui_mode_str = pid_on_ui_mode ? "true" : "false";
    ui_settings_page_show_pid_on_ui(pid_on_ui_mode_str);
}

// TODO: 增加更多功能
static void ui_settings_page_draw_box(int pid_on_ui_color)
{
    char select_box_interval = 3;
    // PID-ON-UI
    show_select_box(160, 60, 50, 30, 10, select_box_interval, pid_on_ui_color);
}

static void ui_settings_page_draw_bold_box(int pid_on_ui_color)
{
    char select_box_size = 3;

    LCD_Draw_Rectangle(
        160 - select_box_size + 1,
        50 - select_box_size + 1,
        160 + 60 + select_box_size - 1, 50 + 30 + select_box_size - 1,
        pid_on_ui_color);
}

void ui_settings_page_select_box(int mode)
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

void ui_settings_parameter_select_box_bold(int mode) // 参数选择框加粗，即选中框
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