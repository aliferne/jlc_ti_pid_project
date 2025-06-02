> @所有人  小伙伴们，现在PID训练营项目已经进入到自主完成阶段
> 【截止时间】6月11号之前
> 
> 【拉黑提醒】领券后必须在规定时间内完成，不然会被拉进黑名单，领券的小伙伴注意规划好时间完成项目并开源
> 
> 【开源提醒】完成项目后要进行开源了。开源审核通过后，把进度改为【已开源】评奖时间：6.5~6.11
> 
> 【开源工程写得越好，越容易拿奖】：
> 1. 封面图：验证好的实物图；
> 2. 描述：完整的工程描述（简介、功能介绍、原理图说明、实物图、pcb设计说明、关键程序说明、重要物料购买、装配说明）
> 3. 视频：功能演示视频；
> 4. 附件：工程的源码或相关外壳文件等附件； 
> 例子：https://oshwhub.com/li-chuang-kai-fa-ban/cw32-shu-zi-dian-ya-dian-liu-biao-kai-fa-ban-tao-jian


# 立创·天猛星MSPM0G3507简易PID电机驱动项目

## 零、设计背景

PID（Proportional-Integral-Derivative）控制器是一种经典的控制系统，它通过调整 比例（P）、积分（I）、微分（D）三个参数 来实现自动控制，核心是根据系统当前的误差值计算出合适的控制量，使系统输出稳定在目标值附近。由于PID的控制特性，它常被用在电赛的小车类项目，如平衡小车等。

学习PID控制器，可以让我们更好地理解控制系统的工作原理，提高我们的编程能力，并掌握通往电赛和其他类型的控制项目的关键秘诀。而这也是本项目的意义所在。

## 一、项目简介

本项目基于立创天猛星开发板，使用MSPM0G3507微型处理器，实现了一个简易的PID电机驱动项目。通过调节PID参数，可以实现对电机的精确控制，包括速度控制和位置控制。项目特点如下：

### 硬件架构

#### 主控芯片

- **型号**：TI MSPM0G3507
- **平台**：立创天猛星开发板
- **架构**：ARM Cortex-M0+

#### 外设配置

- **电机驱动**：PWM控制，支持正反转
- **编码器**：正交编码器，用于位置和速度反馈
- **显示屏**：SPI接口TFT彩屏
- **按键**：5个独立按键 + 1个摇杆按键
- **串口**：UART调试和命令接口

### 软件功能

#### 1. 可视化用户界面

- **TFT彩屏显示**：提供直观的参数显示和控制界面
- **实时PID曲线**：动态显示PID控制效果和系统响应
- **多页面UI管理**：包括主页、定速页、定距页和参数设置页

#### 2. 双模式PID控制

- **定速控制**：基于编码器反馈的速度闭环控制
- **定距控制**：基于位置反馈的角度定位控制
- **实时参数调节**：支持在线调整P、I、D参数

#### 3. 多样化交互方式

- **五向按键**：上/下/左/右/中间按键，支持短按和长按
- **模拟摇杆**：提供模拟量输入控制
- **串口通信**：支持外部指令控制

#### 4. 扩展功能（规划中）

- **语音识别**：基于天问ASRPro芯片的TTS服务
- **Q弹UI效果**：基于PID算法的动态交互效果
- **LVGL移植**：更丰富的图形界面库

## 二、功能介绍

1. 电机速度控制：通过调节PID参数和目标速度参数，实现对电机的精确速度控制
2. 电机位置控制：通过调节PID参数和目标角度参数，实现对电机的精确位置控制
3. 摇杆控制：通过摇杆控制电机速度和方向
4. 语音控制：通过语音控制电机速度和方向
5. UI动效：将PID融入UI中，使得UI界面更加生动有趣

## 三、原理图说明

![电源电路原理图](./img/电源电路原理图.png)

电源电路采用一个 TYPE-C 接口输入 5V 电源，通过开关进行控制。其中有两个电源信号：MOTOR_VCC 和 +5V。

- MOTOR_VCC 是直接将TYPE-C的电源接入电机正负极。
- +5V 是由 MOTOR_VCC 经过一个防倒灌二极管 U0 后的电源，该电源接入到开发板的5V上，为开发板供电。

