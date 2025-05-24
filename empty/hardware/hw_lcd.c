#include "hw_lcd.h"
#include "lcdfont.h"

#define delay_ms(X) delay_cycles(((CPUCLK_FREQ / 1000) * (X)))

void spi_write_bus(unsigned char dat)
{
    // ��������
    DL_SPI_transmitData8(SPI_LCD_INST, dat);
    // �ȴ�SPI���߿���
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
    LCD_DC_Clear(); // д����
    LCD_Writ_Bus(dat);
    LCD_DC_Set(); // д����
}

/**
 * @brief ��ʼ��LCD
 */
void LCD_Init(void)
{
    /* GPIO����ͼ�λ������г�ʼ�� */

    LCD_RES_Clear(); // ��λ
    delay_ms(30);
    LCD_RES_Set();
    delay_ms(100);
    // LCD_BLK_Set();//�򿪱���
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
 * @brief ����LCD��ַ
 * @param x1 ��ʼX����
 * @param y1 ��ʼY����
 * @param x2 ����X����
 * @param y2 ����Y����
 */
void LCD_Address_Set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    LCD_Write_Register(0x2a); // �е�ַ����
    LCD_Write_Data32(x1);
    LCD_Write_Data32(x2);
    LCD_Write_Register(0x2b); // �е�ַ����
                              // + 35 ��ȷ��������ʾ
    LCD_Write_Data32(y1 + 35);
    LCD_Write_Data32(y2 + 35);
    LCD_Write_Register(0x2c); // ������д
}

/* ********************** ����Ϊ����UI ********************** */

/**
 * @brief ���ָ��������ɫ
 * @param xsta ��ʼX����
 * @param ysta ��ʼY����
 * @param xend ����X����
 * @param yend ����Y����
 * @param color �����ɫ
 */
void LCD_Fill(unsigned int xsta, unsigned int ysta, unsigned int xend, unsigned int yend, unsigned int color)
{
    unsigned int i, j;
    LCD_Address_Set(xsta, ysta, xend - 1, yend - 1); // ������ʾ��Χ
    for (i = ysta; i < yend; i++) {
        for (j = xsta; j < xend; j++) {
            LCD_Write_Data32(color);
        }
    }
}

/**
 * @brief ��ָ��λ�û�һ����
 * @param x ���X����
 * @param y ���Y����
 * @param color �����ɫ
 */
void LCD_Draw_Point(unsigned int x, unsigned int y, unsigned int color)
{
    LCD_Address_Set(x, y, x, y); // ���ù��λ��
    LCD_Write_Data32(color);
}

/**
 * @brief ��ָ��λ�û�һ����
 * @param x1 ��ʼX����
 * @param y1 ��ʼY����
 * @param x2 ����X����
 * @param y2 ����Y����
 * @param color �ߵ���ɫ
 */
