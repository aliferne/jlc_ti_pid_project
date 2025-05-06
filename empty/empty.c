#include "ti_msp_dl_config.h"

// 使用dl库内置的 `delay_cycles` 函数来延时
#define delay_ms(X) delay_cycles((CPUCLK_FREQ / 1000) * (X));

volatile uint32_t delay_times = 0; // 延时次数
volatile uint8_t uart_data    = 0; // uart数据

void Uart0_Transmit_Char(char ch);      // 发送字符
void Uart0_Transmit_String(char *data); // 发送字符串
void Uart0_Receive_String(void);

int main()
{
    SYSCFG_DL_init();
    // 清除串口中断标志
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    // 串口中断使能
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);

    while (1) {
        // LED引脚高电平
        DL_GPIO_setPins(LED_PORT, LED_PIN_22_PIN); // 点亮LED
        delay_ms(500);
        // LED引脚低电平
        DL_GPIO_clearPins(LED_PORT, LED_PIN_22_PIN); // 点亮LED
        delay_ms(500);
    }
}

void Uart0_Transmit_Char(char ch)
{
    // 等待串口空闲
    while (DL_UART_isBusy(UART_0_INST) == true);
    DL_UART_Main_transmitData(UART_0_INST, ch); // 发送数据
}

void Uart0_Transmit_String(char *data)
{
    // 字符串为空时不发送
    while (*data) {
        Uart0_Transmit_Char(*data++); // 发送数据
    }
}

void UART_0_INST_IRQHandler()
{
    switch (DL_UART_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_IIDX_RX:
            // 将发送过来的数据保存在变量中
            uart_data = DL_UART_Main_receiveData(UART_0_INST);
            // 然后再发出去
            Uart0_Transmit_Char(uart_data);
            break;
        default: // 其他串口中断不管
            break;
    }
}
