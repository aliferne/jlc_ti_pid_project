#include "app_ui.h"
#include "string.h"
#include "stdio.h"
#include "hw_lcd.h"
#include "app_sys_mode.h"
#include "app_speed_pid.h"
#include "app_distance_pid.h"
#include "hw_encoder.h"
#include "mid_timer.h"

/// @brief 这个变量由 `show_select_box` 和 `ui_settings_page` 共享
/// @brief 表示是否需要给显示框加上PID算法
static bool mode = false;
// static bool mode = true;

/**
 * @file app_ui.c
 * @brief 用户界面绘制与更新
 *
 * 本文件包含用户界面的绘制和更新函数，用于在LCD屏幕上显示和更新各种UI元素。
 * 包括静态UI的绘制、参数值的显示、选择框的绘制以及波形曲线的绘制等。
 */

/**
 * @brief 在屏幕中心绘制带圆角背景的字符串
 *
 * 该函数用于在屏幕中心绘制一个带圆角背景的字符串。字符串的位置会根据其长度和字体大小自动调整，以确保它在屏幕中心。
 *
 * @param y 字符串起始Y轴坐标
 * @param str_len 字符串长度
 * @param bc 圆角矩形背景色
 * @param fontSize 字符像素大小
 * @param str 要显示的字符串
 *
 * @note 该函数假设屏幕中心坐标 `screen_center_x` 已经被正确定义。
 * @note 字符串的长度不应超过屏幕的宽度，以避免绘制超出屏幕边界。
 * @note 字符串的字体大小应与屏幕的尺寸相匹配，以确保字符串能够完全显示在屏幕内。
 * @note 背景颜色可以是预定义的颜色常量，如 GRAYBLUE 或 DARKBLUE。
 */
void show_x_center(int y, int str_len, uint16_t bc, unsigned char fontSize, unsigned char *str)
{
    // 计算文字的中心
    int str_center_x = (fontSize * str_len) / 2;
    int str_center_y = fontSize / 2;

    // 绘制圆角矩形
    LCD_Draw_ArcRect(
        screen_center_x - str_center_x - 10,
        y,
        screen_center_x + str_center_x + 10,
        fontSize + y,
        bc);
    LCD_Show_Chinese(
        screen_center_x - str_center_x,
        y, str, WHITE, bc, fontSize, 1);
}

/**
 * @brief 绘制彩色填充矩形，带居中的字符串
 *
 * 该函数用于在指定的位置绘制一个彩色填充的矩形，并在矩形内部居中显示一个字符串。
 *
 * @param x 矩形起始X轴坐标
 * @param w 矩形宽度
 * @param y 矩形起始Y轴坐标
 * @param h 矩形高度
 * @param str_len 字符串长度
 * @param fontSize 字符像素大小
 * @param str 要显示的字符串
 * @param color 矩形背景色
 *
 * @note 该函数假设输入的坐标、宽度和高度都是有效的，且不会导致绘制超出屏幕边界。
 * @note 字符串的长度不应超过矩形的宽度。
 * @note 字符串的字体大小应与矩形的尺寸相匹配，以确保字符串能够完全显示在矩形内。
 * @note 背景颜色可以是预定义的颜色常量，如 GRAYBLUE 或 DARKBLUE。
 */
void show_string_rect(int x, int w, int y, int h, int str_len, int fontSize, unsigned char *str, int color)
{
    int str_center_x  = (fontSize * str_len) / 2;
    int rect_center_x = x + (w / 2);
    int str_center_y  = fontSize / 2;
    int rect_center_y = y + (h / 2);

    LCD_Draw_ArcRect(x, y, x + w, y + h, color);
    LCD_Show_Chinese(
        rect_center_x - str_center_x,
        rect_center_y - str_center_y,
        str, WHITE,
        color, fontSize, 1);
}

