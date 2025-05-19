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

typedef int32_t (*Set_Servo_Func)(int32_t);

Set_Servo_Func Servo_Angle_Func[] = {
    Set_SG90S_Angle,
    Set_SG90_Angle,
};

uint32_t idx  = 1;
int32_t angle = 0;
void Key_Handler();

int main(void)
{
    SYSCFG_DL_init();

    while (1) {
        Servo_Angle_Func[idx](angle);
        delay_ms(10);
        Key_Handler();
        printf("\nAngle: %d\n", angle);
    }
}

void Set_Servo_Speed_Up()
{
    if (angle >= 360) {
        angle = 360;
    }
    angle += 2;
}

void Set_Servo_Speed_Down()
{
    if (angle <= 0) {
        angle = 0;
    }
    angle -= 2;
}

// 设置舵机
void Set_Servo(uint32_t i)
{
    // 将传入的参数i赋值给全局变量idx
    idx = i;
}

void Key_Handler()
{

    if (!DL_GPIO_readPins(KEY_DOWN_PORT, KEY_DOWN_PIN)) {
        delay_ms(50);
        if (DL_GPIO_readPins(KEY_DOWN_PORT, KEY_DOWN_PIN)) {
            Set_Servo_Speed_Down();
            printf("KEY_DOWN pressed\n");
        }
    }
    if (!DL_GPIO_readPins(KEY_UP_PORT, KEY_UP_PIN)) {
        delay_ms(50);
        if (DL_GPIO_readPins(KEY_UP_PORT, KEY_UP_PIN)) {
            Set_Servo_Speed_Up();
            printf("KEY_UP pressed\n");
        }
    }
    if (!DL_GPIO_readPins(KEY_LEFT_PORT, KEY_LEFT_PIN)) {
        delay_ms(50);
        if (DL_GPIO_readPins(KEY_LEFT_PORT, KEY_LEFT_PIN)) {
            Set_Servo(0);
            printf("KEY_LEFT pressed\n");
        }
    }
    if (!DL_GPIO_readPins(KEY_MID_PORT, KEY_MID_PIN)) {
        delay_ms(50);
        if (DL_GPIO_readPins(KEY_MID_PORT, KEY_MID_PIN)) {
            angle = 90;
            printf("KEY_MID pressed\n");
        }
    }
    if (!DL_GPIO_readPins(KEY_RIGHT_PORT, KEY_RIGHT_PIN)) {
        delay_ms(50);
        if (DL_GPIO_readPins(KEY_RIGHT_PORT, KEY_RIGHT_PIN)) {
            Set_Servo(1);
            printf("KEY_RIGHT pressed\n");
        }
    }
}
