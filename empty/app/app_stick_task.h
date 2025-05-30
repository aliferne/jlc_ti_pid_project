/**
 * @file app_stick_task.h
 * @brief 摇杆控制任务应用层头文件
 * 
 * 本头文件定义了摇杆控制任务的应用层接口，主要用于通过摇杆输入
 * 来控制电机转速、方向等参数。该模块作为硬件抽象层与应用逻辑
 * 之间的桥梁，提供摇杆数据处理和电机控制的高级接口。
 *
 * @note 该模块依赖于底层ADC驱动和电机控制模块
 * @note 支持摇杆控制（Y轴）
 * 
 * @author Ferne
 * @date 2025/5/30
 * @version 1.0
 * @warning 在这里的配置中，摇杆往上拨到最顶部电压值为3.3，往下为0.0
 * @warning 公式是 `adc_value/4095.0*3.3`
 * 
 * @see hw_adc.h
 */

#ifndef __APP_STICK_TASK_H__
#define __APP_STICK_TASK_H__

#include "ti_msp_dl_config.h"

// void app_stick_task_init(void);

#endif /* __APP_STICK_TASK_H__ */