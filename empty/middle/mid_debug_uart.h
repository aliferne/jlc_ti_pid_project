#ifndef __MID_DEBUG_UART_H__
#define __MID_DEBUG_UART_H__

#include "../ti_msp_dl_config.h"

#define RECEIVE_BUFFER_MAX 127

typedef struct {
    char receive_buffer[RECEIVE_BUFFER_MAX];
    int receive_data_length;
} DEBUG_UART_STRUCT;

/**
 * @brief 初始化调试UART
 *
 * 此函数用于初始化调试UART，清除UART的中断挂起标志，并使能UART的中断。
 */
void debug_uart_init(void);

/**
 * @brief 通过调试UART发送一个字符
 *
 * @param ch 要发送的字符
 *
 * 此函数通过调试UART发送一个字符，并在发送前等待UART变为非忙状态。
 */
void debug_uart_send_char(char ch);

/**
 * @brief 通过调试UART发送一个字符串
 *
 * @param str 要发送的字符串
 *
 * 此函数通过调试UART发送一个字符串，逐字符调用debug_uart_send_char函数。
 */
void debug_uart_send_string(char *str);

/**
 * @brief 获取调试UART接收到的数据
 *
 * @return 指向接收缓冲区的指针
 *
 * 此函数返回指向调试UART接收缓冲区的指针，从中可以读取接收到的数据。
 */
char *get_debug_uart_receive_data(void);

/**
 * @brief 清除调试UART接收到的数据
 *
 * 此函数清除调试UART的接收缓冲区和接收数据长度。
 */
void clear_debug_uart_receive_data(void);

/**
 * @brief 检查调试UART接收数据是否超出限制长度
 *
 * @return 如果接收数据长度未超出限制则返回1，否则返回0
 *
 * 此函数检查调试UART的接收数据长度是否已达到或超出接收缓冲区的最大长度，如果是，则重置接收数据长度。
 */
static int debug_uart_receive_limit_length(void);

/**
 * @brief UART调试实例中断处理函数
 *
 * 此函数是UART调试实例的中断处理函数，处理接收中断，并将接收到的数据存储到接收缓冲区。
 */
void UART_DEBUG_INST_INT_IRQN_Handler(void);

#endif