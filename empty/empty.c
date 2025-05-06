#include "ti_msp_dl_config.h"

// X is the number you assigned for `delay_ms`
volatile uint8_t time_10ms = 10; // 100ms

void delay_ms(uint8_t ms)
{
    time_10ms = ms;
}

int main()
{
    SYSCFG_DL_init();
    // 设置开启按键引脚的GPIOB端口中断
    NVIC_EnableIRQ(BTN_INT_IRQN);

    while (1) {
        DL_GPIO_togglePins(LED2_PORT, LED2_PIN_3_PIN); // Set LED to ON
    }
}

void GROUP1_IRQHandler()
{
    // 读取Group1的中断寄存器，并清除中断标志位
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        // 检查是否为BTN的中断，注意是INT_IIDX
        case BTN_INT_IIDX:
            // 如果按键按下变为高电平
            if (DL_GPIO_readPins(BTN_PORT, BTN_PIN_21_PIN) > 0) {
                // 翻转板子上的LED
                DL_GPIO_togglePins(LED_PORT, LED_PIN_22_PIN);
            }
            break;

        default:
            break;
    }
}

void Btn_Detector()
{
    uint32_t onPressed = 0;
    onPressed          = DL_GPIO_readPins(BTN_PORT, BTN_PIN_21_PIN);
    if (onPressed == 0) {
        onPressed = DL_GPIO_readPins(BTN_PORT, BTN_PIN_21_PIN);
        if (onPressed == 0) {
            DL_GPIO_togglePins(LED_PORT, LED_PIN_22_PIN);
        }
    }
}

// void SysTick_Handler()
// {
//     if (time_10ms > 0) {
//         time_10ms--;
//     } else {
//         time_10ms = 0;
//     }
//     Btn_Detector();
// }
