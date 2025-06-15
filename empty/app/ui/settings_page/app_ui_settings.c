#include "app_ui_settings.h"
#include "app_ui_home.h"

static const char *pid_on_ui_block  = "PID for UI: ";
static const char *stick_block      = "Stick for control: ";
static const char *asrpro_tts_block = "TTS for motor: ";

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
    // 关闭背光
    LCD_BLK_Clear();
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
    // PID-ON-UI 部分
    ui_settings_page_show_parameters_block(
        pid_on_ui_block,
        PID_ON_UI_STR_X_START,
        PID_ON_UI_STR_Y_START,
        5,
        PID_ON_UI_BOX_X_START,
        PID_ON_UI_BOX_WIDTH,
        PID_ON_UI_BOX_Y_START,
        PID_ON_UI_BOX_HEIGHT,
        BLUE,
        isOn(pid_on_ui_mode));
    // 摇杆模块部分
    ui_settings_page_show_parameters_block(
        stick_block,
        STICK_STR_X_START,
        STICK_STR_Y_START,
        10,
        STICK_BOX_X_START,
        STICK_BOX_WIDTH,
        STICK_BOX_Y_START,
        STICK_BOX_HEIGHT,
        BLUE,
        "on"
        // TODO:
        // isOn(pid_on_ui_mode)
    );
    // ASRPro 部分
    ui_settings_page_show_parameters_block(
        asrpro_tts_block,
        ASRPRO_TTS_STR_X_START,
        ASRPRO_TTS_STR_Y_START,
        8,
        ASRPRO_TTS_BOX_X_START,
        ASRPRO_TTS_BOX_WIDTH,
        ASRPRO_TTS_BOX_Y_START,
        ASRPRO_TTS_BOX_HEIGHT,
        BLUE,
        "on"
        // TODO:
        // isOn(pid_on_ui_mode)
    );
    // 开启背光
    LCD_BLK_Set();
}

void ui_settings_page_show_parameters_block(
    const char *str,
    int str_x_start,
    int str_y_start,
    int str_len,
    int box_x_start,
    int box_width,
    int box_y_start,
    int box_height,
    int box_color,
    const char *mode_str)
{
    LCD_Show_String(
        str_x_start, str_y_start,
        (const uint8_t *)str,
        WHITE, BLACK, FONTSIZE, str_len);
    show_string_rect(
        box_x_start,
        box_width,
        box_y_start,
        box_width, 2,
        FONTSIZE, (uint8_t *)mode_str, box_color);
}

void ui_settings_page_change_pid_on_ui()
{
    pid_on_ui_mode = !pid_on_ui_mode;
    ui_settings_page_show_parameters_block(
        pid_on_ui_block,
        PID_ON_UI_STR_X_START,
        PID_ON_UI_STR_Y_START,
        5,
        PID_ON_UI_BOX_X_START,
        PID_ON_UI_BOX_WIDTH,
        PID_ON_UI_BOX_Y_START,
        PID_ON_UI_BOX_HEIGHT,
        BLUE,
        isOn(pid_on_ui_mode));
}

// TODO: 增加更多功能
static void ui_settings_page_draw_box(
    int pid_on_ui_color,
    int stick_color,
    int asrpro_tts_color)
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

    // STICK
    show_select_box(
        STICK_BOX_X_START,
        STICK_BOX_WIDTH,
        STICK_BOX_Y_START,
        STICK_BOX_HEIGHT,
        PID_SETTINGS_SELECT_LINE_LENGTH,
        select_box_interval, stick_color);

    // ASRPro-TTS
    show_select_box(
        ASRPRO_TTS_BOX_X_START,
        ASRPRO_TTS_BOX_WIDTH,
        ASRPRO_TTS_BOX_Y_START,
        ASRPRO_TTS_BOX_HEIGHT,
        PID_SETTINGS_SELECT_LINE_LENGTH,
        select_box_interval, asrpro_tts_color);
}

static void ui_settings_page_draw_bold_box(
    int pid_on_ui_color,
    int stick_color,
    int asrpro_tts_color)
{
    char select_box_size = 3;

    // PID-ON-UI
    LCD_Draw_Rectangle(
        PID_ON_UI_BOX_X_START - select_box_size,
        PID_ON_UI_BOX_Y_START - select_box_size,
        PID_ON_UI_BOX_X_START + PID_ON_UI_BOX_WIDTH + select_box_size,
        PID_ON_UI_BOX_Y_START + PID_ON_UI_BOX_HEIGHT + select_box_size,
        pid_on_ui_color);

    // STICK
    LCD_Draw_Rectangle(
        STICK_BOX_X_START - select_box_size,
        STICK_BOX_Y_START - select_box_size,
        STICK_BOX_X_START + STICK_BOX_WIDTH + select_box_size,
        STICK_BOX_Y_START + STICK_BOX_HEIGHT + select_box_size,
        stick_color);

    // ASRPro-TTS
    LCD_Draw_Rectangle(
        ASRPRO_TTS_BOX_X_START - select_box_size,
        ASRPRO_TTS_BOX_Y_START - select_box_size,
        ASRPRO_TTS_BOX_X_START + ASRPRO_TTS_BOX_WIDTH + select_box_size,
        ASRPRO_TTS_BOX_Y_START + ASRPRO_TTS_BOX_HEIGHT + select_box_size,
        asrpro_tts_color);
}

void ui_settings_parameter_page_select_box(int mode)
{
    switch (mode) {
        case PID_ON_UI_SELECTED:
            ui_settings_page_draw_box(WHITE, BLACK, BLACK);
            break;
        case STICK_SELECTED:
            ui_settings_page_draw_box(BLACK, WHITE, BLACK);
            break;
        case ASRPRO_SELECTED:
            ui_settings_page_draw_box(BLACK, BLACK, WHITE);
            break;
        case SETTINGS_PARAMETER_ALL_CLEAN:
            ui_settings_page_draw_box(BLACK, BLACK, BLACK);
            break;
    }
}

void ui_settings_parameter_page_select_box_bold(int mode) // 参数选择框加粗，即选中框
{
    switch (mode) {
        case PID_ON_UI_SELECTED:
            ui_settings_page_draw_bold_box(WHITE, BLACK, BLACK);
            break;
        case STICK_SELECTED:
            ui_settings_page_draw_bold_box(BLACK, WHITE, BLACK);
            break;
        case ASRPRO_SELECTED:
            ui_settings_page_draw_bold_box(BLACK, BLACK, WHITE);
            break;
        case SETTINGS_PARAMETER_ALL_CLEAN:
            ui_settings_page_draw_bold_box(BLACK, BLACK, BLACK);
            break;
    }
}