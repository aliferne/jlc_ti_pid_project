> `@workspace` 项目说明文档（README）
> PID项目主要功能：
> 1. 提供UI可视化界面和用户操作的按键/手柄
> 2. 通过PID控制电机，并在TFT屏幕中提供可视化PID曲线，并允许用户通过按键/手柄调参
> 3. 预计将要提供基于天问asrpro芯片的语音识别（TTS）服务，用于控制电机
> 4. 提供可选的Q谈的UI选择框（基于PID算法），交互时会有眼前一亮的感觉（但不建议使用，会导致CPU过载，预计加上DMA减轻CPU负担）
> 5. 将移植LVGL库作为新的UI界面
> 使用的TI芯片为MSPM0G3507（立创天猛星开发板）
> 项目用于展示PID在电机控制上的应用
> 文档请尽可能详细，能够附上源代码则最好


# 基于MSPM0G3507的PID电机控制项目

## 项目概述

本项目是一个基于TI MSPM0G3507微控制器的PID电机控制系统，使用立创天猛星开发板进行开发。项目旨在展示PID算法在电机控制中的实际应用，提供了直观的用户界面和实时的PID控制效果可视化。

## 核心功能特性

### 1. 可视化用户界面
- **TFT彩屏显示**：提供直观的参数显示和控制界面
- **实时PID曲线**：动态显示PID控制效果和系统响应
- **多页面UI管理**：包括主页、定速页、定距页和参数设置页

### 2. 双模式PID控制
- **定速控制**：基于编码器反馈的速度闭环控制
- **定距控制**：基于位置反馈的角度定位控制
- **实时参数调节**：支持在线调整P、I、D参数

### 3. 多样化交互方式
- **五向按键**：上/下/左/右/中间按键，支持短按和长按
- **模拟摇杆**：提供模拟量输入控制
- **串口通信**：支持外部指令控制

### 4. 扩展功能（规划中）
- **语音识别**：基于天问ASRPro芯片的TTS服务
- **Q弹UI效果**：基于PID算法的动态交互效果
- **LVGL移植**：更丰富的图形界面库

## 硬件架构

### 主控芯片
- **型号**：TI MSPM0G3507
- **平台**：立创天猛星开发板
- **架构**：ARM Cortex-M0+

### 外设配置
- **电机驱动**：PWM控制，支持正反转
- **编码器**：正交编码器，用于位置和速度反馈
- **显示屏**：SPI接口TFT彩屏
- **按键**：5个独立按键 + 1个摇杆按键
- **串口**：UART调试和命令接口

## 软件架构

### 目录结构
```
empty/
├── app/                    # 应用层
│   ├── app_key_task.c     # 按键事件处理
│   ├── app_speed_pid.c    # 定速PID控制
│   ├── app_distance_pid.c # 定距PID控制
│   ├── app_ui.c           # 用户界面管理
│   ├── app_sys_mode.c     # 系统状态管理
│   └── app_uart_task.c    # 串口任务处理
├── hardware/               # 硬件抽象层
│   ├── hw_motor.c         # 电机驱动
│   ├── hw_encoder.c       # 编码器处理
│   ├── hw_lcd.c           # 屏幕驱动
│   └── hw_adc.c           # ADC采集
└── middle/                 # 中间件层
     ├── mid_pid.c          # PID算法实现
     ├── mid_button.c       # 按键驱动
     ├── mid_timer.c        # 定时器管理
     └── mid_uart.c         # 串口通信
```

## 核心算法实现

### PID控制器

#### 基本PID公式
```c
float pid_calc(PID_Struct *pid, float target, float current)
{
    float error = target - current;
    
    // 比例项
    float p_out = pid->kp * error;
    
    // 积分项
    pid->integral += error;
    // 积分限幅
    if (pid->integral > pid->maxIntegral) {
        pid->integral = pid->maxIntegral;
    } else if (pid->integral < -pid->maxIntegral) {
        pid->integral = -pid->maxIntegral;
    }
    float i_out = pid->ki * pid->integral;
    
    // 微分项
    float derivative = error - pid->lastError;
    float d_out = pid->kd * derivative;
    
    // PID输出
    float output = p_out + i_out + d_out;
    
    // 输出限幅
    if (output > pid->maxOutput) {
        output = pid->maxOutput;
    } else if (output < -pid->maxOutput) {
        output = -pid->maxOutput;
    }
    
    pid->lastError = error;
    return output;
}
```

#### 定速控制实现
```c
PID_Struct motor_speed_control(int target)
{
    int current_speed = get_encoder_count();
    float pid_output = pid_calc(get_speed_pid(), target, current_speed);
    
    // 将PID输出转换为PWM值
    set_motor_value((int16_t)pid_output);
    
    return *get_speed_pid();
}
```

#### 定距控制实现
```c
PID_Struct motor_distance_control(int target_angle)
{
    int current_angle = get_temp_encoder() * DEGREES_PER_PULSE;
    float pid_output = pid_calc(get_distance_pid(), target_angle, current_angle);
    
    set_motor_value((int16_t)pid_output);
    
    return *get_distance_pid();
}
```

## 用户界面设计

