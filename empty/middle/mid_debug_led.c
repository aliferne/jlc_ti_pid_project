#include "mid_debug_led.h"

static DEBUG_LED_STRUCT debug_led;

void set_debug_led()
{
    debug_led.state = LED_ON; // 打开板载LED（PB22）
    DL_GPIO_setPins(DEBUG_LED_PORT, DEBUG_LED_PIN_22_PIN);
}

void clear_debug_led()
{
    debug_led.state = LED_OFF; // 关闭板载LED（PB22）
    DL_GPIO_clearPins(DEBUG_LED_PORT, DEBUG_LED_PIN_22_PIN);
}

void toggle_debug_led()
{
    // 翻转LED状态
    if (debug_led.state == LED_ON) {
        clear_debug_led();
    } else {
        set_debug_led();
    }
}

LED_STATE get_debug_led_state()
{
    // 返回LED状态
    return debug_led.state;
}
