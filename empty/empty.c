#include "ti_msp_dl_config.h"
#include "./middle/mid_debug_led.h"
#include "./middle/mid_debug_uart.h"
#include "string.h"
#include "./hardware/hw_lcd.h"
#include "./middle/mid_button.h"
#include "./app/app_key_task.h"

void ui_home_page(void);// 首页页面初始化
void ui_home_page_select(int mode);//绘制选择框

int main(void)
{
		int sys_time = 0;
	
    SYSCFG_DL_init();
		
		//DEBUG串口初始化
		debug_uart_init();
		
		//按键任务初始化
    user_button_init();
		
		//LCD初始化
		lcd_init();
		
		//LCD显示UI
		ui_home_page();
	
    while (1) 
		{
			sys_time++;

			if( sys_time % 10 == 0 )//2*10=20ms进行一次按键扫描
			{
					//按键扫描
					flex_button_scan();
			}
			
			if( sys_time % 50 == 0 )//2*50=100ms刷新一次屏幕
			{
					//屏幕刷新
					ui_home_page_select( get_app_key_current_mode() );
			}
			
			delay_cycles(CPUCLK_FREQ / 1000 * 2);//时间基准2ms
    }
}


/*
功能说明：显示居中字符串函数用于显示居中的文字，计算文字的居中坐标
参数说明：  x=屏幕中心x坐标
        w=屏幕宽度
        y=屏幕中心y坐标
        h=屏幕高度
        str_len=字符串长度
        sizey=字体大小
        *str=需要显示的字符串
        color背景颜色        
备注：GRAYBLUE 浅蓝色
      DARKBLUE 深蓝色
*/
void disp_x_center(int y, int str_len, uint16_t bc, unsigned char sizey, unsigned char* str)
{
    int str_center_x = (sizey * str_len) / 2;//字符串x=字体大小*字符串长度/2
    int str_center_y = sizey / 2;//字符串y=字体大小/2

    //显示居中坐标的文字    
	  LCD_ArcRect(screen_center_x - str_center_x - 10, y, screen_center_x + str_center_x + 10, sizey+y, bc);
    LCD_ShowChinese(screen_center_x - str_center_x,y,str,WHITE,bc,sizey,1);
}

/*
功能说明：显示字符串矩形函数用于显示矩形的文字，计算文字的居中坐标
参数说明：  x=矩形起始x坐标
        w=矩形宽度
        y=矩形起始y坐标
        h=矩形高度
        str_len=字符串长度
        sizey=字体大小
        *str=需要显示的字符串
        color背景颜色        
备注：GRAYBLUE 浅蓝色
      DARKBLUE 深蓝色
*/
void disp_string_rect(int x, int w, int y, int h, int str_len, int sizey, unsigned char* str, int color)
{
    int str_center_x = (sizey * str_len) / 2; //字符串x = 字体大小*字符串长度/2
    int rect_center_x = x + (w / 2); //矩形中心x
    int str_center_y = sizey  / 2; //字符串y=字体大小/2
    int rect_center_y = y + (h / 2); //矩形中心y

    //显示背景矩形    
	  LCD_ArcRect(x, y, x + w, y + h, color);
    //显示字符串
    LCD_ShowChinese(rect_center_x - str_center_x, rect_center_y - str_center_y,str,WHITE,color,sizey,1);
}

/*
功能说明：显示选择框函数用于显示选择框：  x=按钮起始X坐标
        w=显示的按钮选择框宽度
        y=按钮起始Y坐标
        h=显示的按钮选择框高度
        line_length=选择框边线长度
        interval=选择框边线间隔 按钮选择框边框之间的距离
        color=选择框边线颜色
*/
void disp_select_box(int x, int w, int y, int h, int line_length, int interval, int color)
{
    //计算按钮选择框边框之间的距离+边线
    x = x - interval;
    w = w + (interval + interval);
    y = y - interval;
    h = h + (interval + interval);
    //左上角    
	  LCD_DrawLine(x, y, x + line_length, y, color);
    LCD_DrawLine(x, y, x, y + line_length, color);
    //右上角    
	  LCD_DrawLine(x + w, y, x + w - line_length, y, color);
    LCD_DrawLine(x + w, y, x + w, y + line_length, color);
    //左下角    
	  LCD_DrawLine(x, y + h, x + line_length, y + h, color);
    LCD_DrawLine(x, y + h, x, y + h - line_length, color);
    //右下角    
	  LCD_DrawLine(x + w, y + h, x + w - line_length, y + h, color);
    LCD_DrawLine(x + w, y + h, x + w, y + h - line_length, color);
}

//家居页面初始化
void ui_home_page(void)
{
    //关闭背光    
	  LCD_BLK_Clr();

    //显示全屏背景颜色
    LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
    //显示标题
    disp_x_center(3,5,BLUE,16,(unsigned char *)"立创开发板");
    //显示副标题
    disp_x_center(3+16+3,8,BLUE,16,(unsigned char *)"简易PID入门套件");

    int x = 40;
    int x_offset = 80;
    int y = 65;
    int y_offset = 80;
    //显示第一个按钮：定速
    disp_string_rect(x, x_offset, y, y_offset, 2, 24, "定速", BLUE);
    int x2 = 200;
    int x2_offset = 80;
    int y2 = 65;
    int y2_offset = 80;
    //显示第二个按钮：定距  
		disp_string_rect(x2, x2_offset, y2, y2_offset, 2, 24, "定距", BLUE);
    
    //显示选择框  
		disp_select_box(40,80,65,80,10,5,WHITE);  
	
    //打开背光   
		LCD_BLK_Set();
}

//根据按键选择绘制首页两个选项的选择框
void ui_home_page_select(int mode)
{
    char select_box_seze = 5;
    switch(mode)
    {
        case 0: //选择PID定速模式
            disp_select_box(40,80,65,80,10,select_box_seze,WHITE);
            disp_select_box(200,80,65,80,10,select_box_seze,BLACK); 
            break;
        case 1: //选择PID定距模式
            disp_select_box(40,80,65,80,10,select_box_seze,BLACK);
            disp_select_box(200,80,65,80,10,select_box_seze,WHITE);
            break;
    }
}