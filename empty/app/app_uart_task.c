#include "app_uart_task.h"
#include "app_ui.h"
#include "app_sys_mode.h"
#include "app_speed_pid.h"
#include "app_distance_pid.h"
#include "hw_encoder.h"
#include "hw_motor.h"

static const CTRL_MOTOR_STATE motor_state_command = {
    .motor_on   = "Motor On",
    .motor_off  = "Motor Off",
    .motor_stop = "Motor Stop",
};

static const CTRL_MOTOR_SPEED motor_speed_command = {
    .speed_plus_30  = "Speed 30",
    .speed_minus_30 = "Speed -30",
    .speed_plus_60  = "Speed 60",
    .speed_minus_60 = "Speed -60",
    .speed_plus_90  = "Speed 90",
    .speed_minus_90 = "Speed -90",
};
