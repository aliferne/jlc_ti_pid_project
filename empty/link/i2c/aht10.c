#include "aht10.h"
#include "bsp_i2c.h"

#define delay_us(X) delay_cycles((CPUCLK_FREQ / 1000000) * (X))

/// @brief Initialize AHT10
static void AHT10_Init()
{

    delay_us(50000);
    I2C_Start();

    I2C_Send_Byte(WRITEMODE);
    I2C_Send_Byte(INIT_CMD);
    I2C_Send_Byte(0x08);
    I2C_Send_Byte(0x00);

    delay_us(50000);
    I2C_Stop();
}

/// @brief Initialize AHT10 for reading temperature and humidity
static void AHT10_Init_Read_Mode()
{
    I2C_Start();

    I2C_Send_Byte(WRITEMODE);
    I2C_Wait_Ack();
    // See Datasheet for details
    I2C_Send_Byte(READ_TEMP);
    I2C_Wait_Ack();
    // actually PARA0 and PARA1 are useless
    I2C_Send_Byte(PARA0);
    I2C_Wait_Ack();
    I2C_Send_Byte(PARA1);
    I2C_Wait_Ack();

    I2C_Stop();
}

/// @brief Write data into recvBuffer
/// @param recvBuffer the buffer to store the data
static void AHT10_Write_recvBuffer(uint8_t *recvBuffer)
{
    uint8_t i = 0;

    // Waiting for measuring and keep asking if finished
    do {
        delay_us(8000); // Waiting for sensor
        I2C_Start();
        I2C_Send_Byte(READMODE);
    } while (I2C_Wait_Ack()); // Keep query till not busy

    for (i = 0; i < DATA_BYTES; i++) {
        recvBuffer[i] = I2C_Read_Byte();
        // When last time read, send NACK to end this session
        I2C_Send_Ack((i == (DATA_BYTES - 1)) ? 1 : 0);
    }
    I2C_Stop(); // End this session
}

void AHT10_readTemperature()
{
    uint8_t status;
    // Store data here
    uint8_t recvBuffer[DATA_BYTES] = {0x00};

    AHT10_Init();
    AHT10_Init_Read_Mode();             // Initialize read mode
    AHT10_Write_recvBuffer(recvBuffer); // Write into recvBuffer

    uint32_t humidity    = ((recvBuffer[0] << 12) | (recvBuffer[1] << 4) | (recvBuffer[2] >> 4));
    uint32_t temperature = (((recvBuffer[2] & 0x0F) << 16) | (recvBuffer[3] << 8) | (recvBuffer[4]));
    printf("\r\n");
    printf("--> %x, %x, %x\n", recvBuffer[0], recvBuffer[1], recvBuffer[2]);
    // FIXME: 温度不显示
    printf("--> %x, %x, %x\n", recvBuffer[2], recvBuffer[3], recvBuffer[4]);
    printf("-> HUMI: %d\r\n", (uint32_t)(humidity * 100.0 / BASE));
    printf("-> TEMP: %d\r\n", (int)(temperature * 200.0 / BASE - 50.0));

    AHT10_Soft_Reset();
}

void AHT10_Soft_Reset()
{
    I2C_Start();
    I2C_Send_Byte(AHT10_ADDR << 1 | WRITE);
    I2C_Wait_Ack();
    I2C_Send_Byte(SOFT_REBOOT);
    I2C_Wait_Ack();
    I2C_Stop();
}
