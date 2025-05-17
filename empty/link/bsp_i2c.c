#include "bsp_i2c.h"

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
