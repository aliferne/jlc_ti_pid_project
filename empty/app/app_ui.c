// #include "app_ui.h"
// #include "string.h"
// #include "stdio.h"
// #include "hw_lcd.h"

// /**
//  * @brief 在屏幕中心绘制带圆角背景的字符串
//  *
//  * 该函数用于在屏幕中心绘制一个带圆角背景的字符串。字符串的位置会根据其长度和字体大小自动调整，以确保它在屏幕中心。
//  *
//  * @param y 字符串起始Y轴坐标
//  * @param str_len 字符串长度
//  * @param bc 圆角矩形背景色
//  * @param fontSize 字符像素大小
//  * @param str 要显示的字符串
//  *
//  * @note 该函数假设屏幕中心坐标 `screen_center_x` 已经被正确定义。
//  * @note 字符串的长度不应超过屏幕的宽度，以避免绘制超出屏幕边界。
//  * @note 字符串的字体大小应与屏幕的尺寸相匹配，以确保字符串能够完全显示在屏幕内。
//  * @note 背景颜色可以是预定义的颜色常量，如 GRAYBLUE 或 DARKBLUE。
//  */
// void disp_x_center(int y, int str_len, uint16_t bc, unsigned char fontSize, unsigned char *str)
// {
//     // 计算文字的中心
//     int str_center_x = (fontSize * str_len) / 2;
//     int str_center_y = fontSize / 2;

//     // 绘制圆角矩形
//     LCD_ArcRect(screen_center_x - str_center_x - 10,
//                 screen_center_x + str_center_x + 10,
//                 fontSize + y,
//                 bc);
//     LCD_ShowChinese(screen_center_x - str_center_x,
//                     y, str, WHITE, bc, fontSize, 1);
// }

// /**
//  * @brief 绘制彩色填充矩形，带居中的字符串
//  *
//  * 该函数用于在指定的位置绘制一个彩色填充的矩形，并在矩形内部居中显示一个字符串。
//  *
//  * @param x 矩形起始X轴坐标
//  * @param w 矩形宽度
//  * @param y 矩形起始Y轴坐标
//  * @param h 矩形高度
//  * @param str_len 字符串长度
//  * @param fontSize 字符像素大小
//  * @param str 要显示的字符串
//  * @param color 矩形背景色
//  *
//  * @note 该函数假设输入的坐标、宽度和高度都是有效的，且不会导致绘制超出屏幕边界。
//  * @note 字符串的长度不应超过矩形的宽度。
//  * @note 字符串的字体大小应与矩形的尺寸相匹配，以确保字符串能够完全显示在矩形内。
//  * @note 背景颜色可以是预定义的颜色常量，如 GRAYBLUE 或 DARKBLUE。
//  */
// void disp_string_rect(int x, int w, int y, int h, int str_len, int fontSize, unsigned char *str, int color)
// {
//     int str_center_x  = (fontSize * str_len) / 2;
//     int rect_center_x = x + (w / 2);
//     int str_center_y  = fontSize / 2;
//     int rect_center_y = y + (h / 2);

//     LCD_ArcRect(x, y, x + w, y + h, color);
//     LCD_ShowChinese(rect_center_x - str_center_x,
//                     rect_center_y - str_center_y,
//                     str, WHITE,
//                     color, fontSize, 1)
// }

// void disp_select_box(int x, int w, int y, int h, int line_length, int interval, int color)
// {
//     // interval: 间距
//     x -= interval;     // 向左偏移interval的距离
//     w += interval * 2; // 宽度增加interval*2的距离（矩形框两边对称）
//     y -= interval;     // 向上偏移interval的距离
//     h += interval * 2; // 高度增加interval*2的距离

//     // 左上
//     LCD_DrawLine(x, y, x + line_length, y, color);
//     LCD_DrawLine(x, y, x, y + line_length, color);
//     // 右上
//     LCD_DrawLine(x + w, y, x + w - line_length, y, color);
//     LCD_DrawLine(x + w, y, x + w, y + line_length, color);
//     // 左下
//     LCD_DrawLine(x, y + h, x + line_length, y + h, color);
//     LCD_DrawLine(x, y + h, x, y + h - line_length, color);
//     // 右下
//     LCD_DrawLine(x + w, y + h, x + w - line_length, y + h, color);
//     LCD_DrawLine(x + w, y + h, x + w, y + h - line_length, color);
// }

// void ui_home_page(void)
// {
//     // 关闭背光
//     LCD_BLK_Clr();
//     // 绘制纯黑色背景
//     LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
//     // 绘制一些其他的文字
//     disp_x_center(3, 5, BLUE, 16, (uint8_t *)"立创开发板");
//     disp_x_center(23, 5, BLUE, 16, (uint8_t *)"简易PID入门套件");

//     int x        = 40;
//     int x_offset = 80;
//     int y        = 65;
//     int y_offset = 80;
//     // 任务一：PID定速
//     disp_string_rect(x, x_offset, y, y_offset, 2, 24, (uint8_t *)"定速", BLUE);
//     int x        = 40;
//     int x_offset = 80;
//     int y        = 65;
//     int y_offset = 80;
//     // 任务一：PID定距
//     disp_string_rect(x, x_offset, y, y_offset, 2, 24, (uint8_t *)"定距", BLUE);

//     // TODO: 接着往下写
//     // switch (get_default_page)

// }
