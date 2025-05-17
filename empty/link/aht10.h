#include "bsp_i2c.h"

#ifndef __AHT10_H__
#define __AHT10_H__

// WRITE MODE
#define WRITE 0
// READ MODE
#define READ 1
// Addr of AHT10
#define AHT10_ADDR 0x70
// AHT10 WRITE MODE
#define WRITEMODE (AHT10_ADDR | WRITE)
// AHT10 READ MODE
#define READMODE (AHT10_ADDR | READ)

// 指示读取温度
#define READ_TEMP 0xAC // 首先发送以指示读取温度
#define PARA0     0x33 // 再发送以指示读取温度
#define PARA1     0x00 // 最后发送以指示读取温度
// 指示软复位
#define SOFT_REBOOT 0xBA
// AHT10总共会传入6个字节的数据
// 其中5个字节是湿度和温度
#define DATA_BYTES 6
// 2^20 的值为 1,048,576(1024 * 1024)
#define BASE 0x100000

/// @brief Start I2C
void I2C_Start();

/// @brief Stop I2C
void I2C_Stop();

/// @brief Send a bit signal(ACK/NACK) to I2C
/// @param ack the signal to Slaver, 1: NACK; 0: ACK
void I2C_Send_Ack(uint8_t ack);

/// @brief Wait for an ACK signal from I2C
/// @return ack the signal from Slaver, 1: NACK; 0: ACK
char I2C_Wait_Ack();

/// @brief Send a byte to I2C
/// @param byte the byte to send
void I2C_Send_Byte(uint8_t byte);

/// @brief Read a byte from I2C
/// @return the byte from I2C Slaver
uint8_t I2C_Read_Byte();

/// @brief Initialize AHT10 for reading temperature and humidity
static void AHT10_Init_Read_Mode();

/// @brief Read data from AHT10 and write into recvBuffer
/// @param recvBuffer the buffer to store the data
static void AHT10_Write_recvBuffer(uint8_t *recvBuffer);

/// @brief
void AHT10_readTemperature();

/// @brief Soft Reset
void AHT10_Soft_Reset();

#endif