/**
 * @file mid_debug_uart.h
 * @brief 调试串口中间件头文件
 * @details 提供调试串口的初始化、数据发送和接收功能，支持字符和字符串的收发操作
 * @details 也可以作为一般串口的使用，此时函数的命名将不会有`debug`字样
 * @version 1.0
 * @date 2025/5/31
 * @author Ferne
 */

#ifndef _MID_DEBUG_UART_H_
#define _MID_DEBUG_UART_H_

#include "ti_msp_dl_config.h"

/**
 * @brief 接收缓冲区最大长度
 * @details 定义串口接收缓冲区的最大容量，实际可存储字符数为126个（需要预留结束符位置）
 */
#define RECEIVE_BUFFER_MAX 127

/**
 * @brief 调试串口结构体
 * @details 用于管理调试串口的接收数据缓冲区和数据长度信息
 */
typedef struct {
    char recv_buffer[RECEIVE_BUFFER_MAX]; /**< 接收数据缓冲区，用于存储接收到的字符数据 */
    int recv_data_length;                 /**< 接收数据的实际长度 */
} UART_STRUCT;

typedef struct {
    void (*init)(void);              /**< 初始化函数指针 */
    void (*send_char)(char ch);      /**< 发送单个字符函数指针 */
    void (*send_string)(char *str);  /**< 发送字符串函数指针 */
    char *(*get_recv_data)(void);    /**< 获取接收数据函数指针 */
    void (*clear_recv_data)(void);   /**< 清除接收数据函数指针 */
    bool (*limit_recv_length)(void); /**< 限制接收长度函数指针 */
} UART_INTERFACE;

typedef enum {
    UART_DEBUG = 0,
    UART_CMD,
    UART_NUM,
} UART_INSTANCE;

typedef struct {
    const char *name;        /**< 串口名称 */
    IRQn_Type irq_num;       /**< 中断号 */
    UART_Regs *uart_reg;     /**< 串口地址 */
    UART_STRUCT *uart_struct; /**< 串口结构体 */
} UART_CONFIG;

#ifndef MID_UART_H
#define MID_UART_H

#include <stdint.h>
#include <stdbool.h>

// ================================
// 公共接口函数声明
// ================================

/**
 * @brief 初始化UART接口
 */
void uart_interface_init(void);

/**
 * @brief 获取UART接口指针
 * @param uart_id UART ID
 * @return UART接口指针，失败返回NULL
 */
UART_INTERFACE *get_uart_interface(uint8_t uart_id);

// ================================
// Debug UART接口函数声明
// ================================

/**
 * @brief 初始化调试串口
 */
void debug_uart_init(void);

/**
 * @brief 通过调试串口发送单个字符
 * @param ch 要发送的字符
 */
void debug_uart_send_char(char ch);

/**
 * @brief 通过调试串口发送字符串
 * @param str 要发送的字符串
 */
void debug_uart_send_string(char *str);

/**
 * @brief 获取调试串口接收到的数据
 * @return 指向接收缓冲区的指针
 */
char *debug_uart_get_recv_data(void);

/**
 * @brief 清除调试串口接收到的数据
 */
void debug_uart_clear_recv_data(void);

bool debug_uart_limit_recv_length(void);

// ================================
// CMD UART接口函数声明
// ================================

/**
 * @brief 初始化命令串口
 */
void cmd_uart_init(void);

/**
 * @brief 通过命令串口发送单个字符
 * @param ch 要发送的字符
 */
void cmd_uart_send_char(char ch);

/**
 * @brief 通过命令串口发送字符串
 * @param str 要发送的字符串
 */
void cmd_uart_send_string(char *str);

/**
 * @brief 获取命令串口接收到的数据
 * @return 指向接收缓冲区的指针
 */
char *cmd_uart_get_recv_data(void);

/**
 * @brief 清除命令串口接收到的数据
 */
void cmd_uart_clear_recv_data(void);

bool cmd_uart_limit_recv_length(void);


#endif /* MID_UART_H */

#endif /* _MID_DEBUG_UART_H_ */
