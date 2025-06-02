#include "mid_uart.h"
#include "string.h"

/// @warning 使用前应当先清除数组内无用信息（xxx_uart_clear_recv_data）
static UART_STRUCT debug_uart; // 调试用串口
static UART_STRUCT cmd_uart;   // 给天问ASRPro使用的串口

// UART接口实例数组
static UART_INTERFACE uart_interfaces[UART_NUM];

static UART_CONFIG uart_configs[UART_NUM] = {
    // DEBUG UART
    [UART_DEBUG] = {
        .name        = "DEBUG UART",
        .uart_reg    = UART_DEBUG_INST,
        .irq_num     = UART_DEBUG_INST_INT_IRQN,
        .uart_struct = &debug_uart,
    },
    // CMD UART
    [UART_CMD] = {
        .name        = "CMD UART",
        .uart_reg    = UART_CMD_INST,
        .irq_num     = UART_CMD_INST_INT_IRQN,
        .uart_struct = &cmd_uart,
    }};

void uart_interface_init(void)
{
    // DEBUG UART
    uart_interfaces[UART_DEBUG].init              = debug_uart_init;
    uart_interfaces[UART_DEBUG].send_char         = debug_uart_send_char;
    uart_interfaces[UART_DEBUG].send_string       = debug_uart_send_string;
    uart_interfaces[UART_DEBUG].get_recv_data     = debug_uart_get_recv_data;
    uart_interfaces[UART_DEBUG].clear_recv_data   = debug_uart_clear_recv_data;
    uart_interfaces[UART_DEBUG].limit_recv_length = debug_uart_limit_recv_length;

    // CMD UART
    uart_interfaces[UART_CMD].init              = cmd_uart_init;
    uart_interfaces[UART_CMD].send_char         = cmd_uart_send_char;
    uart_interfaces[UART_CMD].send_string       = cmd_uart_send_string;
    uart_interfaces[UART_CMD].get_recv_data     = cmd_uart_get_recv_data;
    uart_interfaces[UART_CMD].clear_recv_data   = cmd_uart_clear_recv_data;
    uart_interfaces[UART_CMD].limit_recv_length = cmd_uart_limit_recv_length;

    // 初始化所有UART
    for (uint8_t i = 0; i < UART_NUM; i++) {
        uart_interfaces[i].init();
    }
}

UART_INTERFACE *get_uart_interface(uint8_t uart_id)
{
    if (uart_id >= UART_NUM) {
        return NULL; // 超出总数量则直接返回NULL
    }
    return &uart_interfaces[uart_id];
}

/**
 * @brief 初始化UART中断
 * @details 清除指定UART的中断挂起状态并启用中断
 *
 * @param uart_irqn UART中断号，用于标识特定的UART中断
 *
 * @note 这是一个静态内部函数，仅供模块内部使用
 * @note 函数会先清除可能存在的挂起中断，然后启用中断
 *
 * @see NVIC_ClearPendingIRQ()
 * @see NVIC_EnableIRQ()
 */
static void uart_init(IRQn_Type uart_irqn)
{
    NVIC_ClearPendingIRQ(uart_irqn);
    NVIC_EnableIRQ(uart_irqn);
}

/**
 * @brief 发送单个字符
 * @details 通过指定的UART寄存器发送一个字符，发送前会等待UART空闲
 *
 * @param uart_reg UART寄存器基址，指定要使用的UART外设
 * @param ch 要发送的字符
 *
 * @note 这是一个静态内部函数，仅供模块内部使用
 * @note 函数会阻塞等待直到UART不忙碌时才发送数据
 * @note 发送完成后函数立即返回，不等待传输完成
 *
 * @see DL_UART_isBusy()
 * @see DL_UART_Main_transmitData()
 */
static void uart_send_char(UART_Regs *uart_reg, char ch)
{
    // 检查指针有效性
    if (uart_reg == NULL) return;
    // 等待UART空闲
    while (DL_UART_isBusy(uart_reg) == true);
    // 发送数据
    DL_UART_Main_transmitData(uart_reg, ch);
}

/**
 * @brief 发送字符串
 * @details 通过指定的UART寄存器发送以空字符结尾的字符串
 *
 * @param uart_reg UART寄存器基址，指定要使用的UART外设
 * @param str 指向要发送的字符串的指针，必须以'\0'结尾
 *
 * @note 这是一个静态内部函数，仅供模块内部使用
 * @note 函数会逐个字符发送，直到遇到空字符或空指针
 * @note 函数内部调用_uart_send_char()来发送每个字符
 *
 * @warning 传入NULL指针是安全的，函数会检查指针有效性
 *
 * @see _uart_send_char()
 */
static void uart_send_string(UART_Regs *uart_reg, char *str)
{
    // 检查指针有效性
    if (str == NULL || uart_reg == NULL) return;
    // 逐个字符发送
    while (*str != '\0') {
        uart_send_char(uart_reg, *str++);
    }
}

