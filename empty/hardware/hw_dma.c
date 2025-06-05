#include "hw_dma.h"
#include "mid_debug_led.h"

/**
 * @brief 使用DMA方式向LCD发送单字节数据
 * 
 * 该函数配置DMA通道，将单个字节数据从内存传输到SPI的TXDATA寄存器，
 * 实现非阻塞方式的LCD数据传输。
 * 
 * @param data 要发送到LCD的8位数据
 * 
 * @note 
 * - 使用DMA_LCD_TX_CHAN_ID通道进行数据传输
 * - 传输完成后DMA通道会自动禁用
 * - 该函数为非阻塞函数，调用后立即返回
 * - 需要确保DMA通道已正确初始化
 * 
 * @warning 
 * - 传入的data参数为局部变量，DMA传输可能在函数返回后进行，
 *   可能导致数据不一致问题
 * - 建议传入全局变量或静态变量的地址
 * 
 * @see DL_DMA_setSrcAddr()
 * @see DL_DMA_setDestAddr() 
 * @see DL_DMA_setTransferSize()
 * @see DL_DMA_enableChannel()
 * 
 * @example
 * @code
 * uint8_t lcd_data = 0x55;
 * dma_transmit_lcd_data(lcd_data);
 * @endcode
 */
void dma_transmit_lcd_data(uint8_t data)
{
    // 设置搬运起始地址
    DL_DMA_setSrcAddr(DMA, DMA_LCD_TX_CHAN_ID, (uint32_t)&data);
    // 设置搬运目的地址
    DL_DMA_setDestAddr(DMA, DMA_LCD_TX_CHAN_ID, (uint32_t)(&SPI_LCD_INST->TXDATA));
    // 设置传输数据的长度
    DL_DMA_setTransferSize(DMA, DMA_LCD_TX_CHAN_ID, 1);
    // 开启DMA通道
    DL_DMA_enableChannel(DMA, DMA_LCD_TX_CHAN_ID);
}
