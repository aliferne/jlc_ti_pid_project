#ifndef __APP_UART_TASK_H__
#define __APP_UART_TASK_H__

#include "ti_msp_dl_config.h"

#define MAX_CMD_LENGTH 127

typedef struct
{
    const char motor_on[MAX_CMD_LENGTH];
    const char motor_off[MAX_CMD_LENGTH];
    const char motor_stop[MAX_CMD_LENGTH];
} ctrl_motor_state;

typedef struct
{
    const char cmd[MAX_CMD_LENGTH];
    const uint8_t value;
} motor_cmd;

typedef struct
{
    const motor_cmd *plus_30;
    const motor_cmd *plus_60;
    const motor_cmd *plus_90;
    const motor_cmd *minus_30;
    const motor_cmd *minus_60;
    const motor_cmd *minus_90;
} ctrl_motor_speed;

void recv_cmd();
void send_recv_data();

#endif /* __APP_UART_TASK_H__ */
