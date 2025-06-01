#ifndef __APP_UART_TASK_H__
#define __APP_UART_TASK_H__

#include "ti_msp_dl_config.h"

typedef struct
{
    const char *motor_on;
    const char *motor_off;
    const char *motor_stop;
} CTRL_MOTOR_STATE;

typedef struct
{
    const char *speed_plus_30;
    const char *speed_plus_60;
    const char *speed_plus_90;
    const char *speed_minus_30;
    const char *speed_minus_60;
    const char *speed_minus_90;
} CTRL_MOTOR_SPEED;

#endif /* __APP_UART_TASK_H__ */
