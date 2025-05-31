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
 *
 * @note 在这里的配置中，摇杆往上拨到最顶部输出4095，往下为0，
 * @note 但实际上由于参考电压VREF=3.3V
 * @note 所以哪怕拨动一点点都会到4095，反而往下拨动到最底部才会到0
 * @note 我综合了一下，最终决定使用公式 `adc_getValue() / 40.95`，
 * @note 从而可以将值限定在0到100之间
 * @note 此外不拨动摇杆时读数稳定在76左右。
 * @note 我超！本来不是GPIO口不够用吗，我直接把检测端口接到面包板了
 * @note 然后x轴和y轴都能检测到了，我超，我是天才!😋
 * @note 但这样子搞了之后，最低就降不到0了，而是在3左右波动（不超过5）
 *
 * @see hw_adc.h
 */

#ifndef __APP_STICK_TASK_H__
#define __APP_STICK_TASK_H__

#include "ti_msp_dl_config.h"

#define STICK_MAX_OUTPUT     100.00f // 摇杆最大输出值
#define STICK_MIN_OUTPUT     2.50f   // 摇杆最小输出值
#define STICK_DEFAULT_OUTPUT 76.00f  // 摇杆默认输出值（即不拨动摇杆）
#define TOLERANCE            1       // 由于默认值存在波动，所以需要给一个误差范围
// 判断摇杆往左/下拨动的阈值（需要用 `>`  判断）
#define STICK_TO_LEFT_DOWN_CHECKER (STICK_DEFAULT_OUTPUT + TOLERANCE)
// 判断摇杆往右/上拨动的阈值（需要用 `<`  判断）
#define STICK_TO_RIGHT_UP_CHECKER (STICK_DEFAULT_OUTPUT - TOLERANCE)
// 将摇杆的输出值除以这个倍率，从而可以用来扩大其他值
#define MINIMIZE_TIMES 10

// 由于摇杆在减小到0时会有波动（我们采用了X和Y轴接到同一个通道去的做法）
// 所以需要这个来判断是否把摇杆拨动到最底
#define ABS(x) ((x) > 0 ? (x) : -(x))


void stick_scan();
/**
 * @brief 摇杆z轴按键事件回调函数
 *
 * @param btn 指向按键结构体的指针
 */
void btn_stick_z_cb(flex_button_t *btn);

#endif /* __APP_STICK_TASK_H__ */