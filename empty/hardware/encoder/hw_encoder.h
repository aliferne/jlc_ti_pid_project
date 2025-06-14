#ifndef _HW_ENCODER_H_
#define _HW_ENCODER_H_

#include "ti_msp_dl_config.h"

#define SPEED_ENCODER_VALUE_MAX    (-(SPEED_ENCODER_VALUE_MIN))
#define SPEED_ENCODER_VALUE_MID    0
#define SPEED_ENCODER_VALUE_MIN    (-100)

#define DISTANCE_ENCODER_VALUE_MAX (-(DISTANCE_ENCODER_VALUE_MIN))
#define DISTANCE_ENCODER_VALUE_MID 0
#define DISTANCE_ENCODER_VALUE_MIN (-360)

typedef enum {
    FORWARD, // 正向
    REVERSAL // 反向
} ENCODER_DIR;

typedef struct {
    volatile long long temp_count; // 保存实时计数值
    int count;                     // 根据定时器时间更新的计数值
    ENCODER_DIR dir;               // 旋转方向
} ENCODER_RES;

/**
 * @brief 初始化编码器
 *
 * 该函数用于初始化编码器硬件和软件资源。
 */
void encoder_init(void);

/**
 * @brief 获取编码器计数
 *
 * @return 编码器的当前计数值
 *
 * 该函数用于获取编码器的当前计数值，表示旋转的圈数或距离。
 */
int get_encoder_count(void);

/**
 * @brief 获取编码器方向
 *
 * @return 编码器的当前旋转方向
 *
 * 该函数用于获取编码器的当前旋转方向，可能是顺时针或逆时针。
 */
ENCODER_DIR get_encoder_dir(void);

/**
 * @brief 获取编码器实时数据
 *
 * @return 编码器的当前实时计数值
 *
 * 该函数用于获取编码器的当前实时计数值。
 */
long long get_temp_encoder(void);

/**
 * @brief 更新编码器状态
 *
 * 该函数用于更新编码器的状态，包括计数和方向。
 */
void encoder_update(void);

#endif