// FIXME: 使用PID绘制UI界面相对较卡，需要使用DMA之类的技术作为辅助
// 绘制线框的函数（水平方向）
void ui_draw_lines_horizonal(
    int x, int w, int y, int h,
    int line_length, int interval, int color)
{
    // 左上
    LCD_Draw_Line(x, y, x + line_length, y, color);
    // 右上
    LCD_Draw_Line(x + w, y, x + w - line_length, y, color);
    // 左下
    LCD_Draw_Line(x, y + h, x + line_length, y + h, color);
    // 右下
    LCD_Draw_Line(x + w, y + h, x + w - line_length, y + h, color);
}

// 绘制线框的函数（竖直方向）
void ui_draw_lines_vertical(
    int x, int w, int y, int h,
    int line_length, int interval, int color)
{
    // 左上
    LCD_Draw_Line(x, y, x, y + line_length, color);
    // 右上
    LCD_Draw_Line(x + w, y, x + w, y + line_length, color);
    // 左下
    LCD_Draw_Line(x, y + h, x, y + h - line_length, color);
    // 右下
    LCD_Draw_Line(x + w, y + h, x + w, y + h - line_length, color);
}

/**
 * @brief 显示选择框
 *
 * 该函数用于在指定位置绘制一个选择框，支持静态显示和PID动画显示两种模式。
 * 选择框由水平线和垂直线组成，可以设置线条长度、间距和颜色。
 *
 * @param target_x 目标X坐标位置
 * @param w 选择框的宽度
 * @param target_y 目标Y坐标位置
 * @param h 选择框的高度
 * @param line_length 线条的长度
 * @param interval 线条间距，同时也作为选择框向外扩展的距离
 * @param color 线条颜色
 *
 * @details
 * 函数内部有两种显示模式：
 * - mode = 0：静态显示模式，直接在目标位置绘制选择框
 * - mode ≠ 0：PID动画模式，使用PID控制算法实现选择框从当前位置平滑移动到目标位置
 *
 * 在PID模式下：
 * - 使用PID参数：kp=0.4, ki=0.2, kd=0.2
 * - 动画过程中会先清除上一帧的线条，再绘制新位置的线条
 * - 当X轴和Y轴误差都小于0.5时停止动画
 * - 动画结束后会重置PID参数
 *
 * @note
 * - 选择框的实际绘制区域会比输入的w和h各边扩大interval个像素
 * - 需要依赖ui_draw_lines_horizonal()和ui_draw_lines_vertical()函数
 * - PID模式需要pid_init(), pid_calc(), pid_change_zero()等PID相关函数
 * - 需要定义LCD_W, LCD_H, BLACK等常量
 *
 * @see ui_draw_lines_horizonal()
 * @see ui_draw_lines_vertical()
 * @see pid_init()
 * @see pid_calc()
 * @see pid_change_zero()
 */
void show_select_box(
    int target_x, int w, int target_y, int h,
    int line_length, int interval, int color)
{
    // interval: 间距
    target_x -= interval; // 向左偏移interval的距离
    w += interval * 2;    // 宽度增加interval * 2的距离（矩形框两边对称）

    target_y -= interval; // 向上偏移interval的距离
    h += interval * 2;    // 高度增加interval * 2的距离

    // int mode = 0; // TODO: 做到设置页面去调整

    if (mode == false) {
        ui_draw_lines_horizonal(target_x, w, target_y, h, line_length, interval, color);
        ui_draw_lines_vertical(target_x, w, target_y, h, line_length, interval, color);
    } else {
        // p、i、d的参数
        float kp = 0.4, ki = 0.2, kd = 0.2;
        int current_x = 0; // 当前x轴坐标（从0开始）
        int current_y = 0; // 当前y轴坐标（从0开始）

        int value_x, value_y;          // x轴和y轴方向上与目标的差值
        PID_Struct screen_x, screen_y; // x轴和y轴方向上的pid结构体

        // 初始化PID
        pid_init(&screen_x, kp, ki, kd, LCD_W, LCD_W, target_x);
        pid_init(&screen_y, kp, ki, kd, LCD_H, LCD_H, target_y);

        do {
            // 分别计算x轴和y轴方向上的差值
            value_x = pid_calc(&screen_x, target_x, current_x);
            value_y = pid_calc(&screen_y, target_y, current_y);
            // 清除上次线条
            ui_draw_lines_horizonal(current_x, w, target_y, h, line_length, interval, BLACK);
            ui_draw_lines_vertical(target_x, w, current_y, h, line_length, interval, BLACK);
            // 平移到本次坐标的位置
            current_x += value_x;
            current_y += value_y;
            // 绘制本次线条
            ui_draw_lines_horizonal(current_x, w, target_y, h, line_length, interval, color);
            ui_draw_lines_vertical(target_x, w, current_y, h, line_length, interval, color);
            // 延迟一段时间
            delay_cycles(100000 * 8);
        } while (
            // 限制误差在0.5以内
            (ABS(current_x - target_x) > 0.5) &&
            (ABS(current_y - target_y) > 0.5));
        // 将参数置零
        pid_change_zero(&screen_x);
        pid_change_zero(&screen_y);
    }
}

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
        (uint8_t *)"donkey: Move", // down key => donkey
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

