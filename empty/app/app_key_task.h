#ifndef _APP_KEY_TASK_H_
#define _APP_KEY_TASK_H_

#include "ti_msp_dl_config.h"
#include "mid_button.h"

void set_app_key_current_mode(char mode);
char get_app_key_current_mode(void);

void btn_up_cb(flex_button_t *btn);
void btn_left_cb(flex_button_t *btn);
void btn_right_cb(flex_button_t *btn);
void btn_down_cb(flex_button_t *btn);

#endif
