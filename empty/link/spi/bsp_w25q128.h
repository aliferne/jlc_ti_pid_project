#ifndef __BSP_W25Q128_H__
#define __BSP_W25Q128_H__

#include "../ti_msp_dl_config.h"

// CS引脚的输出控制
// x=0时输出低电平
// x=1时输出高电平
#define SPI_CS(x) ((x) ? DL_GPIO_setPins(CS_PORT, CS_PIN_PIN) : DL_GPIO_clearPins(CS_PORT, CS_PIN_PIN))

/**
 * @brief Read and write a byte via SPI.
 * @param content The byte to be sent.
 * @return uint8_t The received byte.
 */
uint8_t spi_read_write_byte(uint8_t content);

/**
 * @brief Read the chip ID of W25Q128.
 * @return uint16_t The chip ID.
 */
uint16_t W25Q128_readID(void);

/**
 * @brief Enable write operation for W25Q128.
 */
void W25Q128_write_enable(void);

/**
 * @brief Wait until W25Q128 is not busy.
 */
void W25Q128_wait_busy(void);

/**
 * @brief Erase a sector of W25Q128.
 * @param addr The sector address to be erased.
 */
void W25Q128_erase_sector(uint32_t addr);

/**
 * @brief Write data to W25Q128.
 * @param buffer The data buffer to be written.
 * @param write_addr The address to write data to.
 * @param numbyte The number of bytes to write.
 */
void W25Q128_write(char *buffer, uint32_t write_addr, uint16_t numbyte);

/**
 * @brief Read data from W25Q128.
 * @param buffer The buffer to store the read data.
 * @param read_addr The address to read data from.
 * @param read_length The number of bytes to read.
 */
void W25Q128_read(uint8_t *buffer, uint32_t read_addr, uint16_t read_length);

#endif