![电机驱动原理图](./img/电机驱动原理图.png)

电机驱动芯片U4采用巴丁微电子生产的BDR6126D，这款芯片为大功率直流电机驱动芯片，最大可承受`18V`电压，持续输出电流可达`4.5A`，同时由于电机在启动及快速正反转时会有较大的瞬时电流，因此增加了蓄能电容。

![按键电路](./img/按键电路.png)

为节约成本，按键电路并未设置硬件消抖措施，需要使用软件消抖。

![开发板电路](./img/开发板电路.png)

开发板输入电源为+5V，其本身自带5V降压到3V3的功能，可以直接作为编码器的电源输入，电机控制信号线FI/BI连接到了GPIOA26和GPIOA27（这两个都支持PWM），而AB相信号可以接到开发板上的任意GPIO，只要通过软件+外部中断来采集数据即可，按键则无所谓哪个GPIO，只要配置好上拉电阻即可。

## 四、PCB设计说明

![PCB示意图](img/PCB示意图.png)

在布局的设计上我和官方案例略有不同，我选择将核心板竖直放置，并多引出了排针，以方便后期想要添加功能的需求和一板用到底的需求。而为了适应布局需要，我将电源线稍微调细了些，此外为了美观和布局方便，我将钽电容放置在同一行、瓷片电容放置在同一行。

## 五、实物图

TODO:

## 六、关键程序说明

1. 在彩屏驱动方面，我抑制了官方案例，但需要注意的是，我的开发环境为vscode，因此有些地方并不一样，比如函数 `LCD_Show_Chinese` 中，我所修改的部分如下：

```c
/**
 * @brief 显示汉字串
 * @param x 汉字显示的X坐标
 * @param y 汉字显示的Y坐标
 * @param s 汉字字符串指针
 * @param fc 前景色（字体颜色）
 * @param bc 背景色
 * @param sizey 字体大小
 * @param mode 显示模式
 */
void LCD_Show_Chinese(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    while (*s != 0) {
        if (*s >= 128) {
            if (sizey == 12)
                LCD_Show_Chinese12x12(x, y, s, fc, bc, sizey, mode);
            else if (sizey == 16)
                LCD_Show_Chinese16x16(x, y, s, fc, bc, sizey, mode);
            else if (sizey == 24)
                LCD_Show_Chinese24x24(x, y, s, fc, bc, sizey, mode);
            else if (sizey == 32)
                LCD_Show_Chinese32x32(x, y, s, fc, bc, sizey, mode);
            else
                return;
            s += 3; // NOTE: utf-8 汉字占用3个字节
            x += sizey;
        } else {
            LCD_Show_Char(x, y, *s, fc, bc, sizey, mode);
            s += 1;
            x += (sizey / 2);
        }
    }
}
```

在我提到的 `s += 3;` 那行语句，官方示例采用的是 `s += 2;`，实际上这是 Keil 默认以 GB2312 为编码的结果，而 vscode 默认是 utf-8 编码，自然需要修改。另附上不修改的显示图片：

![不修改的显示图片](./img/不修改s的显示图片.jpg)

因此我同时也修改了`lcdfont.h`中的一些结构体定义，这里仅举一例：

```c
typedef struct
{
    // NOTE: 这里从2变为了4（汉字三个字节，还有一个\0）
    unsigned char Index[4];
    unsigned char Msk[32];
} typFNT_GB16; // 我并没有更改结构体的名称，虽然其实应该改成UTF的
```

2. 在按键驱动方面，我和官方案例一样移植了`flexible_button`开源库，同时加入了自己的一些按键扫面函数和按键回调函数的处理逻辑，代码如下：

