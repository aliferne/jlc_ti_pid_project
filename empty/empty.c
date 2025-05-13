#include "ti_msp_dl_config.h"
#include "stdio.h"

#define delay_ms(X) delay_cycles((CPUCLK_FREQ / 1000) * (X))

void uart0_send_string(char *str); // 串口发送数据
volatile bool gCheckADC;           // ADC采集成功标志位
unsigned int adc_getValue(void);   // 读取ADC的数据

int main(void)
{
    char show_buff[100]    = {0};
    unsigned int adc_value = 0;
    float voltage_value    = 0;

    SYSCFG_DL_init();

    // 开启ADC中断
    NVIC_EnableIRQ(ADC_VOLTAGE_INST_INT_IRQN);

    uart0_send_string("adc Demo start\r\n");
    while (1) {
        // 获取ADC数据
        adc_value = adc_getValue();
        sprintf(show_buff, "adc value:%d\r\n", adc_value);
        uart0_send_string(show_buff);

        // 将ADC采集的数据换算为电压
        voltage_value = adc_value / 4095.0 * 3.3;

        sprintf(show_buff, "voltage value:%.2f\r\n", voltage_value);
        uart0_send_string(show_buff);
        delay_ms(1000);
    }
}

// 串口发送字符串
void uart0_send_string(char *str)
{
    // 当前字符串地址不在结尾 并且 字符串首地址不为空
    while (*str != 0 && str != 0) {
        // 发送字符串首地址中的字符，并且在发送完成之后首地址自增
        DL_UART_transmitDataBlocking(UART_0_INST, *str++);
    }
}

// 读取ADC的数据
unsigned int adc_getValue(void)
{
    unsigned int gAdcResult = 0;

    // 软件触发ADC开始转换
    DL_ADC12_startConversion(ADC_VOLTAGE_INST);
    // 如果当前状态为正在转换中则等待转换结束
    while (false == gCheckADC) {
        __WFE();
    }
    // 获取数据
    gAdcResult = DL_ADC12_getMemResult(ADC_VOLTAGE_INST, ADC_VOLTAGE_ADCMEM_ADC_CH0);

    // 清除标志位
    gCheckADC = false;

    return gAdcResult;
}

// ADC中断服务函数
void ADC_VOLTAGE_INST_IRQHandler(void)
{
    // 查询并清除ADC中断
    switch (DL_ADC12_getPendingInterrupt(ADC_VOLTAGE_INST)) {
        // 检查是否完成数据采集
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            gCheckADC = true; // 将标志位置1
            break;
        default:
            break;
    }
}