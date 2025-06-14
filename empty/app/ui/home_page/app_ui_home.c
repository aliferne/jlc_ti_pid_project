#include "app_ui_home.h"

void ui_home_page(void)
{
    // 关闭背光
    LCD_BLK_Clear();
    // 绘制纯黑色背景
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
    // 绘制一些其他的文字
    show_x_center(3, 5, BLUE, FONTSIZE, (uint8_t *)"立创开发板");
    show_x_center(23, 8, BLUE, FONTSIZE, (uint8_t *)"简易PID入门套件");
    show_x_center(43, 3, BLUE, FONTSIZE, (uint8_t *)"Ferne");

    // 绘制任务一：PID定速
    show_string_rect(
        PID_SPEED_SELECT_BOX_X_START,
        PID_SPEED_SELECT_BOX_WIDTH,
        PID_SPEED_SELECT_BOX_Y_START,
        PID_SELECT_BOX_HEIGHT,
        3, FONTSIZE, (uint8_t *)"Speed", BLUE);
    // 绘制任务二：PID定距
    show_string_rect(
        PID_DISTANCE_SELECT_BOX_X_START,
        PID_DISTANCE_SELECT_BOX_WIDTH,
        PID_DISTANCE_SELECT_BOX_Y_START,
        PID_SELECT_BOX_HEIGHT,
        4, FONTSIZE, (uint8_t *)"Distance", BLUE);

    show_string_rect(
        PID_MANUAL_SELECT_BOX_X_START,
        PID_MANUAL_SELECT_BOX_WIDTH,
        PID_MANUAL_SELECT_BOX_Y_START,
        PID_SELECT_BOX_HEIGHT,
        3, FONTSIZE, (uint8_t *)"Manual", BLUE);

    show_string_rect(
        PID_SETTINGS_SELECT_BOX_X_START,
        PID_SETTINGS_SELECT_BOX_WIDTH,
        PID_SETTINGS_SELECT_BOX_Y_START,
        PID_SELECT_BOX_HEIGHT,
        4, FONTSIZE, (uint8_t *)"Setttings", BLUE);

    // 根据首页当前选择内容 绘制选择框
    ui_home_page_select(get_default_page_flag());

    LCD_BLK_Set(); // 打开背光
    // NOTE: 先关背光再打开是为了刷新时更好的视觉体验
}

// 给定速选项和定距选项绘制选择框（主要在 `ui_home_page_select` 中调用）
void ui_home_page_draw_select_box(
    int speed_color, int distance_color, int manual_color, int settings_color)
{
    char select_box_size = 5;

    show_select_box( // SPEED PAGE
        PID_SPEED_SELECT_BOX_X_START,
        PID_SPEED_SELECT_BOX_WIDTH,
        PID_SPEED_SELECT_BOX_Y_START,
        PID_SELECT_BOX_HEIGHT,
        PID_SELECT_LINE_LEN,
        select_box_size,
        speed_color);
    show_select_box( // DISTANCE PAGE
        PID_DISTANCE_SELECT_BOX_X_START,
        PID_DISTANCE_SELECT_BOX_WIDTH,
        PID_DISTANCE_SELECT_BOX_Y_START,
        PID_SELECT_BOX_HEIGHT,
        PID_SELECT_LINE_LEN,
        select_box_size,
        distance_color);
    show_select_box( // MANUAL PAGE
        PID_MANUAL_SELECT_BOX_X_START,
        PID_MANUAL_SELECT_BOX_WIDTH,
        PID_MANUAL_SELECT_BOX_Y_START,
        PID_SELECT_BOX_HEIGHT,
        PID_SELECT_LINE_LEN,
        select_box_size,
        manual_color);
    show_select_box( // SETTINGS PAGE
        PID_SETTINGS_SELECT_BOX_X_START,
        PID_SETTINGS_SELECT_BOX_WIDTH,
        PID_SETTINGS_SELECT_BOX_Y_START,
        PID_SELECT_BOX_HEIGHT,
        PID_SELECT_LINE_LEN,
        select_box_size,
        settings_color);
}

// 绘制首页选择框
void ui_home_page_select(int mode)
{
    switch (mode) {
        case SPEED_PAGE: // 选择PID定速模式
            ui_home_page_draw_select_box(WHITE, BLACK, BLACK, BLACK);
            break;
        case DISTANCE_PAGE: // 选择PID定距模式
            ui_home_page_draw_select_box(BLACK, WHITE, BLACK, BLACK);
            break;
        case MANUAL_PAGE: // 选择手册模式
            ui_home_page_draw_select_box(BLACK, BLACK, WHITE, BLACK);
            break;
        case SETTINGS_PAGE:
            ui_home_page_draw_select_box(BLACK, BLACK, BLACK, WHITE);
            break;
    }
}
