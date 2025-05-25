/**
 * @file hw_key.h
 * @brief 硬件按键管理相关函数和结构体声明
 */

#ifndef _HW_KEY_H_
#define _HW_KEY_H_

#include "ti_msp_dl_config.h"

/**
 * @brief 按键状态结构体
 *
 * 该结构体使用位字段来表示每个按键的状态，以节省空间。
 */
typedef struct {
    unsigned int up : 1;    //!< 上按键状态
    unsigned int left : 1;  //!< 左按键状态
    unsigned int right : 1; //!< 右按键状态
    unsigned int down : 1;  //!< 下按键状态
    unsigned int mid : 1;   //!< 中间按键状态
} KEY_STATUS;

/**
 * @brief 扫描按键状态
 *
 * @return 当前按键状态
 *
 * 该函数用于扫描并返回当前按键的状态。
 */
KEY_STATUS key_scan(void);

#endif // _HW_KEY_H_
