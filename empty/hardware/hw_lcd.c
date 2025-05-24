#include "hw_lcd.h"
#include "lcdfont.h"

#define delay_ms(X) delay_cycles(((CPUCLK_FREQ / 1000) * (X)))

void spi_write_bus(unsigned char dat)
{
    // 发送数据
    DL_SPI_transmitData8(SPI_LCD_INST, dat);
    // 等待SPI总线空闲
    while (DL_SPI_isBusy(SPI_LCD_INST));
}

void LCD_Writ_Bus(unsigned char dat)
{
    LCD_CS_Clear();
    spi_write_bus(dat);
    LCD_CS_Set();
}

void LCD_Write_Data8(unsigned char dat)
{
    LCD_Writ_Bus(dat);
}

void LCD_Write_Data32(unsigned int dat)
{
    LCD_Writ_Bus(dat >> 8);
    LCD_Writ_Bus(dat);
}

void LCD_Write_Register(unsigned char dat)
{
    LCD_DC_Clear(); // 写命令
    LCD_Writ_Bus(dat);
    LCD_DC_Set(); // 写数据
}

/**
 * @brief 初始化LCD
 */
void LCD_Init(void)
{
    /* GPIO已在图形化工具中初始化 */

    LCD_RES_Clear(); // 复位
    delay_ms(30);
    LCD_RES_Set();
    delay_ms(100);
    // LCD_BLK_Set();//打开背光
    // delay_ms(100);
    LCD_Write_Register(0x11);

    LCD_Write_Register(0x36);
    LCD_Write_Data8(0x70);

    LCD_Write_Register(0x3A);
    LCD_Write_Data8(0x05);

    LCD_Write_Register(0xB2);
    LCD_Write_Data8(0x0C);
    LCD_Write_Data8(0x0C);
    LCD_Write_Data8(0x00);
    LCD_Write_Data8(0x33);
    LCD_Write_Data8(0x33);

    LCD_Write_Register(0xB7);
    LCD_Write_Data8(0x35);

    LCD_Write_Register(0xBB);
    LCD_Write_Data8(0x1A);

    LCD_Write_Register(0xC0);
    LCD_Write_Data8(0x2C);

    LCD_Write_Register(0xC2);
    LCD_Write_Data8(0x01);

    LCD_Write_Register(0xC3);
    LCD_Write_Data8(0x0B);

    LCD_Write_Register(0xC4);
    LCD_Write_Data8(0x20);

    LCD_Write_Register(0xC6);
    LCD_Write_Data8(0x0F);

    LCD_Write_Register(0xD0);
    LCD_Write_Data8(0xA4);
    LCD_Write_Data8(0xA1);

    LCD_Write_Register(0x21);
    LCD_Write_Register(0xE0);
    LCD_Write_Data8(0xF0);
    LCD_Write_Data8(0x00);
    LCD_Write_Data8(0x04);
    LCD_Write_Data8(0x04);
    LCD_Write_Data8(0x04);
    LCD_Write_Data8(0x05);
    LCD_Write_Data8(0x29);
    LCD_Write_Data8(0x33);
    LCD_Write_Data8(0x3E);
    LCD_Write_Data8(0x38);
    LCD_Write_Data8(0x12);
    LCD_Write_Data8(0x12);
    LCD_Write_Data8(0x28);
    LCD_Write_Data8(0x30);

    LCD_Write_Register(0xE1);
    LCD_Write_Data8(0xF0);
    LCD_Write_Data8(0x07);
    LCD_Write_Data8(0x0A);
    LCD_Write_Data8(0x0D);
    LCD_Write_Data8(0x0B);
    LCD_Write_Data8(0x07);
    LCD_Write_Data8(0x28);
    LCD_Write_Data8(0x33);
    LCD_Write_Data8(0x3E);
    LCD_Write_Data8(0x36);
    LCD_Write_Data8(0x14);
    LCD_Write_Data8(0x14);
    LCD_Write_Data8(0x29);
    LCD_Write_Data8(0x32);
    LCD_Write_Register(0x11);
    delay_ms(120);
    LCD_Write_Register(0x29);
}

