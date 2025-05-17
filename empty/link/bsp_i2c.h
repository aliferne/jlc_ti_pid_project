#ifndef __BSP_I2C_H__
#define __BSP_I2C_H__

#include "../ti_msp_dl_config.h"
#include "stdio.h"

// Set SDA to output
void SDA_OUT();
// Set SDA to input
void SDA_IN();
// Get SDA status
uint8_t SDA_GET();

// SDA and SCL output
#define SDA(x) ((x) ? DL_GPIO_setPins(I2C_PORT, I2C_SDA_PIN) : (DL_GPIO_clearPins(I2C_PORT, I2C_SDA_PIN)))
#define SCL(x) ((x) ? DL_GPIO_setPins(I2C_PORT, I2C_SCL_PIN) : (DL_GPIO_clearPins(I2C_PORT, I2C_SCL_PIN)))

#endif
