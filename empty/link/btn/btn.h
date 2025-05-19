#ifndef __BTN_H__
#define __BTN_H__

#include "../../ti_msp_dl_config.h"

/**
 * @brief 读取按钮的状态
 *
 * 该函数用于读取指定GPIO端口和引脚上的按钮状态。
 *
 * @param gpio_btn 指向GPIO端口寄存器的指针
 * @param pins 指定要读取的引脚
 * @return 按钮的状态，true表示按下，false表示未按下
 */
inline bool Read_Btn_State(GPIO_Regs *gpio_btn, uint32_t pins)
{
    // 读取状态，如果某一位不为1就返回true，否则证明没有按键按下
    return DL_GPIO_readPins(gpio_btn, pins) ? false : true;
}

#endif