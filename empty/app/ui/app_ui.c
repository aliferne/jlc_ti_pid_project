#include "app_ui.h"

// 根据选择确定显示哪一个页面
void ui_home_select_page_show(int page)
{
    switch (page) {
        case HOME_PAGE:
            ui_home_page();
            break;
        case SPEED_PAGE:
            ui_speed_page();
            // 绘制定速页面的各项参数
            ui_speed_page_value_set(
                get_speed_pid()->kp, get_speed_pid()->ki, get_speed_pid()->kd,
                get_encoder_count(), get_speed_pid()->target, 0);
            break;
        case DISTANCE_PAGE:
            ui_distance_page();
            // 绘制定距页面的各项参数
            int current_angle = get_temp_encoder() * DEGREES_PER_PULSE;
            ui_distance_page_value_set(
                get_distance_pid()->kp, get_distance_pid()->ki, get_distance_pid()->kd,
                current_angle, get_distance_pid()->target, 0);
            break;
        case MANUAL_PAGE:
            ui_manual_page();
            break;
        case SETTINGS_PAGE:
            ui_settings_page_show();
            break;
        default:
            break;
    }
}
