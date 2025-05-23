#include "hw_encoder.h"


static ENCODER_RES motor_encoder;

//编码器初始化
void encoder_init(void)
{
	//编码器引脚外部中断
	NVIC_ClearPendingIRQ(GPIOB_INT_IRQn);
	NVIC_EnableIRQ(GPIOB_INT_IRQn);
}

//获取编码器的值
int get_encoder_count(void)
{
    
	return motor_encoder.count;
}
//获取编码器的方向
ENCODER_DIR get_encoder_dir(void)
{
	return motor_encoder.dir;
}

//编码器数据更新
//请间隔一定时间更新
void encoder_update(void)
{
	motor_encoder.count = motor_encoder.temp_count;

	//确定方向
	motor_encoder.dir = ( motor_encoder.count >= 0 ) ? FORWARD : REVERSAL;

	motor_encoder.temp_count = 0;//编码器计数值清零
}

//外部中断处理函数
void GROUP1_IRQHandler(void)
{
	uint32_t gpio_status;

	//获取中断信号情况
	gpio_status = DL_GPIO_getEnabledInterruptStatus(GPIO_ENCODER_PORT, GPIO_ENCODER_PIN_A_PIN | GPIO_ENCODER_PIN_B_PIN);
	//编码器A相上升沿触发
	if((gpio_status & GPIO_ENCODER_PIN_A_PIN) == GPIO_ENCODER_PIN_A_PIN)
	{
		//如果在A相上升沿下，B相为低电平
		if(!DL_GPIO_readPins(GPIO_ENCODER_PORT,GPIO_ENCODER_PIN_B_PIN))
		{
			motor_encoder.temp_count--;
		}
		else
		{
			motor_encoder.temp_count++;
		}
	}//编码器B相上升沿触发
	else if((gpio_status & GPIO_ENCODER_PIN_B_PIN)==GPIO_ENCODER_PIN_B_PIN)
	{
		//如果在B相上升沿下，A相为低电平
		if(!DL_GPIO_readPins(GPIO_ENCODER_PORT,GPIO_ENCODER_PIN_A_PIN))
		{
			motor_encoder.temp_count++;
		}
		else
		{
			motor_encoder.temp_count--;
		}
	}
	//清除状态
	DL_GPIO_clearInterruptStatus(GPIO_ENCODER_PORT,GPIO_ENCODER_PIN_A_PIN|GPIO_ENCODER_PIN_B_PIN);
}