void ui_settings_page()
{
    // 关闭背光
    LCD_BLK_Clear();
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);

    show_string_rect(40, 60, 40, 40, 3, FONTSIZE, (uint8_t *)"pid-on", BLUE);

    // 开启背光
    LCD_BLK_Set();
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

/**
 * @brief 绘制PID参数页面的背景和布局框架
 *
 * 该函数负责绘制PID参数调节页面的整体背景，包括清屏、显示PID标题字符
 * 以及为P、I、D三个参数绘制圆角矩形背景框。函数会自动计算各元素的居中位置。
 *
 * @param[in,out] p P参数的文本对象，函数会设置其显示区域坐标
 * @param[in,out] i I参数的文本对象，函数会设置其显示区域坐标
 * @param[in,out] d D参数的文本对象，函数会设置其显示区域坐标
 *
 * @details 函数执行的具体操作：
 *          - 使用黑色填充整个LCD屏幕作为背景
 *          - 在屏幕上方居中显示"P"、"I"、"D"三个标题字符（白字蓝底）
 *          - 为每个PID参数绘制蓝色圆角矩形背景框
 *          - 自动计算并设置传入的文本对象的坐标范围
 *
 * @note 该函数依赖以下全局变量和常量：
 *       - screen_center_x: 屏幕中心X坐标
 *       - DEFAULT_CENTER_X_OFFSET: 默认中心偏移量
 *       - DEFAULT_PID_TITLE_Y_POSITION: PID标题Y坐标位置
 *       - PID_NEXT_Y_START_POSITION: PID参数框起始Y坐标
 *       - FONTSIZE: 字体大小
 *
 * @warning 调用此函数前需确保LCD已正确初始化
 *
 * @see LCD_Fill(), LCD_Show_Char(), LCD_Draw_ArcRect()
 * @see TXT_OBJECT
 */
