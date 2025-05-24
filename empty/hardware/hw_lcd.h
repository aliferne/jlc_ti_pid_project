/**
 * @file HW_LCD_H
 * @brief LCDӲ������ͷ�ļ�
 */

#ifndef __HW_LCD_H__
#define __HW_LCD_H__

#include "ti_msp_dl_config.h"

/**
 * @brief LCD���
 */
#define LCD_W 320

/**
 * @brief LCD�߶�
 */
#define LCD_H 170

/**
 * @brief ��Ļ����X����
 */
#define screen_center_x (LCD_W / 2)

/**
 * @brief ��Ļ����Y����
 */
#define screen_center_y (LCD_H / 2)

/**
 * @brief ��ɫ
 */
#define WHITE 0xFFFF

/**
 * @brief ��ɫ
 */
#define BLACK 0x0000

/**
 * @brief ��ɫ
 */
#define BLUE 0x001F

/**
 * @brief �غ�ɫ
 */
#define BRED 0XF81F

/**
 * @brief �̺�ɫ
 */
#define GRED 0XFFE0

/**
 * @brief ����ɫ
 */
#define GBLUE 0X07FF

/**
 * @brief ��ɫ
 */
#define RED 0xF800

/**
 * @brief Ʒ��ɫ
 */
#define MAGENTA 0xF81F

/**
 * @brief ��ɫ
 */
#define GREEN 0x07E0

/**
 * @brief ��ɫ
 */
#define CYAN 0x7FFF

/**
 * @brief ��ɫ
 */
#define YELLOW 0xFFE0

/**
 * @brief ��ɫ
 */
#define BROWN 0XBC40

/**
 * @brief �غ�ɫ
 */
#define BRRED 0XFC07

/**
 * @brief ��ɫ
 */
#define GRAY 0X8430

/**
 * @brief ����ɫ
 */
#define DARKBLUE 0X01CF

/**
 * @brief ǳ��ɫ
 */
#define LIGHTBLUE 0X7D7C

/**
 * @brief ����ɫ
 */
#define GRAYBLUE 0X5458

/**
 * @brief ǳ��ɫ
 */
#define LIGHTGREEN 0X841F

/**
 * @brief ǳ��ɫ
 */
#define LGRAY 0XC618

/**
 * @brief ǳ����ɫ
 */
#define LGRAYBLUE 0XA651

/**
 * @brief ǳ����ɫ
 */
#define LBBLUE 0X2B12

/**
 * @brief ���LCD��λ����
 */
#define LCD_RES_Clear() DL_GPIO_clearPins(GPIO_LCD_PORT, GPIO_LCD_PIN_RES_PIN)

/**
 * @brief ����LCD��λ����
 */
#define LCD_RES_Set() DL_GPIO_setPins(GPIO_LCD_PORT, GPIO_LCD_PIN_RES_PIN)

/**
 * @brief ���LCD����/��������
 */
#define LCD_DC_Clear() DL_GPIO_clearPins(GPIO_LCD_PORT, GPIO_LCD_PIN_DC_PIN)

/**
 * @brief ����LCD����/��������
 */
#define LCD_DC_Set() DL_GPIO_setPins(GPIO_LCD_PORT, GPIO_LCD_PIN_DC_PIN)

/**
 * @brief ���LCDƬѡ����
 */
#define LCD_CS_Clear() DL_GPIO_clearPins(GPIO_LCD_PORT, GPIO_LCD_PIN_CS_PIN)

/**
 * @brief ����LCDƬѡ����
 */
#define LCD_CS_Set() DL_GPIO_setPins(GPIO_LCD_PORT, GPIO_LCD_PIN_CS_PIN)

/**
 * @brief ���LCD��������
 */
#define LCD_BLK_Clear() DL_GPIO_clearPins(GPIO_LCD_PORT, GPIO_LCD_PIN_BLK_PIN)

/**
 * @brief ����LCD��������
 */
#define LCD_BLK_Set() DL_GPIO_setPins(GPIO_LCD_PORT, GPIO_LCD_PIN_BLK_PIN)

