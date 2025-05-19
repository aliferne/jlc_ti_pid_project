#include "bsp_i2c.h"

#define delay_us(X) delay_cycles((CPUCLK_FREQ / 1000000) * (X))

// Set SDA to output
void SDA_OUT()
{
    DL_GPIO_initDigitalOutput(I2C_SDA_IOMUX);
    DL_GPIO_setPins(I2C_PORT, I2C_SDA_PIN);
    DL_GPIO_enableOutput(I2C_PORT, I2C_SDA_PIN);
}

// Set SDA to input
void SDA_IN()
{
    DL_GPIO_initDigitalInput(I2C_SDA_IOMUX);
}
// Get SDA status
uint8_t SDA_GET()
{
    return (DL_GPIO_readPins(I2C_PORT, I2C_SDA_PIN) & I2C_SDA_PIN) > 0 ? 1 : 0;
}


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
