#include "app_ui_public.h"


/**
 * @brief 使用PID控制绘制动态选择框
 *
 * 该函数通过PID控制算法实现选择框的动态移动效果，从当前位置平滑移动到目标位置。
 * 使用双轴PID控制器分别控制X轴和Y轴的移动，创造出流畅的动画效果。
 * 
 * @param target_x 目标选择框的X坐标位置
 * @param w 选择框的宽度
 * @param target_y 目标选择框的Y坐标位置  
 * @param h 选择框的高度
 * @param line_length 选择框线条的长度
 * @param interval 选择框线条的间距
 * @param color 选择框的颜色值
 * 
 * @note 该函数为静态函数，仅供内部使用
 * @warning 函数会阻塞执行直到选择框到达目标位置
 */
static void ui_draw_select_box_by_pid(
    int target_x, int w, int target_y, int h,
    int line_length, int interval, int color);

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

void show_select_box(
    int target_x, int w, int target_y, int h,
    int line_length, int interval, int color)
{
    // interval: 间距
    target_x -= interval; // 向左偏移interval的距离
    w += interval * 2;    // 宽度增加interval * 2的距离（矩形框两边对称）

    target_y -= interval; // 向上偏移interval的距离
    h += interval * 2;    // 高度增加interval * 2的距离

    if (pid_on_ui_mode == false) {
        ui_draw_lines_horizonal(target_x, w, target_y, h, line_length, interval, color);
        ui_draw_lines_vertical(target_x, w, target_y, h, line_length, interval, color);
    } else {
        ui_draw_select_box_by_pid(target_x, w, target_y, h, line_length, interval, color);
    }
}


static void ui_draw_select_box_by_pid(
    int target_x, int w, int target_y, int h,
    int line_length, int interval, int color)
{
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
