#include "ti_msp_dl_config.h"

// typedef void *CallbackFn();

uint8_t flag100ms = 0;

int main()
{
    SYSCFG_DL_init();

	DL_GPIO_setPins(LED1_PORT, LED1_PIN_22_PIN);
    while (1) {
		if (flag100ms) {
			flag100ms = 0;
			DL_GPIO_clearPins(LED1_PORT, LED1_PIN_22_PIN);
			DL_SYSTICK_disable();
		}
        // if (flag100ms) {
        //     flag100ms = 0;
        //     DL_GPIO_togglePins(LED1_PORT, LED1_PIN_22_PIN);
        // }
    }
}

void SysTick_Handler()
{
    // delay for 10ms
    static uint32_t counter = 0;
	
    if (counter < 1000) {
        counter++;
    } else {
        counter   = 0;
        flag100ms = 1;
    }
}
