#ifndef __MID_DEBUG_LED_H__
#define __MID_DEBUG_LED_H__

#include "../ti_msp_dl_config.h"

typedef enum {
    LED_OFF,
    LED_ON
} LED_STATE;

typedef struct {
    LED_STATE state;
} DEBUG_LED_STRUCT;

/**
 * @brief 设置调试LED的状态为打开
 *
 * 此函数将调试LED的状态设置为打开，并点亮相应的LED。
 */
void set_debug_led(void);

/**
 * @brief 设置调试LED的状态为关闭
 *
 * 此函数将调试LED的状态设置为关闭，并熄灭相应的LED。
 */
void clear_debug_led(void);

/**
 * @brief 切换调试LED的状态
 *
 * 此函数切换调试LED的当前状态。如果LED当前是打开的，它将被关闭；如果LED当前是关闭的，它将被打开。
 */
void toggle_debug_led(void);

/**
 * @brief 获取调试LED的当前状态
 *
 * @return 当前LED的状态，可以是LED_ON或LED_OFF。
 */
LED_STATE get_debug_led_state(void);

#endif