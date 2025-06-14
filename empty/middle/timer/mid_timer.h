#ifndef __MID_TIMER_H__
#define __MID_TIMER_H__

#include "ti_msp_dl_config.h"

// 定义枚举类型
typedef enum {
    TASK_DISABLE = 0,
    TASK_ENABLE
} Task_state;

void timer_init(void);
void enable_task_interrupt(void);
void disable_task_interrupt(void);

#endif