/**
 * @brief 设置LCD地址
 * @param x1 起始X坐标
 * @param y1 起始Y坐标
 * @param x2 结束X坐标
 * @param y2 结束Y坐标
 */
void LCD_Address_Set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    LCD_Write_Register(0x2a); // 列地址设置
    LCD_Write_Data32(x1);
    LCD_Write_Data32(x2);
    LCD_Write_Register(0x2b); // 行地址设置
                              // + 35 以确保正常显示
    LCD_Write_Data32(y1 + 35);
    LCD_Write_Data32(y2 + 35);
    LCD_Write_Register(0x2c); // 储存器写
}

/* ********************** 以下为基础UI ********************** */

/**
 * @brief 填充指定区域颜色
 * @param xsta 起始X坐标
 * @param ysta 起始Y坐标
 * @param xend 结束X坐标
 * @param yend 结束Y坐标
 * @param color 填充颜色
 */
void LCD_Fill(unsigned int xsta, unsigned int ysta, unsigned int xend, unsigned int yend, unsigned int color)
{
    unsigned int i, j;
    LCD_Address_Set(xsta, ysta, xend - 1, yend - 1); // 设置显示范围
    for (i = ysta; i < yend; i++) {
        for (j = xsta; j < xend; j++) {
            LCD_Write_Data32(color);
        }
    }
}

/**
 * @brief 在指定位置画一个点
 * @param x 点的X坐标
 * @param y 点的Y坐标
 * @param color 点的颜色
 */
void LCD_Draw_Point(unsigned int x, unsigned int y, unsigned int color)
{
    LCD_Address_Set(x, y, x, y); // 设置光标位置
    LCD_Write_Data32(color);
}

/**
 * @brief 在指定位置画一条线
 * @param x1 起始X坐标
 * @param y1 起始Y坐标
 * @param x2 结束X坐标
 * @param y2 结束Y坐标
 * @param color 线的颜色
 */
void LCD_Draw_Line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color)
{
    unsigned int t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; // 计算坐标增量
    delta_y = y2 - y1;
    uRow    = x1; // 画线起点坐标
    uCol    = y1;
    if (delta_x > 0)
        incx = 1; // 设置单步方向
    else if (delta_x == 0)
        incx = 0; // 垂直线
    else {
        incx    = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0; // 水平线
    else {
        incy    = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
        distance = delta_x; // 选取基本增量坐标轴
    else
        distance = delta_y;
    for (t = 0; t < distance + 1; t++) {
        LCD_Draw_Point(uRow, uCol, color); // 画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance) {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance) {
            yerr -= distance;
            uCol += incy;
        }
    }
}

/**
 * @brief 在指定位置画一个矩形
 * @param x1 起始X坐标
 * @param y1 起始Y坐标
 * @param x2 结束X坐标
 * @param y2 结束Y坐标
 * @param color 矩形的颜色
 */
void LCD_Draw_VerrticalLine(int x, int y1, int y2, unsigned int color)
{
    int i;
    LCD_Address_Set(x, y1, x, y1 + y2);
    for (i = y1; i <= y1 + y2; i++) {
        LCD_Write_Data32(color);
    }
}

/**
 * @brief 在指定位置画一个矩形
 * @param x1 起始X坐标
 * @param y1 起始Y坐标
 * @param x2 结束X坐标
 * @param y2 结束Y坐标
 * @param color 矩形的颜色
 */
void LCD_Draw_Rectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color)
{
    LCD_Draw_Line(x1, y1, x2, y1, color);
    LCD_Draw_Line(x1, y1, x1, y2, color);
    LCD_Draw_Line(x1, y2, x2, y2, color);
    LCD_Draw_Line(x2, y1, x2, y2, color);
}

/**
 * @brief 在指定位置画一个圆
 * @param x0 圆心的X坐标
 * @param y0 圆心的Y坐标
 * @param r 圆的半径
 * @param color 圆的颜色
 */