void ui_pid_page_draw_background_box(TXT_OBJECT p, TXT_OBJECT i, TXT_OBJECT d)
{
    // 绘制纯黑色背景
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
    int str_center_x = (24 * 1) / 2;

    // 显示静态的PID标题
    LCD_Show_Char(
        screen_center_x - str_center_x - DEFAULT_CENTER_X_OFFSET,
        DEFAULT_PID_TITLE_Y_POSITION, 'P', WHITE, BLUE, FONTSIZE, 1);
    LCD_Show_Char(
        screen_center_x - str_center_x,
        DEFAULT_PID_TITLE_Y_POSITION, 'I', WHITE, BLUE, FONTSIZE, 1);
    LCD_Show_Char(
        screen_center_x - str_center_x + DEFAULT_CENTER_X_OFFSET,
        DEFAULT_PID_TITLE_Y_POSITION, 'D', WHITE, BLUE, FONTSIZE, 1);

    // 显示P参数的圆角矩形背景
    p.start_x = screen_center_x - str_center_x - DEFAULT_CENTER_X_OFFSET - 30;
    p.start_y = PID_NEXT_Y_START_POSITION;
    p.end_x   = screen_center_x - str_center_x - DEFAULT_CENTER_X_OFFSET + 40;
    p.end_y   = PID_NEXT_Y_START_POSITION + 24;
    LCD_Draw_ArcRect(p.start_x, p.start_y, p.end_x, p.end_y, BLUE);

    // 显示I参数的圆角矩形背景
    i.start_x = screen_center_x - str_center_x - 30;
    i.start_y = PID_NEXT_Y_START_POSITION;
    i.end_x   = screen_center_x - str_center_x + 40;
    i.end_y   = PID_NEXT_Y_START_POSITION + 24;
    LCD_Draw_ArcRect(i.start_x, i.start_y, i.end_x, i.end_y, BLUE);

    // 显示D参数的圆角矩形背景
    d.start_x = screen_center_x - str_center_x + DEFAULT_CENTER_X_OFFSET - 30;
    d.start_y = PID_NEXT_Y_START_POSITION;
    d.end_x   = screen_center_x - str_center_x + DEFAULT_CENTER_X_OFFSET + 40;
    d.end_y   = PID_NEXT_Y_START_POSITION + 24;
    LCD_Draw_ArcRect(d.start_x, d.start_y, d.end_x, d.end_y, BLUE);

    // 显示 Target 参数的圆角矩形背景
    LCD_Show_String(320 - 150, 170 - 34, (uint8_t *)"Target: ", WHITE, BLUE, 24, 1);
}

// 绘制定速页静态UI
void ui_speed_page(void)
{
    TXT_OBJECT p = {0}, i = {0}, d = {0};

    // 关闭背光
    LCD_BLK_Clear();

    ui_pid_page_draw_background_box(p, i, d);
    // 显示静态的Speed Target 标题
    LCD_Show_String(20, 170 - 34, (uint8_t *)"Speed: ", WHITE, BLUE, 24, 1);

    LCD_BLK_Set(); // 打开背光
}

void ui_pid_page_update_value(
    float *last_p, float *last_i, float *last_d, int *last_current, int *last_target,
    float p, float i, float d, int current, int target, int quick_update)
{
    char show_buff[50] = {0};
    TXT_OBJECT txt_p = {0}, txt_i = {0}, txt_d = {0};
    int txt_size         = 0;  // 一整个字符串的像素
    int char_width_pixel = 8;  // 16x16大小的英文字符实际大小是8x16
    int rect_w           = 70; // 矩形的宽度
    int rect_h           = 24; // 矩形的高度
    int rect_start_x     = 34; // 第一个矩形的起始X轴位置
    int rect_apart_x     = 84; // 矩形与矩形间的间隔

    if (quick_update != 1) // 没有开启快速更新
    {
        *last_p       = 65535.0; // 设置一个不可能的值
        *last_i       = 65535.0; // 设置一个不可能的值
        *last_d       = 65535.0; // 设置一个不可能的值
        *last_current = 65535;   // 设置一个不可能的值
        *last_target  = 65535;   // 设置一个不可能的值
    }
    // 更新 P 参数
    if (*last_p != p) {
        *last_p = p;
        sprintf(show_buff, " %.2f ", p);
        txt_size      = strlen(show_buff) * char_width_pixel;
        txt_p.start_x = rect_start_x + ((rect_w - txt_size) / 2);
        txt_p.start_y = 104 + ((rect_h - FONTSIZE) / 2);
        LCD_Show_String(txt_p.start_x, txt_p.start_y, (uint8_t *)show_buff, WHITE, BLUE, FONTSIZE, 0);
    }
    // 更新 I 参数
    if (*last_i != i) {
        *last_i = i;
        sprintf(show_buff, " %.2f ", i);
        txt_size      = strlen(show_buff) * char_width_pixel;
        txt_i.start_x = (rect_start_x + rect_apart_x) + ((rect_w - txt_size) / 2);
        txt_i.start_y = 104 + ((rect_h - FONTSIZE) / 2);
        LCD_Show_String(txt_i.start_x, txt_i.start_y, (uint8_t *)show_buff, WHITE, BLUE, FONTSIZE, 0);
    }
    // 更新 D 参数
    if (*last_d != d) {
        *last_d = d;
        sprintf(show_buff, " %.2f ", d);
        txt_size      = strlen(show_buff) * char_width_pixel;
        txt_d.start_x = (rect_start_x + rect_apart_x + rect_apart_x) + ((rect_w - txt_size) / 2);
        txt_d.start_y = 104 + ((rect_h - FONTSIZE) / 2);
        LCD_Show_String(txt_d.start_x, txt_d.start_y, (uint8_t *)show_buff, WHITE, BLUE, FONTSIZE, 0);
    }
    // 更新当前 （速度/距离） 参数
    if (*last_current != current) {
        *last_current = current;
        sprintf(show_buff, "%d ", current);
        LCD_Show_String(12 * 6 + 20, 170 - 34, (uint8_t *)show_buff, WHITE, BLACK, 24, 0);
    }
    // 更新目标参数
    if (*last_target != target) {
        *last_target = target;
        sprintf(show_buff, "%d ", target);
        LCD_Show_String(320 - 150 + 12 * 7, 170 - 34, (uint8_t *)show_buff,
                        WHITE, BLACK, 24, 0);
    }
}