void LCD_Draw_Line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color)
{
    unsigned int t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; // ������������
    delta_y = y2 - y1;
    uRow    = x1; // �����������
    uCol    = y1;
    if (delta_x > 0)
        incx = 1; // ���õ�������
    else if (delta_x == 0)
        incx = 0; // ��ֱ��
    else {
        incx    = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0; // ˮƽ��
    else {
        incy    = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
        distance = delta_x; // ѡȡ��������������
    else
        distance = delta_y;
    for (t = 0; t < distance + 1; t++) {
        LCD_Draw_Point(uRow, uCol, color); // ����
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
 * @brief ��ָ��λ�û�һ������
 * @param x1 ��ʼX����
 * @param y1 ��ʼY����
 * @param x2 ����X����
 * @param y2 ����Y����
 * @param color ���ε���ɫ
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
 * @brief ��ָ��λ�û�һ������
 * @param x1 ��ʼX����
 * @param y1 ��ʼY����
 * @param x2 ����X����
 * @param y2 ����Y����
 * @param color ���ε���ɫ
 */
void LCD_Draw_Rectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color)
{
    LCD_Draw_Line(x1, y1, x2, y1, color);
    LCD_Draw_Line(x1, y1, x1, y2, color);
    LCD_Draw_Line(x1, y2, x2, y2, color);
    LCD_Draw_Line(x2, y1, x2, y2, color);
}

/**
 * @brief ��ָ��λ�û�һ��Բ
 * @param x0 Բ�ĵ�X����
 * @param y0 Բ�ĵ�Y����
 * @param r Բ�İ뾶
 * @param color Բ����ɫ
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
        if ((a * a + b * b) > (r * r)) // �ж�Ҫ���ĵ��Ƿ��Զ
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
 * @brief ��Բ�Ǿ���
 * @param xsta ��ʼX����
 * @param ysta ��ʼY����
 * @param xend ����X����
 * @param yend ����Y����
 * @param color ���ε���ɫ
 */
void LCD_Draw_ArcRect(unsigned int xsta, unsigned int ysta, unsigned int xend, unsigned int yend, unsigned int color)
{

    int r = 4;
    // ��������
    LCD_Draw_Line(xsta + r, ysta, xend - r, ysta, color);
    LCD_Draw_Line(xsta, ysta + r, xsta, yend - r, color);
    LCD_Draw_Line(xsta + r, yend, xend - r, yend, color);
    LCD_Draw_Line(xend, ysta + r, xend, yend - r, color);

    // �ٻ��ĸ�Բ��
    Drawarc(xsta + r, ysta + r, 90, 180, r, color);  // ����
    Drawarc(xend - r, ysta + r, 0, 90, r, color);    // ����
    Drawarc(xsta + r, yend - r, 180, 270, r, color); // ����
    Drawarc(xend - r, yend - r, 270, 360, r, color); // ����

    // �����ʵ�ľ���
    LCD_Fill(xsta + r, ysta, xend - r, ysta + r, color);     // ��
    LCD_Fill(xend - r, ysta + r, xend, yend - r, color);     // ��
    LCD_Fill(xsta + r, yend - r, xend - r, yend, color);     // ��
    LCD_Fill(xsta, ysta + r, xsta + r, yend - r, color);     // ��
    LCD_Fill(xsta + r, ysta + r, xend - r, yend - r, color); // ��
}

/**
 * @brief ��ʾ���ִ�
 * @param x ������ʾ��X����
 * @param y ������ʾ��Y����
 * @param s �����ַ���ָ��
 * @param fc ǰ��ɫ��������ɫ��
 * @param bc ����ɫ
 * @param sizey �����С
 * @param mode ��ʾģʽ
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
            s += 3; // utf-8 ����ռ��3���ֽ�
            x += sizey;
        } else {
            LCD_Show_Char(x, y, *s, fc, bc, sizey, mode);
            s += 1;
            x += (sizey / 2);
        }
    }
}

/**
 * @brief ��ʾ����12x12����
 * @param x ������ʾ��X����
 * @param y ������ʾ��Y����
 * @param s �����ַ���ָ��
 * @param fc ǰ��ɫ��������ɫ��
 * @param bc ����ɫ
 * @param sizey �����С
 * @param mode ��ʾģʽ
 */
void LCD_Show_Chinese12x12(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    unsigned char i, j, m = 0;
    unsigned int k;
    unsigned int HZnum;       // ������Ŀ
    unsigned int TypefaceNum; // һ���ַ���ռ�ֽڴ�С
    unsigned int x0 = x;
    TypefaceNum     = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;

    HZnum = sizeof(tfont12) / sizeof(typFNT_GB12); // ͳ�ƺ�����Ŀ
    for (k = 0; k < HZnum; k++) {
        if ((tfont12[k].Index[0] == *(s)) && (tfont12[k].Index[1] == *(s + 1))) {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode) // �ǵ��ӷ�ʽ
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
                    } else // ���ӷ�ʽ
                    {
                        if (tfont12[k].Msk[i] & (0x01 << j)) LCD_Draw_Point(x, y, fc); // ��һ����
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
        continue; // ���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
    }
}

/**
 * @brief ��ʾ����16x16����
 * @param x ������ʾ��X����
 * @param y ������ʾ��Y����
 * @param s �����ַ���ָ��
 * @param fc ǰ��ɫ��������ɫ��
 * @param bc ����ɫ
 * @param sizey �����С
 * @param mode ��ʾģʽ
 */
void LCD_Show_Chinese16x16(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    unsigned char i, j, m = 0;
    unsigned int k;
    unsigned int HZnum;       // ������Ŀ
    unsigned int TypefaceNum; // һ���ַ���ռ�ֽڴ�С
    unsigned int x0 = x;
    TypefaceNum     = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey; // 32
    HZnum           = sizeof(tfont16) / sizeof(typFNT_GB16);       // ͳ�ƺ�����Ŀ
    for (k = 0; k < HZnum; k++) {
        if ((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s + 1))) {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode) // �ǵ��ӷ�ʽ
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
                    } else // ���ӷ�ʽ
                    {
                        if (tfont16[k].Msk[i] & (0x01 << j)) LCD_Draw_Point(x, y, fc); // ��һ����
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
        continue; // ���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
    }
}

/**
 * @brief ��ʾ����24x24����
 * @param x ������ʾ��X����
 * @param y ������ʾ��Y����
 * @param s �����ַ���ָ��
 * @param fc ǰ��ɫ��������ɫ��
 * @param bc ����ɫ
 * @param sizey �����С
 * @param mode ��ʾģʽ
 */
void LCD_Show_Chinese24x24(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    unsigned char i, j, m = 0;
    unsigned int k;
    unsigned int HZnum;       // ������Ŀ
    unsigned int TypefaceNum; // һ���ַ���ռ�ֽڴ�С
    unsigned int x0 = x;
    TypefaceNum     = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum           = sizeof(tfont24) / sizeof(typFNT_GB24); // ͳ�ƺ�����Ŀ
    for (k = 0; k < HZnum; k++) {
        if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s + 1))) {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode) // �ǵ��ӷ�ʽ
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
                    } else // ���ӷ�ʽ
                    {
                        if (tfont24[k].Msk[i] & (0x01 << j)) LCD_Draw_Point(x, y, fc); // ��һ����
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
        continue; // ���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
    }
}