### 状态机管理
系统采用状态机模式管理不同的界面和功能状态：

```c
typedef enum {
    DEFAULT_PAGE,    // 主页
    PID_PAGE,        // 定速页
    DISTANCE_PAGE,   // 定距页
    SET_PAGE,        // 设置页
    PARAMETER_PAGE   // 参数调节页
} SystemPageShow;
```

### 按键事件处理
支持多种按键事件和长短按区分：

```c
void btn_up_cb(flex_button_t *btn)
{
    switch (btn->event) {
        case FLEX_BTN_PRESS_CLICK:      // 单击
            // 处理单击逻辑
            break;
        case FLEX_BTN_PRESS_LONG_HOLD:  // 长按保持
            // 触发长按加事件
            event_manager(&system_status, LONG_PRESS_ADD_START_EVENT);
            break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP: // 长按松开
            // 触发长按停止事件
            event_manager(&system_status, LONG_PRESS_END_EVENT);
            break;
    }
}
```

### 实时曲线显示
动态绘制PID控制效果曲线：

```c
uint16_t draw_motor_pid_curve(
    int window_start_x, int window_start_y,
    int window_w, int window_h, int curve_color,
    int background_color, short int rawValue)
{
    static uint16_t curve_x = 0;
    static short int last_value = 0;
    
    // 计算当前点的Y坐标
    uint16_t current_y = window_start_y + window_h/2 - 
                        (rawValue * window_h/4) / 100;
    
    // 绘制曲线点
    LCD_Draw_Point(window_start_x + curve_x, current_y, curve_color);
    
    // 更新X坐标
    curve_x = (curve_x + 1) % window_w;
    
    return curve_x;
}
```

## 硬件接口实现

### 电机控制
```c
void set_motor_value(int16_t value)
{
    // 限制PWM最大值
    restrict_pwm_max_value(&value);
    
    if (value >= 0) {
        // 正转
        set_fi((uint16_t)value);
        set_bi(0);
    } else {
        // 反转
        set_fi(0);
        set_bi((uint16_t)(-value));
    }
}
```

### 编码器处理
```c
void encoder_update(void)
{
    // 读取编码器状态
    bool a_state = DL_GPIO_readPins(ENCODER_A_PORT, ENCODER_A_PIN);
    bool b_state = DL_GPIO_readPins(ENCODER_B_PORT, ENCODER_B_PIN);
    
    // 判断旋转方向
    static bool last_a_state = false;
    
    if (a_state != last_a_state) {
        if (a_state == b_state) {
            motor_encoder.count++;
            motor_encoder.direction = ENCODER_FORWARD;
        } else {
            motor_encoder.count--;
            motor_encoder.direction = ENCODER_BACKWARD;
        }
    }
    
    last_a_state = a_state;
}
```

## 系统配置

### 时钟配置
- **系统时钟**：80MHz
- **PWM频率**：20KHz
- **定时器频率**：1KHz（用于任务调度）

### 中断优先级
- **编码器中断**：最高优先级
- **定时器中断**：高优先级
- **串口中断**：中等优先级

## 使用说明

### 基本操作流程

1. **系统启动**
   - 上电后自动初始化所有外设
   - 显示主页面，可选择定速或定距模式

2. **定速控制**
   - 选择定速模式进入定速页面
   - 按中间键启动/停止电机
   - 使用上下键调整参数
   - 实时查看速度曲线和PID参数

3. **定距控制**
   - 选择定距模式进入定距页面
   - 设置目标角度
   - 启动电机进行定位控制
   - 观察位置响应曲线

4. **参数调节**
   - 在控制页面按右键进入设置模式
   - 选择要调节的参数（P/I/D/Target）
   - 再次按右键进入调节模式
   - 使用上下键进行参数调节

### 按键功能说明



## 开发环境

### 工具链
- **IDE**：Code Composer Studio (CCS)
- **编译器**：TI Clang
- **调试器**：XDS110

### 依赖库
- **TI DriverLib**：底层驱动库
- **CMSIS DSP**：数字信号处理库
- **FlexButton**：按键处理库

## 项目亮点

### 1. 模块化设计
- 分层架构清晰，便于维护和扩展
- 硬件抽象层统一接口管理
- 应用层专注业务逻辑实现

### 2. 实时性能
- 1KHz的控制频率保证系统响应速度
- 中断驱动的事件处理机制
- 优化的PID计算算法

### 3. 用户体验
- 直观的图形界面显示
- 流畅的按键交互体验
- 实时的参数调节反馈

### 4. 扩展性
- 预留串口通信接口
- 支持多种控制模式
- 为语音控制和LVGL移植做好准备

## 后续开发计划

1. **语音控制集成**
   - 集成天问ASRPro语音识别模块
   - 实现语音指令控制电机

2. **UI效果优化**
   - 实现Q弹交互效果
   - 优化CPU占用率

3. **LVGL移植**
   - 移植LVGL图形库
   - 重新设计更丰富的用户界面

4. **通信协议扩展**
   - 完善串口通信协议
   - 支持更多外部控制指令

本项目为PID控制算法的学习和实践提供了完整的解决方案，展示了现代嵌入式系统开发的最佳实践。