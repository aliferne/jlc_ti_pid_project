#ifndef __HW_LCD_H
#define __HW_LCD_H

#include "ti_msp_dl_config.h"

#define LCD_W 320
#define LCD_H 170

#define screen_center_x (LCD_W/2)//屏幕中心X = 屏幕x/2
#define screen_center_y (LCD_H/2)//屏幕中心Y = 屏幕y/2

//-----------------LCD端口定义----------------

#define LCD_RES_Clr()  DL_GPIO_clearPins(GPIO_LCD_PORT,GPIO_LCD_PIN_RES_PIN)//RES
#define LCD_RES_Set()  DL_GPIO_setPins(GPIO_LCD_PORT,GPIO_LCD_PIN_RES_PIN)

#define LCD_DC_Clr()   DL_GPIO_clearPins(GPIO_LCD_PORT,GPIO_LCD_PIN_DC_PIN)//DC
#define LCD_DC_Set()   DL_GPIO_setPins(GPIO_LCD_PORT,GPIO_LCD_PIN_DC_PIN)

#define LCD_CS_Clr()   DL_GPIO_clearPins(GPIO_LCD_PORT,GPIO_LCD_PIN_CS_PIN)//CS
#define LCD_CS_Set()   DL_GPIO_setPins(GPIO_LCD_PORT,GPIO_LCD_PIN_CS_PIN)

#define LCD_BLK_Clr()  DL_GPIO_clearPins(GPIO_LCD_PORT,GPIO_LCD_PIN_BLK_PIN)//BLK
#define LCD_BLK_Set()  DL_GPIO_setPins(GPIO_LCD_PORT,GPIO_LCD_PIN_BLK_PIN)

void LCD_WR_DATA(unsigned int dat);
void lcd_init(void);
void LCD_Address_Set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color);//指定区域填充颜色
void LCD_DrawPoint(unsigned int x,unsigned int y,unsigned int color);//在指定位置画一个点
void LCD_DrawLine(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);//在指定位置画一条线
void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,unsigned int color);//在指定位置画一个矩形
void Draw_Circle(unsigned int x0,unsigned int y0,unsigned char r,unsigned int color);//在指定位置画一个圆

void LCD_ShowChinese(unsigned int x,unsigned int y,unsigned char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);//显示汉字串
void LCD_ShowChinese12x12(unsigned int x,unsigned int y,unsigned char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);//显示单个12x12汉字
void LCD_ShowChinese16x16(unsigned int x,unsigned int y,unsigned char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);//显示单个16x16汉字
void LCD_ShowChinese24x24(unsigned int x,unsigned int y,unsigned char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);//显示单个24x24汉字
void LCD_ShowChinese32x32(unsigned int x,unsigned int y,unsigned char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);//显示单个32x32汉字

void LCD_ShowChar(unsigned int x,unsigned int y,unsigned char num,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);//显示一个字符
void LCD_ShowString(unsigned int x,unsigned int y,const unsigned char *p,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);//显示字符串
unsigned int mypow(unsigned char m,unsigned char n);//求幂
void LCD_ShowIntNum(unsigned int x,unsigned int y,unsigned int num,unsigned char len,unsigned int fc,unsigned int bc,unsigned char sizey);//显示整数变量
void LCD_ShowFloatNum1(unsigned int x,unsigned int y,float num,unsigned char len,unsigned int fc,unsigned int bc,unsigned char sizey);//显示两位小数变量

void LCD_ShowPicture(unsigned int x,unsigned int y,unsigned int length,unsigned int width,const unsigned char pic[]);//显示图片

void LCD_ArcRect(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color);//画圆角矩形
void LCD_DrawVerrticalLine(int x, int y1, int y2, unsigned int color);
//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE           	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

#endif