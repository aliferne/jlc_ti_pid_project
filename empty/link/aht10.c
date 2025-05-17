#include "aht10.h"
#include "bsp_i2c.h"

#define delay_us(X) delay_cycles((CPUCLK_FREQ / 1000000) * (X))

void I2C_Start()
{
    SDA_OUT(); // 设置SDA为输出模式
    // 先确保都是高电平
    SCL(1);
    SDA(1);
    // 等待5us
    delay_us(5);
    // 在SCL=1时拉低SDA
    SDA(0);
    delay_us(5);
    // 开始传输数据
    SCL(0);
    delay_us(5);
}

void I2C_Stop()
{
    SDA_OUT();
    SCL(0);
    SDA(0);
    // 这里可以加入 delay_us(5);
    // 以保证过了 5us
    // 不过你要是确定两个机器周期能满足5us的话也行
    SCL(1);
    delay_us(5);
    SDA(1);
    delay_us(5);
}

void I2C_Send_Ack(uint8_t ack)
{
    SDA_OUT();
    SCL(0);
    SDA(0);
    delay_us(5);
    if (!ack)
        SDA(0);
    else
        SDA(1);
    SCL(1);
    delay_us(5);
    SCL(0);
    SDA(0);
}

char I2C_Wait_Ack()
{
    char ack = 0; // ack接收位
    // ack在第9个字节
    uint8_t ack_flag = 9;
    SDA_IN();
    SDA(1);
    delay_us(5);
    SCL(1);
    delay_us(5);

    while ((SDA_GET()) && (ack_flag)) {
        ack_flag--;
        delay_us(5);
    }
    // 无应答
    if (ack_flag == 0) {
        I2C_Stop();
        return 1;
    } else { // 有应答
        SCL(0);
        SDA_OUT();
    }
    return ack;
}

void I2C_Send_Byte(uint8_t byte)
{
    uint8_t mask; // 掩码
    SDA_OUT();    // SDA输出模式
    SCL(0);
    // 数据从高到低
    for (mask = 0x80; mask; mask >>= 1) {
        if (byte & mask) {
            SDA(1);
        } else {
            SDA(0);
        }
        delay_us(1);
        SCL(1);
        delay_us(5);
        SCL(0);
        delay_us(5);
    }
}

uint8_t I2C_Read_Byte()
{
    uint8_t mask;
    uint8_t data = 0;
    SDA_IN();
    // 数据从高到低
    for (mask = 0x80; mask; mask >>= 1) {
        SCL(0);
        delay_us(5);
        SCL(1);
        delay_us(5);
        if (SDA_GET() == 1) {
            data |= mask;
        }
    }
    SCL(0);
    return data;
}

/// @brief Initialize AHT10 for reading temperature and humidity
static void AHT10_Init_Read_Mode()
{
    I2C_Start();
    I2C_Send_Byte(WRITEMODE);
    if (I2C_Wait_Ack()) {
        printf("Failed When Initialize AHT10\r\n");
    }
    // See Datasheet for details
    I2C_Send_Byte(READ_TEMP);
    if (I2C_Wait_Ack()) {
        printf("Failed When Sending Read Command\r\n");
    }
    I2C_Send_Byte(PARA0);
    if (I2C_Wait_Ack()) {
        printf("Failed When Sending PARA0\r\n");
    }
    I2C_Send_Byte(PARA1);
    if (I2C_Wait_Ack()) {
        printf("Failed When Sending PARA1\r\n");
    }
    I2C_Stop();
}

/// @brief Write data into recvBuffer
/// @param recvBuffer the buffer to store the data
static void AHT10_Write_recvBuffer(uint8_t *recvBuffer)
{
    uint8_t i = 0;

    // Waiting for measuring and keep asking if finished
    do {
        delay_us(80000); // Waiting for sensor
        I2C_Start();
        I2C_Send_Byte(READMODE);
        i = I2C_Read_Byte();
        // 0x4C here: 01001100
        // printf("Status: %x\n", i);
    } while (!(i & 0x40)); // Keep query till not busy

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
