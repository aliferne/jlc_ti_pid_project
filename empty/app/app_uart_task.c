#include "app_uart_task.h"
#include "app_ui.h"
#include "app_sys_mode.h"
#include "app_speed_pid.h"
#include "app_distance_pid.h"
#include "hw_encoder.h"
#include "hw_motor.h"
#include "mid_uart.h"
#include "string.h"

// TODO: 此部分较难完成，暂时放弃。

static char *command; // 缓存接收到的命令

static const ctrl_motor_state motor_state_cmd = {
    .motor_on   = "Motor On\n",
    .motor_off  = "Motor Off\n",
    .motor_stop = "Motor Stop\n",
};

static const motor_cmd speed_plus_30 = {
    .cmd   = "Speed 30\n",
    .value = 30,
};

static const motor_cmd speed_minus_30 = {
    .cmd   = "Speed -30\n",
    .value = -30,
};

static const motor_cmd speed_plus_60 = {
    .cmd   = "Speed 60\n",
    .value = 60,
};

static const motor_cmd speed_minus_60 = {
    .cmd   = "Speed -60\n",
    .value = -60,
};

static const motor_cmd speed_plus_90 = {
    .cmd   = "Speed 90\n",
    .value = 90,
};

static const motor_cmd speed_minus_90 = {
    .cmd   = "Speed -90\n",
    .value = -90,
};

static const ctrl_motor_speed motor_speed_cmd = {
    .plus_30  = &speed_plus_30,
    .minus_30 = &speed_minus_30,
    .plus_60  = &speed_plus_60,
    .minus_60 = &speed_minus_60,
    .plus_90  = &speed_plus_90,
    .minus_90 = &speed_minus_90,
};

void recv_cmd()
{
    // BUG: string.h 中的 strcmp并不会按照我的预期来，且uart有一定程度的数据丢失
    // 我打算自己实现一个了。
    // BUG: memcpy 和 for 循环均拷贝不正常，放弃缓存架构
    // memcpy(command, str, MAX_CMD_LENGTH);

    cmd_uart_clear_recv_data();         // 先清空缓存
    command = cmd_uart_get_recv_data(); // 赋值给命令
}

void send_recv_data()
{
    debug_uart_send_string(command);
}

bool compare_command(const char *recv, const motor_cmd *preset_cmd)
{
    uint8_t count = 0, recv_len = strlen(recv);
    // 接收到的指令有可能小于于预设指令，
    // 所以以接收到的指令长度为实际判断指标
    for (uint8_t i = 0; i < recv_len; i++) {
        if (recv[i] == preset_cmd->cmd[i]) {
            count++;
        } else {
            // 比如 recv = "Moor On", preset_cmd = "Motor On"
            if (recv[i] == preset_cmd->cmd[i + 1]) {
                count++;
            }
        }
    }
    return 0; // TODO
}
