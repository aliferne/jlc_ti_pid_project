#include "bsp_servo.h"

int32_t Set_SG90_Angle(int32_t angle)
{
    float ServoAngle = 0;
    // 限制在0°到180°之间
    angle      = (angle > 180.0f) ? 180.0f : angle;
    ServoAngle = MIN_COUNT + (((float)angle / 180.0f) * RANGE);
    // 转成unsigned 以确保最低为0
    Set_Servo_Angle((uint32_t)ServoAngle, SG90_INST, GPIO_SG90_C0_IDX);
    return angle;
}

int32_t Set_SG90S_Angle(int32_t angle)
{
    float ServoAngle = 0;
    angle      = (angle > 360.0f) ? 360.0f : angle;
    ServoAngle = MIN_COUNT + (((float)angle / 360.0f) * RANGE);
    // 转成unsigned 以确保最低为0
    Set_Servo_Angle((uint32_t)ServoAngle, SG90S_INST, GPIO_SG90S_C0_IDX);
    return angle;
}

// int32_t Set_MG90_Angle(int32_t angle)
// {
//     float ServoAngle = 0;
//     angle      = (angle > 360.0f) ? 360.0f : angle;
//     ServoAngle = MIN_COUNT + (((float)angle / 360.0f) * RANGE);
//     // 转成unsigned 以确保最低为0
//     Set_Servo_Angle((uint32_t)ServoAngle, MG90_INST, GPIO_MG90_C0_IDX);
//     return angle;
// }

static void Set_Servo_Angle(uint32_t ServoAngle, GPTIMER_Regs *gptimer, DL_TIMER_CC_INDEX ccIndex)
{
    DL_TimerG_setCaptureCompareValue(gptimer, (uint32_t)(ServoAngle + 0.5f), ccIndex);
}