void ui_speed_page_value_set(
    float p, float i, float d, int speed,
    int target, int quick_update) // 绘制定速页参数值的变化
{
    static float last_p, last_i, last_d;
    static int last_speed, last_target;

    ui_pid_page_update_value(
        &last_p, &last_i, &last_d, &last_speed, &last_target,
        p, i, d, speed, target, quick_update);
}

void ui_page_draw_box(int p_color, int i_color, int d_color, int target_color)
{
    char select_box_interval = 3;
    // P
    show_select_box(34, 104 - 34, 104, 128 - 104, 10, select_box_interval, p_color);
    // I
    show_select_box(118, 188 - 118, 104, 128 - 104, 10, select_box_interval, i_color);
    // D
    show_select_box(202, 272 - 202, 104, 128 - 104, 10, select_box_interval, d_color);
    // Target
    show_select_box(320 - 150, 316 - (320 - 150), 170 - 34, 166 - (170 - 34), 10, select_box_interval, target_color);
}

// 绘制定速页选择框
// 参数选择框
void ui_page_select_box(int mode)
{
    switch (mode) {
        case P_SELECTED: // P
            ui_page_draw_box(WHITE, BLACK, BLACK, BLACK);
            break;
        case I_SELECTED: // I
            ui_page_draw_box(BLACK, WHITE, BLACK, BLACK);
            break;
        case D_SELECTED: // D
            ui_page_draw_box(BLACK, BLACK, WHITE, BLACK);
            break;
        case TARGET_SELECTED: // target
            ui_page_draw_box(BLACK, BLACK, BLACK, WHITE);
            break;
        case ALL_CLEAN: // all clean
            ui_page_draw_box(BLACK, BLACK, BLACK, BLACK);
            break;
    }
}

void ui_distance_page(void) // 绘制定距页静态UI
{
    TXT_OBJECT p = {0}, i = {0}, d = {0};

    // 关闭背光
    LCD_BLK_Clear();

    ui_pid_page_draw_background_box(p, i, d);
    // 显示静态的Angle Target 标题
    LCD_Show_String(20, 170 - 34, (uint8_t *)"Angle: ", WHITE, BLUE, 24, 1);

    // 开启背光
    LCD_BLK_Set();
}