/**
 * @brief д��8λ���ݵ�LCD
 * @param dat Ҫд�������
 */
void LCD_Write_Data8(unsigned char dat);

/**
 * @brief ��ʼ��LCD
 */
void LCD_Init(void);

/**
 * @brief ����LCD��ַ
 * @param x1 ��ʼX����
 * @param y1 ��ʼY����
 * @param x2 ����X����
 * @param y2 ����Y����
 */
void LCD_Address_Set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

/**
 * @brief ���ָ��������ɫ
 * @param xsta ��ʼX����
 * @param ysta ��ʼY����
 * @param xend ����X����
 * @param yend ����Y����
 * @param color �����ɫ
 */
void LCD_Fill(unsigned int xsta, unsigned int ysta, unsigned int xend, unsigned int yend, unsigned int color);

/**
 * @brief ��ָ��λ�û�һ����
 * @param x ���X����
 * @param y ���Y����
 * @param color �����ɫ
 */
void LCD_Draw_Point(unsigned int x, unsigned int y, unsigned int color);

/**
 * @brief ��ָ��λ�û�һ����
 * @param x1 ��ʼX����
 * @param y1 ��ʼY����
 * @param x2 ����X����
 * @param y2 ����Y����
 * @param color �ߵ���ɫ
 */
void LCD_Draw_Line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color);

/**
 * @brief ����ֱ��
 * @param x �ߵ�X����
 * @param y1 ��ʼY����
 * @param y2 ����Y����
 * @param color �ߵ���ɫ
 */
void LCD_Draw_VerrticalLine(int x, int y1, int y2, unsigned int color);

/**
 * @brief ��ָ��λ�û�һ������
 * @param x1 ��ʼX����
 * @param y1 ��ʼY����
 * @param x2 ����X����
 * @param y2 ����Y����
 * @param color ���ε���ɫ
 */
void LCD_Draw_Rectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color);

/**
 * @brief ��ָ��λ�û�һ��Բ
 * @param x0 Բ�ĵ�X����
 * @param y0 Բ�ĵ�Y����
 * @param r Բ�İ뾶
 * @param color Բ����ɫ
 */
void LCD_Draw_Circle(unsigned int x0, unsigned int y0, unsigned char r, unsigned int color);

/**
 * @brief ��Բ�Ǿ���
 * @param xsta ��ʼX����
 * @param ysta ��ʼY����
 * @param xend ����X����
 * @param yend ����Y����
 * @param color ���ε���ɫ
 */
void LCD_Draw_ArcRect(unsigned int xsta, unsigned int ysta, unsigned int xend, unsigned int yend, unsigned int color);

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
void LCD_Show_Chinese(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);

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
void LCD_Show_Chinese12x12(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);

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
void LCD_Show_Chinese16x16(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);

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
void LCD_Show_Chinese24x24(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);

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
void LCD_Show_Chinese32x32(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);

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
void LCD_Show_Char(unsigned int x, unsigned int y, unsigned char num, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);

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
void LCD_Show_String(unsigned int x, unsigned int y, const unsigned char *p, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);

/**
 * @brief ����
 * @param m ����
 * @param n ָ��
 * @return m��n����
 */
unsigned int mypow(unsigned char m, unsigned char n);

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
void LCD_Show_IntNum(unsigned int x, unsigned int y, unsigned int num, unsigned char len, unsigned int fc, unsigned int bc, unsigned char sizey);

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
void LCD_Show_FloatNum(unsigned int x, unsigned int y, float num, unsigned char len, unsigned int fc, unsigned int bc, unsigned char sizey);

/**
 * @brief ��ʾͼƬ
 * @param x ͼƬ��ʾ��X����
 * @param y ͼƬ��ʾ��Y����
 * @param length ͼƬ�ĳ���
 * @param width ͼƬ�Ŀ��
 * @param pic ͼƬ����ָ��
 */
void LCD_ShowPicture(unsigned int x, unsigned int y, unsigned int length, unsigned int width, const unsigned char pic[]);

#endif