void LCD_Draw_Circle(unsigned int x0, unsigned int y0, unsigned char r, unsigned int color)
{
    int a, b;
    a = 0;
    b = r;
    while (a <= b) {
        LCD_Draw_Point(x0 - b, y0 - a, color); // 3
        LCD_Draw_Point(x0 + b, y0 - a, color); // 0
        LCD_Draw_Point(x0 - a, y0 + b, color); // 1
        LCD_Draw_Point(x0 - a, y0 - b, color); // 2
        LCD_Draw_Point(x0 + b, y0 + a, color); // 4
        LCD_Draw_Point(x0 + a, y0 - b, color); // 5
        LCD_Draw_Point(x0 + a, y0 + b, color); // 6
        LCD_Draw_Point(x0 - b, y0 + a, color); // 7
        a++;
        if ((a * a + b * b) > (r * r)) // 判断要画的点是否过远
        {
            b--;
        }
    }
}

void Drawarc(int x, int y, int a, int b, unsigned int r, unsigned int c)
{
    int x_tp, y_tp;
    int d; // Decision variable

    // Start with the top and bottom rows of the circle
    for (x_tp = 0; x_tp <= r; x_tp++) {
        // Calculate the corresponding y values
        y_tp = (int)sqrt(r * r - x_tp * x_tp);

        // Draw the horizontal lines from the circle edge to the center
        for (int i = x_tp; i >= -x_tp; i--) {
            LCD_Draw_Point(x + i, y - y_tp, c);
            LCD_Draw_Point(x + i, y + y_tp, c);
        }
    }

    // Now fill the rest of the circle
    d = 3 - 2 * r;
    while (x_tp > y_tp) {
        if (d < 0) {
            d += 4 * x_tp + 6;
        } else {
            d += 4 * (x_tp - y_tp) + 10;
            y_tp++;
        }
        x_tp--;

        // Draw the horizontal lines from the circle edge to the center
        for (int i = -x_tp; i <= x_tp; i++) {
            LCD_Draw_Point(x + i, y - y_tp, c);
            LCD_Draw_Point(x + i, y + y_tp, c);
        }

        // Draw the vertical lines from the circle edge to the center
        for (int i = -y_tp; i <= y_tp; i++) {
            LCD_Draw_Point(x + x_tp, y + i, c);
            LCD_Draw_Point(x - x_tp, y + i, c);
        }
    }
}

/**
 * @brief 画圆角矩形
 * @param xsta 起始X坐标
 * @param ysta 起始Y坐标
 * @param xend 结束X坐标
 * @param yend 结束Y坐标
 * @param color 矩形的颜色
 */
