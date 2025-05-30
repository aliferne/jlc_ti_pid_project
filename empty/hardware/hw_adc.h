/**
 * @file hw_adc.h
 * @brief 摇杆Y轴ADC硬件抽象层头文件
 * 
 * 本头文件定义了摇杆Y轴ADC模块的公共接口函数声明和相关宏定义。
 * 提供ADC初始化和数据读取的标准化接口。
 * 
 * @author Ferne
 * @date 2025/5/30
 * @version 1.0
*/

#ifndef __HW_ADC_H__
#define __HW_ADC_H__

#include "ti_msp_dl_config.h"

/**
 * @defgroup ADC_Module ADC模块
 * @brief 摇杆Y轴ADC相关功能
 * @{
 */

/**
 * @brief 初始化摇杆Y轴ADC模块
 * 
 * 该函数执行ADC模块的基本初始化操作，主要是使能ADC中断。
 * 调用此函数后，ADC模块将能够响应转换完成中断。
 * 
 * @note 函数使用inline修饰符，建议编译器内联展开以提高执行效率
 * @note 调用此函数前需确保ADC硬件已通过系统配置工具正确配置
 * 
 * @see NVIC_EnableIRQ()
 * @see ADC_STICK_Y_INST_INT_IRQN
 * 
 * @par 使用示例:
 * @code
 * // 在主程序初始化时调用
 * adc_init();
 * @endcode
 */
void adc_init(void);

/**
 * @brief 获取摇杆Y轴ADC转换结果
 * 
 * 该函数启动ADC转换，等待转换完成，然后读取并返回转换结果。
 * 采用阻塞式等待方式，直到ADC转换完成才返回。
 * 
 * @return uint32_t ADC转换结果值
 *         - 范围通常为 0 到 4095（12位ADC）
 *         - 具体范围取决于ADC配置和参考电压
 * 
 * @details 函数执行流程：
 *          1. 软件触发ADC开始转换
 *          2. 轮询等待转换完成标志
 *          3. 读取ADC转换结果
 *          4. 清除转换完成标志
 *          5. 返回转换结果
 * 
 * @warning 该函数会阻塞执行直到ADC转换完成，调用时需注意实时性要求
 * @warning 调用前需确保已调用 adc_init() 函数进行初始化
 * 
 * @see adc_init()
 * @see ADC_STICK_Y_INST_IRQHandler()
 * 
 * @par 使用示例:
 * @code
 * uint32_t joystick_y_value;
 * joystick_y_value = adc_getValue();
 * printf("Joystick Y value: %lu\n", joystick_y_value);
 * @endcode
 */
uint32_t adc_getValue(void);

#endif