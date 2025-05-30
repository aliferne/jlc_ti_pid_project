/**
 * @file hw_adc.c
 * @brief 摇杆Y轴ADC硬件抽象层实现
 *
 * 本文件实现了摇杆Y轴ADC的初始化、数据读取和中断处理功能。
 * 使用软件触发方式进行ADC转换，并通过中断机制获取转换完成状态。
 *
 * @author Ferne
 * @date 2025/5/30
 * @version 1.0
 * @note ADC接收端口在PA15
 */

#include "hw_adc.h"
#include "stdio.h"

/**
 * @brief ADC转换完成标志
 *
 * 该静态全局变量用于标识ADC转换是否完成。
 * - true: ADC转换已完成，可以读取结果
 * - false: ADC转换未完成或结果已被读取
 *
 * @note 该变量使用volatile修饰，因为会在中断服务程序中被修改
 */
static volatile bool isAdcConverted;

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
 */
void adc_init()
{
    NVIC_EnableIRQ(ADC_STICK_Y_INST_INT_IRQN);
}

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
 * @see DL_ADC12_startConversion()
 * @see DL_ADC12_getResult()
 * @see ADC_STICK_Y_INST_IRQHandler()
 */
uint32_t adc_getValue()
{
    uint32_t adcResult = 0;
    // 软件触发，ADC开始转换
    DL_ADC12_startConversion(ADC_STICK_Y_INST);
    // 等待转换完成
    while (!isAdcConverted);
    // 读取ADC结果
    adcResult = DL_ADC12_getMemResult(ADC_STICK_Y_INST, ADC_STICK_Y_ADCMEM_CH0);
    // 清除标志位
    isAdcConverted = false;

    return adcResult;
}

/**
 * @brief 摇杆Y轴ADC中断服务程序
 *
 * 该函数处理ADC转换完成中断。当ADC转换完成时，硬件会触发中断，
 * 该函数被调用并设置转换完成标志，通知主程序可以读取ADC结果。
 *
 * @details 中断处理流程：
 *          - 检查中断类型是否为内存0结果加载完成
 *          - 如果是，则设置 isAdcConverted 标志为 true
 *          - 其他中断类型暂不处理
 *
 * @note 该函数为中断服务程序，应保持简短高效
 * @note 函数名必须与中断向量表中定义的名称一致
 * @note 不应直接调用此函数，由硬件中断触发
 *
 * @see adc_getValue()
 * @see DL_ADC12_getPendingInterrupt()
 * @see DL_ADC12_IIDX_MEM0_RESULT_LOADED
 */
void ADC_STICK_Y_INST_IRQHandler()
{
    switch (DL_ADC12_getPendingInterrupt(ADC_STICK_Y_INST)) {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            isAdcConverted = true;
            break;
        default:
            break;
    }
}