void ui_distance_page_value_set(
    float p, float i, float d, int distance,
    int target, int quick_update) // 绘制定距页参数值的变化
{
    static float last_p, last_i, last_d;
    static int last_distance, last_target;

    ui_pid_page_update_value(
        &last_p, &last_i, &last_d, &last_distance, &last_target,
        p, i, d, distance, target, quick_update);
}

void ui_select_page_show(int page) // 根据选择确定显示哪一个页面
{
    switch (page) {
        case HOME_PAGE:
            ui_home_page();
            break;
        case SPEED_PAGE:
            ui_speed_page();
            // 绘制定速页面的各项参数
            ui_speed_page_value_set(
                get_speed_pid()->kp, get_speed_pid()->ki, get_speed_pid()->kd,
                get_encoder_count(), get_speed_pid()->target, 0);
            break;
        case DISTANCE_PAGE:
            ui_distance_page();
            // 绘制定距页面的各项参数
            int current_angle = get_temp_encoder() * DEGREES_PER_PULSE;
            ui_distance_page_value_set(
                get_distance_pid()->kp, get_distance_pid()->ki, get_distance_pid()->kd,
                current_angle, get_distance_pid()->target, 0);
            break;
        case MANUAL_PAGE:
            ui_manual_page();
            break;
        // TODO: 还有SETTINGS_PAGE需要完成
        case SETTINGS_PAGE:
            ui_settings_page();
            break;
        default:
            break;
    }
}

void ui_parameter_select_draw_bold_box(int p_color, int i_color, int d_color, int target_color)
{
    char select_box_size = 3;

    LCD_Draw_Rectangle(
        34 - select_box_size + 1,
        104 - select_box_size + 1,
        104 + select_box_size - 1, 128 + select_box_size - 1,
        p_color);
    LCD_Draw_Rectangle(
        118 - select_box_size + 1,
        104 - select_box_size + 1,
        188 + select_box_size - 1, 128 + select_box_size - 1,
        i_color);
    LCD_Draw_Rectangle(
        202 - select_box_size + 1,
        104 - select_box_size + 1, 272 + select_box_size - 1,
        128 + select_box_size - 1, d_color);
    LCD_Draw_Rectangle(
        320 - 150 - select_box_size + 1,
        170 - 34 - select_box_size + 1,
        316 + select_box_size - 1, 166 + select_box_size - 1,
        target_color);
}

void ui_parameter_select_box_bold(int mode) // 参数选择框加粗，即选中框
{
    switch (mode) {
        case P_SELECTED: // P
            ui_parameter_select_draw_bold_box(WHITE, BLACK, BLACK, BLACK);
            break;
        case I_SELECTED: // I
            ui_parameter_select_draw_bold_box(BLACK, WHITE, BLACK, BLACK);
            break;
        case D_SELECTED: // D
            ui_parameter_select_draw_bold_box(BLACK, BLACK, WHITE, BLACK);
            break;
        case TARGET_SELECTED: // target
            ui_parameter_select_draw_bold_box(BLACK, BLACK, BLACK, WHITE);
            break;
        case ALL_CLEAN: // all clean
            ui_parameter_select_draw_bold_box(BLACK, BLACK, BLACK, BLACK);
            break;
    }
}

uint16_t draw_motor_pid_curve(
    int window_start_x, int window_start_y,
    int window_w, int window_h, int curve_color,
    int background_color, short int rawValue)
{
    uint16_t x = 0, y = 0, i = 0;
    static char firstDraw  = 1; // 判断是否第一次画
    static uint16_t last_x = 0, last_y = 0;

    if (rawValue >= window_h) // 限制最大值
        rawValue = window_h;
    if (rawValue <= 0) // 限制最小值
        rawValue = 0;

    // 基于波形框 底部Y轴点 计算显示数据的偏移量
    y = (window_start_y + window_h) - rawValue;

    if (firstDraw) { // 第一次描点，则无需连线，直接描点即可
        LCD_Draw_Point(window_start_x, y, curve_color);
        last_x    = window_start_x;
        last_y    = y;
        firstDraw = 0;
        return 0;
    }

    x = last_x + 1; // 更新x轴时间线

    if (x < window_w) { // 不超过屏幕宽度
        LCD_Draw_VerrticalLine(x, window_start_y, window_start_y + window_h, background_color);
        LCD_Draw_Line(last_x, last_y, x, y, curve_color);
        LCD_Draw_VerrticalLine(x + 1, window_start_y, window_start_y + window_h, background_color);
        last_x = x;
        last_y = y;
    } else { // 超过屏幕宽度，清屏重新画
        // 清除第一列中之前的点
        LCD_Draw_VerrticalLine(window_start_x, window_start_y, window_start_y + window_h, background_color);
        // 显示当前的点
        LCD_Draw_Point(window_start_x, y, curve_color);
        // 更新之前的坐标为当前坐标
        last_x = window_start_x;
        last_y = y;
    }
    return x;
}