/**
 * @brief 清空UART接收数据缓冲区
 * @details 重置UART结构体的接收数据长度并清空接收缓冲区
 *
 * @param uart_struct 指向UART结构体的指针
 *
 * @note 这是一个静态内部函数，仅供模块内部使用
 * @note 函数会将接收数据长度重置为0
 * @note 函数会将整个接收缓冲区的所有字节置为0
 * @note 循环变量使用uint8_t类型，限制在255以内，注释中已说明
 *
 * @warning 传入的uart_struct指针必须有效，函数不检查空指针
 *
 * @see UART_STRUCT
 * @see RECEIVE_BUFFER_MAX
 */
static void uart_clear_recv_data(UART_STRUCT *uart_struct)
{
    // 检查指针有效性
    if (uart_struct == NULL) return;
    // 长度归零
    uart_struct->recv_data_length = 0;
    // NOTE: 使用 memset 可能会导致一些异常
    for (uint8_t i = 0; i < RECEIVE_BUFFER_MAX; i++) {
        uart_struct->recv_buffer[i] = '\0';
    }
}

/**
 * @brief 检查UART接收数据长度限制
 * @details 检查接收数据长度是否超过缓冲区最大容量，如果超过则重置
 *
 * @param uart_struct 指向UART结构体的指针
 *
 * @return int 返回检查结果状态
 * @retval 1 接收数据长度未超过限制，可以继续接收
 * @retval 0 接收数据长度已达到或超过限制，已重置数据长度
 *
 * @note 这是一个静态内部函数，仅供模块内部使用
 * @note 当接收数据长度达到RECEIVE_BUFFER_MAX时，会自动重置为0
 * @note 该函数可用于防止缓冲区溢出
 *
 * @warning 传入的uart_struct指针必须有效，函数不检查空指针
 * @warning 函数修改了结构体内容，调用前请确保数据已被处理
 *
 * @see UART_STRUCT
 * @see RECEIVE_BUFFER_MAX
 */
static bool uart_limit_recv_length(UART_STRUCT *uart_struct)
{
    if (uart_struct == NULL) return false; // 防止空指针
    // 检查接收数据长度是否超过限制
    if (uart_struct->recv_data_length >= RECEIVE_BUFFER_MAX) {
        uart_struct->recv_data_length = 0;
        return true;
    }
    return false;
}

/* ***************************  debug_uart 部分 *************************** */

// 初始化调试串口
void debug_uart_init(void)
{
    uart_init(uart_configs[UART_DEBUG].irq_num);
}

// 发送单个字符
void debug_uart_send_char(char ch)
{
    uart_send_char(uart_configs[UART_DEBUG].uart_reg, ch);
}

// 发送字符串
void debug_uart_send_string(char *str)
{
    uart_send_string(uart_configs[UART_DEBUG].uart_reg, str);
}

// 获取调试串口接收到的数据
char *debug_uart_get_recv_data(void)
{
    return uart_configs[UART_DEBUG].uart_struct->recv_buffer;
}

// 清除调试串口接收到的数据
void debug_uart_clear_recv_data(void)
{
    uart_clear_recv_data(uart_configs[UART_DEBUG].uart_struct);
}

// 限制接收长度
bool debug_uart_limit_recv_length(void)
{
    return uart_limit_recv_length(uart_configs[UART_DEBUG].uart_struct);
}

/* ***************************  debug_uart 部分 *************************** */

/* ***************************  cmd_uart 部分 *************************** */

// 初始化调试串口
void cmd_uart_init(void)
{
    uart_init(uart_configs[UART_CMD].irq_num);
}

// 发送单个字符
void cmd_uart_send_char(char ch)
{
    uart_send_char(uart_configs[UART_CMD].uart_reg, ch);
}

// 发送字符串
void cmd_uart_send_string(char *str)
{
    uart_send_string(uart_configs[UART_CMD].uart_reg, str);
}

// 获取调试串口接收到的数据
char *cmd_uart_get_recv_data(void)
{
    return uart_configs[UART_CMD].uart_struct->recv_buffer;
}

// 清除调试串口接收到的数据
void cmd_uart_clear_recv_data(void)
{
    uart_clear_recv_data(uart_configs[UART_CMD].uart_struct);
}

// 限制接收长度
bool cmd_uart_limit_recv_length(void)
{
    return uart_limit_recv_length(uart_configs[UART_CMD].uart_struct);
}

/* ***************************  cmd_uart 部分 *************************** */

// 调试串口的中断服务函数
void UART_DEBUG_INST_IRQHandler(void)
{
    if (DL_UART_getPendingInterrupt(UART_DEBUG_INST) == DL_UART_IIDX_RX) {
        debug_uart.recv_buffer[debug_uart.recv_data_length++] = DL_UART_Main_receiveData(UART_DEBUG_INST);
        debug_uart_limit_recv_length();
    }
}

void UART_CMD_INST_IRQHandler(void)
{
    if (DL_UART_getPendingInterrupt(UART_CMD_INST) == DL_UART_IIDX_RX) {
        cmd_uart.recv_buffer[cmd_uart.recv_data_length++] = DL_UART_Main_receiveData(UART_CMD_INST);
        cmd_uart_limit_recv_length();
    }
}
