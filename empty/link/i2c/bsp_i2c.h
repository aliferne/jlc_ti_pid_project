#ifndef __BSP_I2C_H__
#define __BSP_I2C_H__

#include "../../ti_msp_dl_config.h"
#include "stdio.h"

// SDA and SCL output
#define SDA(x) ((x) ? DL_GPIO_setPins(I2C_PORT, I2C_SDA_PIN) : (DL_GPIO_clearPins(I2C_PORT, I2C_SDA_PIN)))
#define SCL(x) ((x) ? DL_GPIO_setPins(I2C_PORT, I2C_SCL_PIN) : (DL_GPIO_clearPins(I2C_PORT, I2C_SCL_PIN)))

/**
 * @brief Set SDA to output mode.
 */
void SDA_OUT(void);

/**
 * @brief Set SDA to input mode.
 */
void SDA_IN(void);

/**
 * @brief Get SDA status.
 * @return uint8_t SDA status (1 or 0).
 */
uint8_t SDA_GET(void);

/**
 * @brief Generate I2C start condition.
 */
void I2C_Start(void);

/**
 * @brief Generate I2C stop condition.
 */
void I2C_Stop(void);

/**
 * @brief Send ACK or NACK.
 * @param ack ACK (0) or NACK (1).
 */
void I2C_Send_Ack(uint8_t ack);

/**
 * @brief Wait for ACK from slave.
 * @return char 0 if ACK received, 1 if no ACK received.
 */
char I2C_Wait_Ack(void);

/**
 * @brief Send a byte via I2C.
 * @param byte The byte to be sent.
 */
void I2C_Send_Byte(uint8_t byte);

/**
 * @brief Read a byte from I2C.
 * @return uint8_t The received byte.
 */
uint8_t I2C_Read_Byte(void);

#endif
