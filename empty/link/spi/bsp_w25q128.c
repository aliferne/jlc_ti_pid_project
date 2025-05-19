#include "bsp_w25q128.h"

uint8_t spi_read_write_byte(uint8_t content)
{
    uint8_t data = 0;

    // 发送数据
    DL_SPI_transmitData8(SPI_INST, content);
    // 等待SPI总线空闲
    while (DL_SPI_isBusy(SPI_INST));
    // 接收数据
    data = DL_SPI_receiveData8(SPI_INST);
    // 等待SPI总线空闲
    while (DL_SPI_isBusy(SPI_INST));

    return data;
}

// 读取芯片ID
// 返回值如下:
// 0XEF13,表示芯片型号为W25Q80
// 0XEF14,表示芯片型号为W25Q16
// 0XEF15,表示芯片型号为W25Q32
// 0XEF16,表示芯片型号为W25Q64
// 0XEF17,表示芯片型号为W25Q128
// 读取设备ID
uint16_t W25Q128_readID()
{
    uint16_t temp = 0;
    // 将CS端拉低为低电平
    SPI_CS(0);
    // 发送指令90h
    spi_read_write_byte(0x90); // 发送读取ID命令
    // 发送地址  000000H
    spi_read_write_byte(0x00);
    spi_read_write_byte(0x00);
    spi_read_write_byte(0x00);

    // 接收数据
    // 接收制造商ID
    temp |= spi_read_write_byte(0xFF) << 8;
    // 接收设备ID
    temp |= spi_read_write_byte(0xFF);
    // 恢复CS端为高电平
    SPI_CS(1);
    // 返回ID
    return temp;
}

void W25Q128_write_enable()
{
    SPI_CS(0); // 拉低CS
    // 写入0x06
    spi_read_write_byte(0x06);
    SPI_CS(1); // 拉高CS
}

void W25Q128_wait_busy()
{
    uint8_t byte = 0;
    do {
        SPI_CS(0);                 // 拉低CS
        spi_read_write_byte(0x05); // 发送指令0x05
        // 接收状态寄存器值
        byte = spi_read_write_byte(0xFF);
        SPI_CS(1); // 拉高CS
        // 判断BUSY位是否为1 如果为1说明在忙，重新读写BUSY位直到为0
    } while (byte & 0x01);
}

void W25Q128_erase_sector(uint32_t addr)
{
    // 计算扇区号，一个扇区4KB=4096
    addr *= 4096;

    W25Q128_write_enable(); // 写使能
    W25Q128_wait_busy();    // 判断忙，如果忙则一直等

    SPI_CS(0);

    spi_read_write_byte(0x20); // 发送指令0x20
                               // 发送24位扇区地址的高8位
    spi_read_write_byte((uint8_t)((addr) >> 16));
    // 发送24位扇区地址的中8位
    spi_read_write_byte((uint8_t)((addr) >> 8));
    // 发送24位扇区地址的低8位
    spi_read_write_byte((uint8_t)addr);

    SPI_CS(1);
    // 等待擦除完成
    W25Q128_wait_busy();
}

void W25Q128_write(char *buffer, uint32_t write_addr, uint16_t numbyte)
{
    uint32_t i = 0;
    // 擦除扇区
    W25Q128_erase_sector(write_addr / 4096);
    // 写使能
    W25Q128_write_enable();
    // 等待擦除完成
    W25Q128_wait_busy();
    // 拉低CS
    SPI_CS(0);
    // 发送指令0x02
    spi_read_write_byte(0x02);
    // 发送24位扇区地址的高8位
    spi_read_write_byte((uint8_t)((write_addr) >> 16));
    // 发送24位扇区地址的中8位
    spi_read_write_byte((uint8_t)((write_addr) >> 8));
    // 发送24位扇区地址的低8位
    spi_read_write_byte((uint8_t)write_addr);
    // 发送数据
    for (i = 0; i < numbyte; i++) {
        spi_read_write_byte(buffer[i]);
    }
    // 拉高CS
    SPI_CS(1);
    // 等待写入完成
    W25Q128_wait_busy();
}

void W25Q128_read(uint8_t *buffer, uint32_t read_addr, uint16_t read_length)
{
    uint16_t i;
    // 拉低CS端为低电平
    SPI_CS(0);
    // 发送指令03h
    spi_read_write_byte(0x03);
    // 发送24位读取数据地址的高8位
    spi_read_write_byte((uint8_t)((read_addr) >> 16));
    // 发送24位读取数据地址的中8位
    spi_read_write_byte((uint8_t)((read_addr) >> 8));
    // 发送24位读取数据地址的低8位
    spi_read_write_byte((uint8_t)read_addr);
    // 根据读取长度读取出地址保存到buffer中
    for (i = 0; i < read_length; i++) {
        buffer[i] = spi_read_write_byte(0XFF);
    }
    // 恢复CS端为高电平
    SPI_CS(1);
}
