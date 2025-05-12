#include "ti_msp_dl_config.h"

#define delay_ms(x) delay_cycles((CPUCLK_FREQ / 1000) * (x))

// FIXME: 跑不了，等会再看看

int main()
{
    int i =0;
    SYSCFG_DL_init();
    while (1)
    {
        for (i = 0; i <= 999; i++){
            DL_TimerG_setCaptureCompareValue(PWM_LED_INST,i,GPIO_PWM_LED_C1_IDX);
            delay_ms(1);
        }
        for (i = 999; i > 0; i--){
            DL_TimerG_setCaptureCompareValue(PWM_LED_INST,i,GPIO_PWM_LED_C1_IDX);
            delay_ms(1);
        }
    }
}