/**
 * @File:    flexible_button.c
 * @Author:  MurphyZhao
 * @Date:    2018-09-29
 * 
 * Copyright (c) 2018-2019 MurphyZhao <d2014zjt@163.com>
 *               https://github.com/murphyzhao
 * All rights reserved.
 * License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Change logs:
 * Date        Author       Notes
 * 2018-09-29  MurphyZhao   First add
 * 2019-08-02  MurphyZhao   Migrate code to github.com/murphyzhao account
 * 2019-12-26  MurphyZhao   Refactor code and implement multiple clicks
 * 
*/

#include "mid_button.h"
#include "hw_key.h"
#include "app_key_task.h"

// 设置你要控制多少个按键
// 最后一位总是USER_BUTTON_MAX
typedef enum
{
    BUTTON_UP = 0, 
    BUTTON_LEFT,     
    BUTTON_RIGHT,    
    BUTTON_DOWN,   
    USER_BUTTON_MAX
} user_button_t;

static flex_button_t user_button[USER_BUTTON_MAX];

static flex_button_t *btn_head = NULL;

static uint16_t trg = 0;
static uint16_t cont = 0;
static uint16_t keydata = 0xFFFF;
static uint16_t key_rst_data = 0xFFFF;
static uint8_t button_cnt = 0;

#define EVENT_CB_EXECUTOR(button) if(button->cb) button->cb((flex_button_t*)button)
#define MAX_BUTTON_CNT 16


// 配置你按键的返回值
static uint8_t button_up_read(void)     { return key_scan().up;     }
static uint8_t button_left_read(void)   { return key_scan().left;   }
static uint8_t button_right_read(void)  { return key_scan().right;  }
static uint8_t button_down_read(void)   { return key_scan().down;   }

// 配置按键初始化
// 用户自行根据案例配置
void user_button_init(void)
{
    int i;

    /* 初始化按键数据结构 */
    memset(&user_button[0], 0x0, sizeof(user_button));

    user_button[BUTTON_UP].usr_button_read = button_up_read;//按键读值回调函数
    user_button[BUTTON_UP].cb = (flex_button_response_callback)btn_up_cb;//按键事件回调函数

    user_button[BUTTON_LEFT].usr_button_read = button_left_read;//按键读值回调函数
    user_button[BUTTON_LEFT].cb = (flex_button_response_callback)btn_left_cb;//按键事件回调函数

    user_button[BUTTON_RIGHT].usr_button_read = button_right_read;//按键读值回调函数
    user_button[BUTTON_RIGHT].cb = (flex_button_response_callback)btn_right_cb;//按键事件回调函数

    user_button[BUTTON_DOWN].usr_button_read = button_down_read;//按键读值回调函数
    user_button[BUTTON_DOWN].cb = (flex_button_response_callback)btn_down_cb;//按键事件回调函数

    /* 初始化 按键引脚 */

    for (i = 0; i < USER_BUTTON_MAX; i ++)
    {
        user_button[i].pressed_logic_level = 0; //按键按下时的逻辑电平
        user_button[i].click_start_tick = 7;//10;
        user_button[i].short_press_start_tick = 12;//20;//短按起始 tick
        user_button[i].long_press_start_tick = 27;//40;//长按起始 tick
        user_button[i].long_hold_start_tick = 32;//45;//长按保持起始tick

        flex_button_register(&user_button[i]);
    }
}

/**
 * @brief Register a user button
 * 
 * @param button: button structure instance
 * @return Number of keys that have been registered
*/
int8_t flex_button_register(flex_button_t *button)
{
    flex_button_t *curr = btn_head;
    
    if (!button || (button_cnt > MAX_BUTTON_CNT))
    {
        return -1;
    }

    while (curr)
    {
        if(curr == button)
        {
            return -1;  //already exist.
        }
        curr = curr->next;
    }

    button->next = btn_head;
    button->status = 0;
    button->event = FLEX_BTN_PRESS_NONE;
    button->scan_cnt = 0;
    button->click_cnt = 0;
    btn_head = button;
    key_rst_data = key_rst_data << 1;
    button_cnt ++;
    return button_cnt;
}

/**
 * @brief Read all key values in one scan cycle
 * 
 * @param void
 * @return none
*/
static void flex_button_read(void)
{
    flex_button_t* target;
    uint16_t read_data = 0;
    keydata = key_rst_data;
    int8_t i = 0;

    for(target = btn_head, i = 0;
        (target != NULL) && (target->usr_button_read != NULL);
        target = target->next, i ++)
    {
        keydata = keydata |
                  (target->pressed_logic_level == 1 ?
                  ((!(target->usr_button_read)()) << i) :
                  ((target->usr_button_read)() << i));
    }

    read_data = keydata^0xFFFF;
    trg = read_data & (read_data ^ cont);
    cont = read_data;
}