```c
void btn_mid_cb(flex_button_t *btn)
{
    switch (btn->event) {
        case FLEX_BTN_PRESS_CLICK: // 单击事件
            // 如果是定速页或者定距页
            if (get_show_state() == PID_PAGE || get_show_state() == DISTANCE_PAGE) {
                // 触发电机事件
                event_manager(&system_status, MOTOR_EVENT);
                // 如果电机状态是关
                if (get_motor_status_flag() == MOTOR_STATUS_OFF) {
                    // 停止电机
                    stop_motor();
                }
            }
            break;
        case FLEX_BTN_PRESS_LONG_HOLD: // 长按保持事件
            break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP: // 长按保持后抬起事件
            break;
        default:
            break;
    }
}

void btn_stick_z_cb(flex_button_t *btn)
{
    switch (btn->event) {
        case FLEX_BTN_PRESS_CLICK: // 单击事件
            // 如果是定速页或者定距页
            if (get_show_state() == PID_PAGE || get_show_state() == DISTANCE_PAGE) {
                // 触发电机事件
                event_manager(&system_status, MOTOR_EVENT);
                // 如果电机状态是关
                if (get_motor_status_flag() == MOTOR_STATUS_OFF) {
                    // 停止电机
                    stop_motor();
                }
            }
            break;
        case FLEX_BTN_PRESS_LONG_HOLD: // 长按保持事件
            break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP: // 长按保持后抬起事件
            break;
        default:
            break;
    }
}


```

3. 在电机驱动方面，我新增了一些函数，使得可以在不需要判断PWM正负的情况下直接传入该函数内部（它会帮我们处理好），代码如下：

```c
/**
 * @brief 设置电机速度，需要传入一个速度值
 * @param value 速度值，可正可负，正为顺时针，负为逆时针
 */
void set_motor_value(int16_t value)
{
    restrict_pwm_max_value(&value);
    if (value > 0) { 
        // 顺时针
        set_fi(0);
        set_bi(value);
    } else {
        // 逆时针
        set_fi(-value);
        set_bi(0);
    }
}
```

4. 学习官方案例的过程中我注意到，实际上官方的ui选择框的绘制略显生硬，没有什么动效，因此我就在思考能否将ui绘制与pid结合起来，使选择框变得更加Q弹有活力，并且最终证明了我的思路其实是可行的，核心代码如下：

```c
/**
 * @brief 显示选择框
 * 
 * 该函数用于在指定位置绘制一个选择框，支持静态显示和PID动画显示两种模式。
 * 选择框由水平线和垂直线组成，可以设置线条长度、间距和颜色。
 * 
 * @param target_x 目标X坐标位置
 * @param w 选择框的宽度
 * @param target_y 目标Y坐标位置  
 * @param h 选择框的高度
 * @param line_length 线条的长度
 * @param interval 线条间距，同时也作为选择框向外扩展的距离
 * @param color 线条颜色
 * 
 * @details 
 * 函数内部有两种显示模式：
 * - mode = 0：静态显示模式，直接在目标位置绘制选择框
 * - mode ≠ 0：PID动画模式，使用PID控制算法实现选择框从当前位置平滑移动到目标位置
 * 
 * 在PID模式下：
 * - 使用PID参数：kp=0.4, ki=0.2, kd=0.2
 * - 动画过程中会先清除上一帧的线条，再绘制新位置的线条
 * - 当X轴和Y轴误差都小于0.5时停止动画
 * - 动画结束后会重置PID参数
 * 
 * @note 
 * - 选择框的实际绘制区域会比输入的w和h各边扩大interval个像素
 * - 需要依赖ui_draw_lines_horizonal()和ui_draw_lines_vertical()函数
 * - PID模式需要pid_init(), pid_calc(), pid_change_zero()等PID相关函数
 * - 需要定义LCD_W, LCD_H, BLACK等常量
 * 
 * @see ui_draw_lines_horizonal()
 * @see ui_draw_lines_vertical()
 * @see pid_init()
 * @see pid_calc()
 * @see pid_change_zero()
 */
void show_select_box(
    int target_x, int w, int target_y, int h,
    int line_length, int interval, int color)
{
    // interval: 间距
    target_x -= interval; // 向左偏移interval的距离
    w += interval * 2;    // 宽度增加interval * 2的距离（矩形框两边对称）

    target_y -= interval; // 向上偏移interval的距离
    h += interval * 2;    // 高度增加interval * 2的距离

    int mode = 0;

    if (mode == 0) {
        ui_draw_lines_horizonal(target_x, w, target_y, h, line_length, interval, color);
        ui_draw_lines_vertical(target_x, w, target_y, h, line_length, interval, color);
    } else {
        // p、i、d的参数
        float kp = 0.4, ki = 0.2, kd = 0.2;
        int current_x = 0; // 当前x轴坐标（从0开始）
        int current_y = 0; // 当前y轴坐标（从0开始）

        int value_x, value_y;          // x轴和y轴方向上与目标的差值
        PID_Struct screen_x, screen_y; // x轴和y轴方向上的pid结构体

        // 初始化PID
        pid_init(&screen_x, kp, ki, kd, LCD_W, LCD_W, target_x);
        pid_init(&screen_y, kp, ki, kd, LCD_H, LCD_H, target_y);

        do {
            // 分别计算x轴和y轴方向上的差值
            value_x = pid_calc(&screen_x, target_x, current_x);
            value_y = pid_calc(&screen_y, target_y, current_y);
            // 清除上次线条
            ui_draw_lines_horizonal(current_x, w, target_y, h, line_length, interval, BLACK);
            ui_draw_lines_vertical(target_x, w, current_y, h, line_length, interval, BLACK);
            // 平移到本次坐标的位置
            current_x += value_x;
            current_y += value_y;
            // 绘制本次线条
            ui_draw_lines_horizonal(current_x, w, target_y, h, line_length, interval, color);
            ui_draw_lines_vertical(target_x, w, current_y, h, line_length, interval, color);
            // 延迟一段时间
            delay_cycles(100000 * 8);
        } while (
            // 限制误差像素点在0.5以内
            (ABS(current_x - target_x) > 0.5) &&
            (ABS(current_y - target_y) > 0.5));
        // 将参数置零
        pid_change_zero(&screen_x);
        pid_change_zero(&screen_y);
    }
}
```

