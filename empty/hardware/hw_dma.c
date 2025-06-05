#include "hw_dma.h"
#include "mid_debug_led.h"

// DMA传输完成标志位
static volatile bool gChannel0getInterrupted = false;
// 验证数据传输是否正确
static volatile bool gVerifyResult = false;

#define DMA_TRANSFER_SIZE_WORDS (16)
const uint32_t gSrcData[DMA_TRANSFER_SIZE_WORDS] = {
    0x00000000, 0x10101010,
    0x20202020, 0x30303030, 0x40404040, 0x50505050, 0x60606060, 0x70707070,
    0x80808080, 0x90909090, 0xA0A0A0A0, 0xB0B0B0B0, 0xC0C0C0C0, 0xD0D0D0D0,
    0xE0E0E0E0, 0xF0F0F0F0};

// FIXME: 此处行为不太正常，甚至无法初始化
void dma_init()
{
    // 使能DMA中断
    NVIC_EnableIRQ(DMA_INT_IRQn);
    // 设置搬运起始地址 TODO:
    // FUCK IT'LL BE A VERY HARD TASK
    DL_DMA_setSrcAddr(DMA, DMA_LCD_TX_CHAN_ID, DL_DMA_getSrcAddr(DMA, DMA_LCD_TX_CHAN_ID));
    // 设置搬运目的地址
    DL_DMA_setDestAddr(DMA, DMA_LCD_TX_CHAN_ID, DL_DMA_getDestAddr(DMA, DMA_LCD_TX_CHAN_ID));
    // 开启DMA通道
    DL_DMA_enableChannel(DMA, DMA_LCD_TX_CHAN_ID);
    // 设置传输数据大小 // TODO: ? 为啥不直接用上面那个宏
    DL_DMA_setTransferSize(DMA, DMA_LCD_TX_CHAN_ID, sizeof(gSrcData) / sizeof(uint32_t));
    set_debug_led_on();
}

void dma_start_transfer()
{
    gChannel0getInterrupted = false;               // 清除传输完成标志位
    DL_DMA_startTransfer(DMA, DMA_LCD_TX_CHAN_ID); // 启动DMA传输
    while (!gChannel0getInterrupted);              // 等待传输完毕
}

void dma_verify_data()
{
    gVerifyResult = true;
    for (int i = 0; i < DMA_TRANSFER_SIZE_WORDS; i++) {
        if (gSrcData[i] != *((uint32_t *)DL_DMA_getDestAddr(DMA, DMA_LCD_TX_CHAN_ID) + i)) {
            gVerifyResult = false;
            break;
        }
    }
    set_debug_led_on();
}

void SPI_LCD_INST_IRQHandler()
{
    switch (DL_DMA_getPendingInterrupt(DMA)) {
        case DMA_LCD_TX_CHAN_ID:
            gChannel0getInterrupted = true;
            break;
        default:
            break;
    }
}