#include "mid_timer.h"
#include "hw_encoder.h"
#include "mid_button.h"

void timer_init(void)
{
    //定时器中断
	NVIC_ClearPendingIRQ(TIMER_TICK_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_TICK_INST_INT_IRQN);
}

//电机编码器脉冲计数
void TIMER_TICK_INST_IRQHandler(void)
{
	//20ms归零中断触发
	if( DL_TimerA_getPendingInterrupt(TIMER_TICK_INST) == DL_TIMER_IIDX_ZERO )
	{
		//编码器更新
		encoder_update();

		//按键扫描+事件管理
		flex_button_scan();

	}
}