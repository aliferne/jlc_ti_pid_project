/**
 * @file HW_LCD_H
 * @brief LCD硬件驱动头文件
 */

#ifndef __HW_LCD_H__
#define __HW_LCD_H__

#include "ti_msp_dl_config.h"

/**
 * @brief LCD宽度
 */
#define LCD_W 320

/**
 * @brief LCD高度
 */
#define LCD_H 170

/**
 * @brief 屏幕中心X坐标
 */
#define screen_center_x (LCD_W / 2)

/**
 * @brief 屏幕中心Y坐标
 */
#define screen_center_y (LCD_H / 2)

/**
 * @brief 白色
 */
#define WHITE 0xFFFF

/**
 * @brief 黑色
 */
#define BLACK 0x0000

/**
 * @brief 蓝色
 */
#define BLUE 0x001F

/**
 * @brief 棕红色
 */
#define BRED 0XF81F

/**
 * @brief 绿红色
 */
#define GRED 0XFFE0

/**
 * @brief 绿蓝色
 */
#define GBLUE 0X07FF

/**
 * @brief 红色
 */
#define RED 0xF800

/**
 * @brief 品红色
 */
#define MAGENTA 0xF81F

/**
 * @brief 绿色
 */
#define GREEN 0x07E0

/**
 * @brief 青色
 */
#define CYAN 0x7FFF

/**
 * @brief 黄色
 */
#define YELLOW 0xFFE0

/**
 * @brief 棕色
 */
#define BROWN 0XBC40

/**
 * @brief 棕红色
 */
#define BRRED 0XFC07

/**
 * @brief 灰色
 */
#define GRAY 0X8430

/**
 * @brief 深蓝色
 */
#define DARKBLUE 0X01CF

/**
 * @brief 浅蓝色
 */
#define LIGHTBLUE 0X7D7C

/**
 * @brief 灰蓝色
 */
#define GRAYBLUE 0X5458

/**
 * @brief 浅绿色
 */
#define LIGHTGREEN 0X841F

/**
 * @brief 浅灰色
 */
#define LGRAY 0XC618

/**
 * @brief 浅灰蓝色
 */
#define LGRAYBLUE 0XA651

/**
 * @brief 浅棕蓝色
 */
#define LBBLUE 0X2B12

/**
 * @brief 清除LCD复位引脚
 */
#define LCD_RES_Clear() DL_GPIO_clearPins(GPIO_LCD_PORT, GPIO_LCD_PIN_RES_PIN)

/**
 * @brief 设置LCD复位引脚
 */
#define LCD_RES_Set() DL_GPIO_setPins(GPIO_LCD_PORT, GPIO_LCD_PIN_RES_PIN)

/**
 * @brief 清除LCD数据/命令引脚
 */
#define LCD_DC_Clear() DL_GPIO_clearPins(GPIO_LCD_PORT, GPIO_LCD_PIN_DC_PIN)

/**
 * @brief 设置LCD数据/命令引脚
 */
#define LCD_DC_Set() DL_GPIO_setPins(GPIO_LCD_PORT, GPIO_LCD_PIN_DC_PIN)

/**
 * @brief 清除LCD片选引脚
 */
#define LCD_CS_Clear() DL_GPIO_clearPins(GPIO_LCD_PORT, GPIO_LCD_PIN_CS_PIN)

/**
 * @brief 设置LCD片选引脚
 */
#define LCD_CS_Set() DL_GPIO_setPins(GPIO_LCD_PORT, GPIO_LCD_PIN_CS_PIN)

/**
 * @brief 清除LCD背光引脚
 */
#define LCD_BLK_Clear() DL_GPIO_clearPins(GPIO_LCD_PORT, GPIO_LCD_PIN_BLK_PIN)

/**
 * @brief 设置LCD背光引脚
 */
#define LCD_BLK_Set() DL_GPIO_setPins(GPIO_LCD_PORT, GPIO_LCD_PIN_BLK_PIN)

/**
 * @brief 写入8位数据到LCD
 * @param dat 要写入的数据
 */
void LCD_Write_Data8(unsigned char dat);

/**
 * @brief 初始化LCD
 */
void LCD_Init(void);

/**
 * @brief 设置LCD地址
 * @param x1 起始X坐标
 * @param y1 起始Y坐标
 * @param x2 结束X坐标
 * @param y2 结束Y坐标
 */
void LCD_Address_Set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

/**
 * @brief 填充指定区域颜色
 * @param xsta 起始X坐标
 * @param ysta 起始Y坐标
 * @param xend 结束X坐标
 * @param yend 结束Y坐标
 * @param color 填充颜色
 */
void LCD_Fill(unsigned int xsta, unsigned int ysta, unsigned int xend, unsigned int yend, unsigned int color);

/**
 * @brief 在指定位置画一个点
 * @param x 点的X坐标
 * @param y 点的Y坐标
 * @param color 点的颜色
 */
void LCD_Draw_Point(unsigned int x, unsigned int y, unsigned int color);

/**
 * @brief 在指定位置画一条线
 * @param x1 起始X坐标
 * @param y1 起始Y坐标
 * @param x2 结束X坐标
 * @param y2 结束Y坐标
 * @param color 线的颜色
 */
void LCD_Draw_Line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color);