5. PID算法的核心实现

```c
float pid_calc(PID_Struct *pid, float target, float current)
{
    pid->last_error = pid->error;       // 更新之前误差
    pid->error      = target - current; // 计算新的误差

    pid->change_p = pid->error;                   // P
    pid->change_i += pid->error;                  // I
    pid->change_d = pid->error - pid->last_error; // D

    if (pid->change_i > pid->max_change_i) {
        pid->change_i = pid->max_change_i; // 限制积分I
    } else if (pid->change_i < -(pid->max_change_i)) {
        pid->change_i = -(pid->max_change_i); // 限制积分I
    }

    // PID输出
    pid->output = (pid->kp * pid->change_p) + (pid->ki * pid->change_i) + (pid->kd * pid->change_d);

    if (pid->output > pid->max_output) {
        pid->output = pid->max_output; // 限制输出
    } else if (pid->output < -(pid->max_output)) {
        pid->output = -(pid->max_output); // 限制输出
    }

    return pid->output;
}
```

6. 按键对应的功能说明

| 按键 | 短按功能          | 长按功能   |
| ---- | ----------------- | ---------- |
| 上键 | 选择上一项/参数加 | 连续加参数 |
| 下键 | 选择下一项/参数减 | 连续减参数 |
| 左键 | 返回上级菜单      | -          |
| 右键 | 进入下级菜单      | -          |
| 中键 | 启动/停止电机     | -          |

7. 大大小小的优化，官方的案例存在较多魔法数字和“Repeat Yourself”（尤其是在ui绘制中），因此我针对出现频率较高的，都单独抽象成了函数或者宏的表现形式，然后再去调用，由于改动较多，就不放出代码了。

TODO: 不过加入PID绘图之后，整个界面比较卡顿，因此我做成可选的了。（记得使用DMA去搬运SPI数据，降低MCU资源占用）


## 七、重要物料购买

