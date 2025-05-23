#include "hw_key.h"

KEY_STATUS key_scan(void)
{
    KEY_STATUS states;
    // 读取每个按键的状态
    states.up = DL_GPIO_readPins(GPIO_KEY_PORT, GPIO_KEY_PIN_UP_PIN) ? 1 : 0;
    states.left = DL_GPIO_readPins(GPIO_KEY_PORT, GPIO_KEY_PIN_LEFT_PIN) ? 1 : 0;
    states.right = DL_GPIO_readPins(GPIO_KEY_PORT, GPIO_KEY_PIN_RIGHT_PIN) ? 1 : 0;
    states.down = DL_GPIO_readPins(GPIO_KEY_PORT, GPIO_KEY_PIN_DOWN_PIN) ? 1 : 0;

    return states;
}