/**
 * @brief 画垂直线
 * @param x 线的X坐标
 * @param y1 起始Y坐标
 * @param y2 结束Y坐标
 * @param color 线的颜色
 */
void LCD_Draw_VerrticalLine(int x, int y1, int y2, unsigned int color);

/**
 * @brief 在指定位置画一个矩形
 * @param x1 起始X坐标
 * @param y1 起始Y坐标
 * @param x2 结束X坐标
 * @param y2 结束Y坐标
 * @param color 矩形的颜色
 */
void LCD_Draw_Rectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color);

/**
 * @brief 在指定位置画一个圆
 * @param x0 圆心的X坐标
 * @param y0 圆心的Y坐标
 * @param r 圆的半径
 * @param color 圆的颜色
 */
void LCD_Draw_Circle(unsigned int x0, unsigned int y0, unsigned char r, unsigned int color);

/**
 * @brief 画圆角矩形
 * @param xsta 起始X坐标
 * @param ysta 起始Y坐标
 * @param xend 结束X坐标
 * @param yend 结束Y坐标
 * @param color 矩形的颜色
 */
void LCD_Draw_ArcRect(unsigned int xsta, unsigned int ysta, unsigned int xend, unsigned int yend, unsigned int color);

/**
 * @brief 显示汉字串
 * @param x 汉字显示的X坐标
 * @param y 汉字显示的Y坐标
 * @param s 汉字字符串指针
 * @param fc 前景色（字体颜色）
 * @param bc 背景色
 * @param sizey 字体大小
 * @param mode 显示模式
 */
void LCD_Show_Chinese(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);

/**
 * @brief 显示单个12x12汉字
 * @param x 汉字显示的X坐标
 * @param y 汉字显示的Y坐标
 * @param s 汉字字符串指针
 * @param fc 前景色（字体颜色）
 * @param bc 背景色
 * @param sizey 字体大小
 * @param mode 显示模式
 */
void LCD_Show_Chinese12x12(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);

/**
 * @brief 显示单个16x16汉字
 * @param x 汉字显示的X坐标
 * @param y 汉字显示的Y坐标
 * @param s 汉字字符串指针
 * @param fc 前景色（字体颜色）
 * @param bc 背景色
 * @param sizey 字体大小
 * @param mode 显示模式
 */
void LCD_Show_Chinese16x16(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);

/**
 * @brief 显示单个24x24汉字
 * @param x 汉字显示的X坐标
 * @param y 汉字显示的Y坐标
 * @param s 汉字字符串指针
 * @param fc 前景色（字体颜色）
 * @param bc 背景色
 * @param sizey 字体大小
 * @param mode 显示模式
 */
void LCD_Show_Chinese24x24(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);

/**
 * @brief 显示单个32x32汉字
 * @param x 汉字显示的X坐标
 * @param y 汉字显示的Y坐标
 * @param s 汉字字符串指针
 * @param fc 前景色（字体颜色）
 * @param bc 背景色
 * @param sizey 字体大小
 * @param mode 显示模式
 */
void LCD_Show_Chinese32x32(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);

/**
 * @brief 显示一个字符
 * @param x 字符显示的X坐标
 * @param y 字符显示的Y坐标
 * @param num 字符编码
 * @param fc 前景色（字体颜色）
 * @param bc 背景色
 * @param sizey 字体大小
 * @param mode 显示模式
 */
void LCD_Show_Char(unsigned int x, unsigned int y, unsigned char num, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);

/**
 * @brief 显示字符串
 * @param x 字符串显示的X坐标
 * @param y 字符串显示的Y坐标
 * @param p 字符串指针
 * @param fc 前景色（字体颜色）
 * @param bc 背景色
 * @param sizey 字体大小
 * @param mode 显示模式
 */
void LCD_Show_String(unsigned int x, unsigned int y, const unsigned char *p, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);

/**
 * @brief 求幂
 * @param m 底数
 * @param n 指数
 * @return m的n次幂
 */
unsigned int mypow(unsigned char m, unsigned char n);

/**
 * @brief 显示整数变量
 * @param x 显示的X坐标
 * @param y 显示的Y坐标
 * @param num 要显示的整数
 * @param len 整数的长度
 * @param fc 前景色（字体颜色）
 * @param bc 背景色
 * @param sizey 字体大小
 */
void LCD_Show_IntNum(unsigned int x, unsigned int y, unsigned int num, unsigned char len, unsigned int fc, unsigned int bc, unsigned char sizey);

/**
 * @brief 显示两位小数变量
 * @param x 显示的X坐标
 * @param y 显示的Y坐标
 * @param num 要显示的小数
 * @param len 小数的长度
 * @param fc 前景色（字体颜色）
 * @param bc 背景色
 * @param sizey 字体大小
 */
void LCD_Show_FloatNum(unsigned int x, unsigned int y, float num, unsigned char len, unsigned int fc, unsigned int bc, unsigned char sizey);

/**
 * @brief 显示图片
 * @param x 图片显示的X坐标
 * @param y 图片显示的Y坐标
 * @param length 图片的长度
 * @param width 图片的宽度
 * @param pic 图片数据指针
 */
void LCD_ShowPicture(unsigned int x, unsigned int y, unsigned int length, unsigned int width, const unsigned char pic[]);

#endif