/**
 * @brief ��ʾ����32x32����
 * @param x ������ʾ��X����
 * @param y ������ʾ��Y����
 * @param s �����ַ���ָ��
 * @param fc ǰ��ɫ��������ɫ��
 * @param bc ����ɫ
 * @param sizey �����С
 * @param mode ��ʾģʽ
 */
void LCD_Show_Chinese32x32(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    unsigned char i, j, m = 0;
    unsigned int k;
    unsigned int HZnum;       // ������Ŀ
    unsigned int TypefaceNum; // һ���ַ���ռ�ֽڴ�С
    unsigned int x0 = x;
    TypefaceNum     = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum           = sizeof(tfont32) / sizeof(typFNT_GB32); // ͳ�ƺ�����Ŀ
    for (k = 0; k < HZnum; k++) {
        if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s + 1))) {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode) // �ǵ��ӷ�ʽ
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
                    } else // ���ӷ�ʽ
                    {
                        if (tfont32[k].Msk[i] & (0x01 << j)) LCD_Draw_Point(x, y, fc); // ��һ����
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
        continue; // ���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
    }
}

/**
 * @brief ��ʾһ���ַ�
 * @param x �ַ���ʾ��X����
 * @param y �ַ���ʾ��Y����
 * @param num �ַ�����
 * @param fc ǰ��ɫ��������ɫ��
 * @param bc ����ɫ
 * @param sizey �����С
 * @param mode ��ʾģʽ
 */
void LCD_Show_Char(unsigned int x, unsigned int y, unsigned char num, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    unsigned char temp, sizex, t, m = 0;
    unsigned int i, TypefaceNum; // һ���ַ���ռ�ֽڴ�С
    unsigned int x0 = x;
    sizex           = sizey / 2;
    TypefaceNum     = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
    num             = num - ' ';                         // �õ�ƫ�ƺ��ֵ
    LCD_Address_Set(x, y, x + sizex - 1, y + sizey - 1); // ���ù��λ��
    for (i = 0; i < TypefaceNum; i++) {
        if (sizey == 12)
            temp = ascii_1206[num][i]; // ����6x12����
        else if (sizey == 16)
            temp = ascii_1608[num][i]; // ����8x16����
        else if (sizey == 24)
            temp = ascii_2412[num][i]; // ����12x24����
        else if (sizey == 32)
            temp = ascii_3216[num][i]; // ����16x32����
        else
            return;
        for (t = 0; t < 8; t++) {
            if (!mode) // �ǵ���ģʽ
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
            } else // ����ģʽ
            {
                if (temp & (0x01 << t)) LCD_Draw_Point(x, y, fc); // ��һ����
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
 * @brief ��ʾ�ַ���
 * @param x �ַ�����ʾ��X����
 * @param y �ַ�����ʾ��Y����
 * @param p �ַ���ָ��
 * @param fc ǰ��ɫ��������ɫ��
 * @param bc ����ɫ
 * @param sizey �����С
 * @param mode ��ʾģʽ
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
 * @brief ����
 * @param m ����
 * @param n ָ��
 * @return m��n����
 */
unsigned int mypow(unsigned char m, unsigned char n)
{
    unsigned int result = 1;
    while (n--) result *= m;
    return result;
}

/**
 * @brief ��ʾ��������
 * @param x ��ʾ��X����
 * @param y ��ʾ��Y����
 * @param num Ҫ��ʾ������
 * @param len �����ĳ���
 * @param fc ǰ��ɫ��������ɫ��
 * @param bc ����ɫ
 * @param sizey �����С
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
 * @brief ��ʾ��λС������
 * @param x ��ʾ��X����
 * @param y ��ʾ��Y����
 * @param num Ҫ��ʾ��С��
 * @param len С���ĳ���
 * @param fc ǰ��ɫ��������ɫ��
 * @param bc ����ɫ
 * @param sizey �����С
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
 * @brief ��ʾͼƬ
 * @param x ͼƬ��ʾ��X����
 * @param y ͼƬ��ʾ��Y����
 * @param length ͼƬ�ĳ���
 * @param width ͼƬ�Ŀ��
 * @param pic ͼƬ����ָ��
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