uint16_t draw_speed_curve(
    int window_start_x, int window_start_y,
    int window_w, int window_h, int curve_color,
    int background_color, short int rawValue)
{
    return draw_motor_pid_curve(
        window_start_x, window_start_y,
        window_w, window_h,
        curve_color, background_color, rawValue);
}

uint16_t draw_distance_curve(
    int window_start_x, int window_start_y,
    int window_w, int window_h, int curve_color,
    int background_color, short int rawValue)
{
    return draw_motor_pid_curve(
        window_start_x, window_start_y,
        window_w, window_h,
        curve_color, background_color, rawValue);
}

void ui_speed_curve()
{
    disable_task_interrupt(); // 禁止任务调度

    PID_Struct *pid = get_speed_pid();
    ui_speed_page_value_set(
        pid->kp, pid->ki, pid->kd,
        get_encoder_count(), pid->target, 1);

    // “+ SPEED_ENCODER_VALUE_MAX” 将编码器数值放大，去除负数
    // “/ SPEED_WAVEFORM_REDUCTION_FACTOR 因为屏幕小放不下编码器最大值和最小值，因此做除法衰减数值
    // 绘制当前编码器数值曲线
    int curve_x = 0;
    curve_x     = draw_speed_curve(
        0, 0, 319, 88, GREEN, BLACK,
        (get_encoder_count() + SPEED_ENCODER_VALUE_MAX) /
            SPEED_WAVEFORM_REDUCTION_FACTOR);

    // 绘制目标速度的波形点
    LCD_Draw_Point(
        curve_x,
        88 - ((get_speed_pid_target() + SPEED_ENCODER_VALUE_MAX) / SPEED_WAVEFORM_REDUCTION_FACTOR),
        RED);

    enable_task_interrupt(); // 允许任务调度
}

void ui_distance_curve()
{
    int current_angle = 0;

    disable_task_interrupt(); // 禁止任务调度

    current_angle   = get_temp_encoder() * DEGREES_PER_PULSE;
    PID_Struct *pid = get_distance_pid();
    ui_distance_page_value_set(
        pid->kp, pid->ki,
        pid->kd, current_angle, pid->target, 1);

    int curve_x = 0;
    // “+ DISTANCE_ENCODER_VALUE_MAX” 将编码器数值放大，去除负数
    // “/ DISTANCE_WAVEFORM_REDUCTION_FACTOR 因为屏幕小放不下编码器最大值和最小值，因此做除法衰减数值
    // 绘制当前编码器数值曲线
    curve_x = draw_distance_curve(
        0, 0, 319, 88, GREEN, BLACK,
        (current_angle + DISTANCE_ENCODER_VALUE_MAX) /
            DISTANCE_WAVEFORM_REDUCTION_FACTOR);

    // 绘制目标速度的波形点
    LCD_Draw_Point(
        curve_x,
        88 - ((get_distance_pid_target() + DISTANCE_ENCODER_VALUE_MAX) / DISTANCE_WAVEFORM_REDUCTION_FACTOR),
        RED);

    enable_task_interrupt(); // 允许任务调度
}
