#include "ti_msp_dl_config.h"
#include "stdio.h"
#include "./link/aht10.h"

#define delay_ms(X) delay_cycles((CPUCLK_FREQ / 1000) * (X))

FILE __stdout;

// 定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}

// printf函数重定义
int fputc(int ch, FILE *stream)
{
    // 当串口0忙的时候等待，不忙的时候再发送传进来的字符
    while (DL_UART_isBusy(UART_0_INST) == true);

    DL_UART_Main_transmitData(UART_0_INST, ch);

    return ch;
}

int main(void)
{
    SYSCFG_DL_init();

    printf("AHT10 Start!!\r\n");
    while (1) {
        AHT10_readTemperature();
        printf("\n");
        // AHT10_Soft_Reset();
        // delay_ms(1000);
    }
}
