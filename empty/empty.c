#include "ti_msp_dl_config.h"
#include "./link/pwm/bsp_servo.h"
// #include "./link/btn/btn.h"
#include "stdio.h"

#define delay_ms(x) (delay_cycles(CPUCLK_FREQ / 1000 * (x)))

FILE __stdout;
// 定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}
// // printf函数重定义
int fputc(int ch, FILE *stream)
{
    // 当串口0忙的时候等待，不忙的时候再发送传进来的字符
    while (DL_UART_isBusy(UART_0_INST) == true);
    DL_UART_Main_transmitData(UART_0_INST, ch);
    return ch;
}

// idx表示当前控制的舵机，0表示PB8，1表示PA12
uint32_t idx = 0;
// 舵机角度（其实在这里应该算是速度了，360°Ver不控制角度）
int32_t angle = 0;
// 按键处理函数
void Key_Handler();
// 舵机控制函数
void Set_Servo(uint32_t i);

int main(void)
{
    SYSCFG_DL_init();

    while (1) {
        // 需要确保angle实时更新，所以Set_Servo放在这里
        Set_Servo(idx);
        delay_ms(10);
        Key_Handler(); // 检测按键并执行对应操作
        printf("\nAngle: %d\n", angle);
    }
}

void Set_Servo_Speed_Up()
{
    if (angle >= 360) {
        angle = 360;
    }
    // 每次+2，且确保不超过360
    angle += 2;
}

void Set_Servo_Speed_Down()
{
    if (angle <= 0) {
        angle = 0;
    }
    // 每次-2，且确保不小于0
    angle -= 2;
}

// 设置舵机
void Set_Servo(uint32_t i)
{
    // angle = 180时，舵机不转
    if (i == 0) {
        // 为0时说明需要开启PB8的舵机（同时关闭PA12）
        Set_SG90S_Angle(180, SG90_INST, GPIO_SG90_C0_IDX);
        Set_SG90S_Angle(angle, SG90S_INST, GPIO_SG90S_C0_IDX);
    } else if (i == 1) {
        // 为1时说明需要开启PA12的舵机（同时关闭PB8）
        Set_SG90S_Angle(180, SG90S_INST, GPIO_SG90S_C0_IDX);
        Set_SG90S_Angle(angle, SG90_INST, GPIO_SG90_C0_IDX);
    }
}

void Key_Handler()
{
    // 首先检测是否按下（输入为低电平）
    if (!DL_GPIO_readPins(KEY_DOWN_PORT, KEY_DOWN_PIN)) {
        delay_ms(50);   // 消抖
        // 再次检测是否松开
        if (DL_GPIO_readPins(KEY_DOWN_PORT, KEY_DOWN_PIN)) {
            Set_Servo_Speed_Down(); // 减速
            printf("KEY_DOWN pressed\n");
        }
    }
    if (!DL_GPIO_readPins(KEY_UP_PORT, KEY_UP_PIN)) {
        delay_ms(50);
        if (DL_GPIO_readPins(KEY_UP_PORT, KEY_UP_PIN)) {
            Set_Servo_Speed_Up(); // 加速
            printf("KEY_UP pressed\n");
        }
    }
    if (!DL_GPIO_readPins(KEY_LEFT_PORT, KEY_LEFT_PIN)) {
        delay_ms(50);
        if (DL_GPIO_readPins(KEY_LEFT_PORT, KEY_LEFT_PIN)) {
            idx = 0; // 为0时说明需要开启PB8的舵机（同时关闭PA12）
            printf("KEY_LEFT pressed\n");
        }
    }
    if (!DL_GPIO_readPins(KEY_MID_PORT, KEY_MID_PIN)) {
        delay_ms(50);
        if (DL_GPIO_readPins(KEY_MID_PORT, KEY_MID_PIN)) {
            // angle = 180时，舵机不转
            angle = 180;
            printf("KEY_MID pressed\n");
        }
    }
    if (!DL_GPIO_readPins(KEY_RIGHT_PORT, KEY_RIGHT_PIN)) {
        delay_ms(50);
        if (DL_GPIO_readPins(KEY_RIGHT_PORT, KEY_RIGHT_PIN)) {
            idx = 1; // 为1时说明需要开启PA12的舵机（同时关闭PB8）
            printf("KEY_RIGHT pressed\n");
        }
    }
}
