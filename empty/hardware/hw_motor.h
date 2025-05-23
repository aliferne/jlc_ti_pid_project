#ifndef _HW_MOTOR_H_
#define _HW_MOTOR_H_

#include "ti_msp_dl_config.h"

// 电机最大输入的PWM值
#define MOTOR_PWM_MAX 9999

void set_motor(uint16_t fi_value, uint16_t bi_value);
void stop_motor(void);

#endif