| 序号 | 器件名称        | 型号参数                 | 商城器件编号 | 数量 |
| ---- | --------------- | ------------------------ | ------------ | ---- |
| 1    | 插件电阻        | 1K                       | C2903245     | 1    |
| 2    | 二极管          | D7G-T                    | C7435972     | 1    |
| 3    | USB连接器       | KH-TYPE-C-2P             | C2919656     | 1    |
| 4    | DIP-8芯片座     | -                        | C72124       | 1    |
| 5    | 电机驱动芯片    | BDR6126D                 | C7424658     | 1    |
| 6    | 拨动开关        | SK-12E12-G5              | C136720      | 1    |
| 7    | 电源灯          | 红色 XL-502SURD          | C2895492     | 1    |
| 8    | 按键            | ZX-QC66-8.5CJ            | C7470133     | 5    |
| 9    | 瓷片电容        | 100nF 104                | C254085      | 2    |
| 10   | 电解电容        | 22uF                     | C437651      | 2    |
| 11   | XH2.54接口      | 2.54mm-6P ZZ             | C5368779     | 2    |
| 12   | 直插排针        | PZ254V-11-06P            | C492405      | 1    |
| 13   | 直插排母        | 2.54-2*20                | C2977589     | 2    |
| 14   | 开发板          | LCKFB-TMX-MSPM0G3507     | C42378531    | 1    |
| 15   | 显示模块        | 1.9寸 ST7789 SPI 屏幕    | -            | 1    |
| 16   | TTL电机带编码器 | 6V 1:48减速比 霍尔传感器 | -            | 1    |
| 17   | 电机连接线      | XH双头一正一反;6P;150mm  | -            | 1    |
| 18   | 十字沉头螺丝钉  | M3×28[50只]              | -            | 2    |
| 19   | M3铜柱          | M3*18+4[10颗]            | -            | 4    |
| 20   | 螺母            | M3 [304材质/100只]       | -            | 6    |
| 21   | 2x20排针        | 2.54mm-2*20              | -            | 2    |

实际上由于购买的时候都是走量批发的，其实也不一定会按照上面的数量来，但我们只需要那么多就行了。

## 八、装配说明

在立创EDA中可以预览3D图的效果，以此来检查是否有焊接错误。

建议从低到高焊接，因此焊接焊接顺序为：

| 焊接顺序 | 器件名称     | 器件参数     | 位号             | 数量 | 备注                                                             |
| -------- | ------------ | ------------ | ---------------- | ---- | ---------------------------------------------------------------- |
| 1        | 插件电阻     | 1kΩ 1/4W     | R1               | 1    |                                                                  |
| 2        | 二极管       | D7G-T        | D1               | 1    |                                                                  |
| 3        | USB连接器    | TYPE-C-2P    | USB1             | 1    |                                                                  |
| 4        | DIP-8芯片座  | DIP-8        | -                | 1    | 焊接在电机驱动芯片处，注意方向                                   |
| 5        | 电机驱动芯片 | BDR6126D     | U4               | 1    | 先焊接DIP-8芯片座再装芯片到座子                                  |
| 6        | 拨动开关     | SK-12E12-G5  | SW1              | 1    |                                                                  |
| 7        | 电源灯       | 5mm 圆形红灯 | LED1             | 1    | 注意正负极，长正短负                                             |
| 8        | 按键         | 6×6×8.5 插件 | U7,U8,U9,U10,U11 | 4    |                                                                  |
| 9        | 瓷片电容     | 104          | C2,C4            | 2    | 不分正负极                                                       |
| 10       | 电解电容     | 22uF         | C1,C3            | 2    | 注意正负极                                                       |
| 11       | XH2.54接口   | 6P           | U5,U6            | 2    |                                                                  |
| 12       | 直插排针     | 2.54mm-6P    | H1               | 1    |                                                                  |
| 13       | 直插排母     | 2.54mm 2×20P | -                | 2    | 焊接在开发板的排针接口处                                         |
| 14       | 开发板       | 天猛星开发板 | -                | 1    | 先焊2×20P排母再安装开发板                                        |
| 15       | 显示模块     | 1.9 寸屏幕   | -                | 1    | 直接安装到开发板上                                               |
| 16       | 电机带编码器 | -            | -                | 1    | 电机转动轴向上，编码器接口向下，通过2根长螺纹柱和2个螺母固定电机 |
| 17       | M3隔离柱     | -            | -                | 4    | 安装在板子四个角落                                               |