void LCD_Draw_ArcRect(unsigned int xsta, unsigned int ysta, unsigned int xend, unsigned int yend, unsigned int color)
{

    int r = 4;
    // 画四条边
    LCD_Draw_Line(xsta + r, ysta, xend - r, ysta, color);
    LCD_Draw_Line(xsta, ysta + r, xsta, yend - r, color);
    LCD_Draw_Line(xsta + r, yend, xend - r, yend, color);
    LCD_Draw_Line(xend, ysta + r, xend, yend - r, color);

    // 再画四个圆角
    Drawarc(xsta + r, ysta + r, 90, 180, r, color);  // 左上
    Drawarc(xend - r, ysta + r, 0, 90, r, color);    // 右上
    Drawarc(xsta + r, yend - r, 180, 270, r, color); // 左下
    Drawarc(xend - r, yend - r, 270, 360, r, color); // 右下

    // 画五个实心矩形
    LCD_Fill(xsta + r, ysta, xend - r, ysta + r, color);     // 上
    LCD_Fill(xend - r, ysta + r, xend, yend - r, color);     // 右
    LCD_Fill(xsta + r, yend - r, xend - r, yend, color);     // 下
    LCD_Fill(xsta, ysta + r, xsta + r, yend - r, color);     // 左
    LCD_Fill(xsta + r, ysta + r, xend - r, yend - r, color); // 中
}

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
void LCD_Show_Chinese(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    while (*s != 0) {
        if (*s >= 128) {
            if (sizey == 12)
                LCD_Show_Chinese12x12(x, y, s, fc, bc, sizey, mode);
            else if (sizey == 16)
                LCD_Show_Chinese16x16(x, y, s, fc, bc, sizey, mode);
            else if (sizey == 24)
                LCD_Show_Chinese24x24(x, y, s, fc, bc, sizey, mode);
            else if (sizey == 32)
                LCD_Show_Chinese32x32(x, y, s, fc, bc, sizey, mode);
            else
                return;
            s += 3; // utf-8 汉字占用3个字节
            x += sizey;
        } else {
            LCD_Show_Char(x, y, *s, fc, bc, sizey, mode);
            s += 1;
            x += (sizey / 2);
        }
    }
}

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
void LCD_Show_Chinese12x12(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    unsigned char i, j, m = 0;
    unsigned int k;
    unsigned int HZnum;       // 汉字数目
    unsigned int TypefaceNum; // 一个字符所占字节大小
    unsigned int x0 = x;
    TypefaceNum     = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;

    HZnum = sizeof(tfont12) / sizeof(typFNT_GB12); // 统计汉字数目
    for (k = 0; k < HZnum; k++) {
        if ((tfont12[k].Index[0] == *(s)) && (tfont12[k].Index[1] == *(s + 1))) {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode) // 非叠加方式
                    {
                        if (tfont12[k].Msk[i] & (0x01 << j))
                            LCD_Write_Data32(fc);
                        else
                            LCD_Write_Data32(bc);
                        m++;
                        if (m % sizey == 0) {
                            m = 0;
                            break;
                        }
                    } else // 叠加方式
                    {
                        if (tfont12[k].Msk[i] & (0x01 << j)) LCD_Draw_Point(x, y, fc); // 画一个点
                        x++;
                        if ((x - x0) == sizey) {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

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
void LCD_Show_Chinese16x16(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    unsigned char i, j, m = 0;
    unsigned int k;
    unsigned int HZnum;       // 汉字数目
    unsigned int TypefaceNum; // 一个字符所占字节大小
    unsigned int x0 = x;
    TypefaceNum     = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey; // 32
    HZnum           = sizeof(tfont16) / sizeof(typFNT_GB16);       // 统计汉字数目
    for (k = 0; k < HZnum; k++) {
        if ((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s + 1))) {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode) // 非叠加方式
                    {
                        if (tfont16[k].Msk[i] & (0x01 << j))
                            LCD_Write_Data32(fc);
                        else
                            LCD_Write_Data32(bc);
                        m++;
                        if (m % sizey == 0) {
                            m = 0;
                            break;
                        }
                    } else // 叠加方式
                    {
                        if (tfont16[k].Msk[i] & (0x01 << j)) LCD_Draw_Point(x, y, fc); // 画一个点
                        x++;
                        if ((x - x0) == sizey) {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

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
void LCD_Show_Chinese24x24(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    unsigned char i, j, m = 0;
    unsigned int k;
    unsigned int HZnum;       // 汉字数目
    unsigned int TypefaceNum; // 一个字符所占字节大小
    unsigned int x0 = x;
    TypefaceNum     = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum           = sizeof(tfont24) / sizeof(typFNT_GB24); // 统计汉字数目
    for (k = 0; k < HZnum; k++) {
        if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s + 1))) {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode) // 非叠加方式
                    {
                        if (tfont24[k].Msk[i] & (0x01 << j))
                            LCD_Write_Data32(fc);
                        else
                            LCD_Write_Data32(bc);
                        m++;
                        if (m % sizey == 0) {
                            m = 0;
                            break;
                        }
                    } else // 叠加方式
                    {
                        if (tfont24[k].Msk[i] & (0x01 << j)) LCD_Draw_Point(x, y, fc); // 画一个点
                        x++;
                        if ((x - x0) == sizey) {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

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
void LCD_Show_Chinese32x32(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    unsigned char i, j, m = 0;
    unsigned int k;
    unsigned int HZnum;       // 汉字数目
    unsigned int TypefaceNum; // 一个字符所占字节大小
    unsigned int x0 = x;
    TypefaceNum     = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum           = sizeof(tfont32) / sizeof(typFNT_GB32); // 统计汉字数目
    for (k = 0; k < HZnum; k++) {
        if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s + 1))) {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode) // 非叠加方式
                    {
                        if (tfont32[k].Msk[i] & (0x01 << j))
                            LCD_Write_Data32(fc);
                        else
                            LCD_Write_Data32(bc);
                        m++;
                        if (m % sizey == 0) {
                            m = 0;
                            break;
                        }
                    } else // 叠加方式
                    {
                        if (tfont32[k].Msk[i] & (0x01 << j)) LCD_Draw_Point(x, y, fc); // 画一个点
                        x++;
                        if ((x - x0) == sizey) {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

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
void LCD_Show_Char(unsigned int x, unsigned int y, unsigned char num, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    unsigned char temp, sizex, t, m = 0;
    unsigned int i, TypefaceNum; // 一个字符所占字节大小
    unsigned int x0 = x;
    sizex           = sizey / 2;
    TypefaceNum     = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
    num             = num - ' ';                         // 得到偏移后的值
    LCD_Address_Set(x, y, x + sizex - 1, y + sizey - 1); // 设置光标位置
    for (i = 0; i < TypefaceNum; i++) {
        if (sizey == 12)
            temp = ascii_1206[num][i]; // 调用6x12字体
        else if (sizey == 16)
            temp = ascii_1608[num][i]; // 调用8x16字体
        else if (sizey == 24)
            temp = ascii_2412[num][i]; // 调用12x24字体
        else if (sizey == 32)
            temp = ascii_3216[num][i]; // 调用16x32字体
        else
            return;
        for (t = 0; t < 8; t++) {
            if (!mode) // 非叠加模式
            {
                if (temp & (0x01 << t))
                    LCD_Write_Data32(fc);
                else
                    LCD_Write_Data32(bc);
                m++;
                if (m % sizex == 0) {
                    m = 0;
                    break;
                }
            } else // 叠加模式
            {
                if (temp & (0x01 << t)) LCD_Draw_Point(x, y, fc); // 画一个点
                x++;
                if ((x - x0) == sizex) {
                    x = x0;
                    y++;
                    break;
                }
            }
        }
    }
}

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
void LCD_Show_String(unsigned int x, unsigned int y, const unsigned char *p, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    while (*p != '\0') {
        LCD_Show_Char(x, y, *p, fc, bc, sizey, mode);
        x += sizey / 2;
        p++;
    }
}

/**
 * @brief 求幂
 * @param m 底数
 * @param n 指数
 * @return m的n次幂
 */
unsigned int mypow(unsigned char m, unsigned char n)
{
    unsigned int result = 1;
    while (n--) result *= m;
    return result;
}

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
void LCD_Show_IntNum(unsigned int x, unsigned int y, unsigned int num, unsigned char len, unsigned int fc, unsigned int bc, unsigned char sizey)
{
    unsigned char t, temp;
    unsigned char enshow = 0;
    unsigned char sizex  = sizey / 2;
    for (t = 0; t < len; t++) {
        temp = (num / mypow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                LCD_Show_Char(x + t * sizex, y, ' ', fc, bc, sizey, 0);
                continue;
            } else
                enshow = 1;
        }
        LCD_Show_Char(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
    }
}

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
void LCD_Show_FloatNum(unsigned int x, unsigned int y, float num, unsigned char len, unsigned int fc, unsigned int bc, unsigned char sizey)
{
    unsigned char t, temp, sizex;
    unsigned int num1;
    sizex = sizey / 2;
    num1  = num * 100;
    for (t = 0; t < len; t++) {
        temp = (num1 / mypow(10, len - t - 1)) % 10;
        if (t == (len - 2)) {
            LCD_Show_Char(x + (len - 2) * sizex, y, '.', fc, bc, sizey, 0);
            t++;
            len += 1;
        }
        LCD_Show_Char(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
    }
}

/**
 * @brief 显示图片
 * @param x 图片显示的X坐标
 * @param y 图片显示的Y坐标
 * @param length 图片的长度
 * @param width 图片的宽度
 * @param pic 图片数据指针
 */
void LCD_ShowPicture(unsigned int x, unsigned int y, unsigned int length, unsigned int width, const unsigned char pic[])
{
    unsigned int i, j;
    uint32_t k = 0;
    LCD_Address_Set(x, y, x + length - 1, y + width - 1);
    for (i = 0; i < length; i++) {
        for (j = 0; j < width; j++) {
            LCD_Write_Data8(pic[k * 2]);
            LCD_Write_Data8(pic[k * 2 + 1]);
            k++;
        }
    }
}
