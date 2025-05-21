#include "mid_debug_uart.h"
#include "string.h"

DEBUG_UART_STRUCT debug_uart;

void debug_uart_init(void)
{
    // 清除中断位
    NVIC_ClearPendingIRQ(UART_DEBUG_INST_INT_IRQN);
    // 中断使能
    NVIC_EnableIRQ(UART_DEBUG_INST_INT_IRQN);
}

void debug_uart_send_char(char ch)
{
    // 等待串口空闲
    while (DL_UART_isBusy(UART_DEBUG_INST));
    // 发送单个字节
    DL_UART_Main_transmitData(UART_DEBUG_INST, ch);
}

void debug_uart_send_string(char *str)
{
    while (*str) {
        debug_uart_send_char(*str++);
    }
}

char *get_debug_uart_receive_data(void)
{
    return debug_uart.receive_buffer;
}

void clear_debug_uart_receive_data(void)
{
    int i = 0;
    // 长度清零
    debug_uart.receive_data_length = 0;
    for (i = 0; i < RECEIVE_BUFFER_MAX; i++) {
        // 字节清零
        debug_uart.receive_buffer[i] = 0;
    }
}

static int debug_uart_receive_limit_length()
{
    // 超出最大长度时返回错误码 0
    if (debug_uart.receive_data_length >= RECEIVE_BUFFER_MAX) {
        debug_uart.receive_data_length = 0;
        return 0;
    }
    return 1;
}

// 串口中断函数
void UART_DEBUG_INST_IRQHandler()
{
    if (DL_UART_getPendingInterrupt(UART_DEBUG_INST) == DL_UART_IIDX_RX) {
        // 接收字节并写入结构体的buffer中
        debug_uart.receive_buffer[debug_uart.receive_data_length++] = DL_UART_Main_receiveData(UART_DEBUG_INST);
        // 确保不会超出最大长度， NOTE: 不过这里没处理返回值
        debug_uart_receive_limit_length();
    }
}