/**
 * @brief Handle all key events in one scan cycle.
 *        Must be used after 'flex_button_read' API
 * 
 * @param void
 * @return none
*/
static void flex_button_process(void)
{
    int8_t i = 0;
    flex_button_t* target;

    for (target = btn_head, i = 0; target != NULL; target = target->next, i ++)
    {
        if (target->status > 0)
        {
            target->scan_cnt ++;
        }

        switch (target->status)
        {
            case 0: /* is default */
                if (trg & (1 << i)) /* is pressed */
                {
                    target->scan_cnt = 0;
                    target->click_cnt = 0;
                    target->status = 1;
                    target->event = FLEX_BTN_PRESS_DOWN;
                    EVENT_CB_EXECUTOR(target);
                }
                else
                {
                    target->event = FLEX_BTN_PRESS_NONE;
                }
                break;

            case 1: /* is pressed */
                if (!(cont & (1 << i))) /* is up */
                {
                    target->status = 2;
                }
                else if ((target->scan_cnt >= target->short_press_start_tick) &&
                        (target->scan_cnt < target->long_press_start_tick))
                {
                    target->status = 4;
                    target->event = FLEX_BTN_PRESS_SHORT_START;
                    EVENT_CB_EXECUTOR(target);
                }
                break;

            case 2: /* is up */
                if ((target->scan_cnt < target->click_start_tick))
                {
                    target->click_cnt++; // 1
                    
                    if (target->click_cnt == 1)
                    {
                        target->status = 3;  /* double click check */
                    }
                    else
                    {
                        target->click_cnt = 0;
                        target->status = 0;
                        target->event = FLEX_BTN_PRESS_DOUBLE_CLICK;
                        EVENT_CB_EXECUTOR(target);
                    }
                }
                else if ((target->scan_cnt >= target->click_start_tick) &&
                    (target->scan_cnt < target->short_press_start_tick))
                {
                    target->click_cnt = 0;
                    target->status = 0;
                    target->event = FLEX_BTN_PRESS_CLICK;
                    EVENT_CB_EXECUTOR(target);
                }
                else if ((target->scan_cnt >= target->short_press_start_tick) &&
                    (target->scan_cnt < target->long_press_start_tick))
                {
                    target->click_cnt = 0;
                    target->status = 0;
                    target->event = FLEX_BTN_PRESS_SHORT_UP;
                    EVENT_CB_EXECUTOR(target);
                }
                else if ((target->scan_cnt >= target->long_press_start_tick) &&
                    (target->scan_cnt < target->long_hold_start_tick))
                {
                    target->click_cnt = 0;
                    target->status = 0;
                    target->event = FLEX_BTN_PRESS_LONG_UP;
                    EVENT_CB_EXECUTOR(target);
                }
                else if (target->scan_cnt >= target->long_hold_start_tick)
                {
                    /* long press hold up, not deal */
                    target->click_cnt = 0;
                    target->status = 0;
                    target->event = FLEX_BTN_PRESS_LONG_HOLD_UP;
                    EVENT_CB_EXECUTOR(target);
                }
                break;

            case 3: /* double click check */
                if (trg & (1 << i))
                {
                    target->click_cnt++;
                    target->status = 2;
                    target->scan_cnt --;
                }
                else if (target->scan_cnt >= target->click_start_tick)
                {
                    target->status = 2;
                }
                break;

            case 4: /* is short pressed */
                if (!(cont & (1 << i))) /* is up */
                {
                    target->status = 2;
                }
                else if ((target->scan_cnt >= target->long_press_start_tick) &&
                        (target->scan_cnt < target->long_hold_start_tick))
                {
                    target->status = 5;
                    target->event = FLEX_BTN_PRESS_LONG_START;
                    EVENT_CB_EXECUTOR(target);
                }
                break;

            case 5: /* is long pressed */
                if (!(cont & (1 << i))) /* is up */
                {
                    target->status = 2;
                }
                else if (target->scan_cnt >= target->long_hold_start_tick)
                {
                    target->status = 6;
                    target->event = FLEX_BTN_PRESS_LONG_HOLD;
                    EVENT_CB_EXECUTOR(target);
                }
                break;

            case 6: /* is long pressed */
                if (!(cont & (1 << i))) /* is up */
                {
                    target->status = 2;
                }
                break;
        }
    }
}

/**
 * flex_button_event_read
 * 
 * @brief Get the button event of the specified button.
 * 
 * @param button: button structure instance
 * @return button event
*/
flex_button_event_t flex_button_event_read(flex_button_t* button)
{
    return (flex_button_event_t)(button->event);
}

/**
 * flex_button_scan
 * 
 * @brief Start key scan.
 *        Need to be called cyclically within the specified period.
 *        Sample cycle: 5 - 20ms
 * 
 * @param void
 * @return none
*/
void flex_button_scan(void)
{
    flex_button_read();
    flex_button_process();
}
