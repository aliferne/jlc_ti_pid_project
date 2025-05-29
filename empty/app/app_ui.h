#ifndef _APP_UI_H_
#define _APP_UI_H_

#include "ti_msp_dl_config.h"

/**
 * @file app_ui.h
 * @brief 用户界面绘制与更新
 *
 * 本文件包含用户界面的绘制和更新函数，用于在LCD屏幕上显示和更新各种UI元素。
 * 包括静态UI的绘制、参数值的显示、选择框的绘制以及波形曲线的绘制等。
 */

#define screen_center_x (LCD_W / 2) // 屏幕中心X = 屏幕x/2
#define screen_center_y (LCD_H / 2) // 屏幕中心Y = 屏幕y/2

// SPEED选项框绘制参数（ui_home_page中调用）
#define PID_SPEED_SELECT_BOX_X_START   40 // 起始x坐标
#define PID_SPEED_SELECT_BOX_WIDTH     80 // 宽度
#define PID_SPEED_SELECT_BOX_Y_START   65 // 起始y坐标
#define PID_SPEED_SELECT_BOX_HEIGHT    80 // 高度
#define PID_SPEED_SELECT_LINE_LEN      10 // 外圈选择框线长度
#define PID_SPEED_SELECT_LINE_INTERVAL 5  // 外圈选择框线与选择框间隔

// DISTANCE选项框绘制参数（ui_home_page中调用）
#define PID_DISTANCE_SELECT_BOX_X_START   40 // 起始x坐标
#define PID_DISTANCE_SELECT_BOX_WIDTH     80 // 宽度
#define PID_DISTANCE_SELECT_BOX_Y_START   65 // 起始y坐标
#define PID_DISTANCE_SELECT_BOX_HEIGHT    80 // 高度
#define PID_DISTANCE_SELECT_LINE_LEN      10 // 外圈选择框线长度
#define PID_DISTANCE_SELECT_LINE_INTERVAL 5  // 外圈选择框线与选择框间隔

// 此处用于 `ui_select_page_show` 中
#define SPEED_PAGE    0
#define DISTANCE_PAGE 1
#define HOME_PAGE     2

// 此处用于 `ui_page_select_box()` 中
#define P_SELECTED                         0
#define I_SELECTED                         1
#define D_SELECTED                         2
#define TARGET_SELECTED                    3
#define ALL_CLEAN                          4

#define FONTSIZE                           16        // PID 字体大小
#define DEFAULT_PID_TITLE_Y_POSITION       87        // 默认Y轴起始位置
#define DEFAULT_CENTER_X_OFFSET            84        // 默认X轴偏移量
#define PID_NEXT_Y_START_POSITION          (60 + 44) // PID参数Y轴起始位置

#define SPEED_WAVEFORM_REDUCTION_FACTOR    2.3 // 定速波形衰减倍数
#define DISTANCE_WAVEFORM_REDUCTION_FACTOR 8.3 // 定距波形衰减倍数

// 该结构体用于PID控制
typedef struct {
    unsigned int start_x;
    unsigned int start_y;
    unsigned int end_x;
    unsigned int end_y;
    unsigned int center;
} TXT_OBJECT;

/**
 * @brief 绘制首页静态UI
 *
 * 该函数用于绘制首页的静态用户界面元素。
 */
void ui_home_page(void);

/**
 * @brief 绘制首页选择框
 *
 * @param mode 选择框的模式
 *
 * 该函数用于在首页上绘制选择框，根据不同的模式显示不同的选择框。
 */
void ui_home_page_select(int mode);

/**
 * @brief 绘制定速页静态UI
 *
 * 该函数用于绘制定速页的静态用户界面元素。
 */
void ui_speed_page(void);

/**
 * @brief 绘制定速页参数值的变化
 *
 * @param p P参数值
 * @param i I参数值
 * @param d D参数值
 * @param speed 当前速度
 * @param target 目标速度
 * @param quick_update 是否快速更新
 *
 * 该函数用于在定速页上绘制参数值的变化。
 */
void ui_speed_page_value_set(float p, float i, float d, int speed, int target, int quick_update);

/**
 * @brief 绘制选择框
 *
 * @param mode 选择框的模式
 *
 * 该函数用于在定速页上绘制选择框，根据不同的模式显示不同的选择框。
 */
void ui_page_select_box(int mode);

/**
 * @brief 绘制定距页静态UI
 *
 * 该函数用于绘制定距页的静态用户界面元素。
 */
void ui_distance_page(void);

/**
 * @brief 绘制定距页参数值的变化
 *
 * @param p P参数值
 * @param i I参数值
 * @param d D参数值
 * @param distance 当前距离
 * @param target 目标距离
 * @param quick_update 是否快速更新
 *
 * 该函数用于在定距页上绘制参数值的变化。
 */
void ui_distance_page_value_set(float p, float i, float d, int distance, int target, int quick_update);

/**
 * @brief 根据选择确定显示哪一个页面
 *
 * @param page 要显示的页面编号
 *
 * 该函数用于根据用户的选择显示相应的页面。
 */
void ui_select_page_show(int page);

/**
 * @brief 参数选择框加粗，即选中框
 *
 * @param mode 参数选择框的模式
 *
 * 该函数用于将参数选择框加粗，表示该框已被选中。
 *
 * @note 请使用文件中已定义好的宏： `SPEED_PAGE`,`DISTANCE_PAGE`,`HOME_PAGE`
 */
void ui_parameter_select_box_bold(int mode);

/**
 * @brief 绘制速度曲线
 *
 * @param window_start_x 曲线窗口的起始X坐标
 * @param window_start_y 曲线窗口的起始Y坐标
 * @param window_w 曲线窗口的宽度
 * @param window_h 曲线窗口的高度
 * @param curve_color 曲线的颜色
 * @param background_color 背景颜色
 * @param rawValue 原始数据值
 * @return 绘制结果状态
 *
 * 该函数用于在指定窗口内绘制速度曲线。
 */
uint16_t draw_speed_curve(int window_start_x, int window_start_y, int window_w, int window_h, int curve_color, int background_color, short int rawValue);

/**
 * @brief 绘制距离曲线
 *
 * @param window_start_x 曲线窗口的起始X坐标
 * @param window_start_y 曲线窗口的起始Y坐标
 * @param window_w 曲线窗口的宽度
 * @param window_h 曲线窗口的高度
 * @param curve_color 曲线的颜色
 * @param background_color 背景颜色
 * @param rawValue 原始数据值
 * @return 绘制结果状态
 *
 * 该函数用于在指定窗口内绘制距离曲线。
 */
uint16_t draw_distance_curve(int window_start_x, int window_start_y, int window_w, int window_h, int curve_color, int background_color, short int rawValue);

/**
 * @brief 绘制速度曲线界面
 *
 * 该函数用于在用户界面上绘制速度曲线，包括当前编码器数值曲线和目标速度的波形点。
 * 在绘制过程中，禁止任务调度以避免界面刷新时的闪烁或异常。
 *
 * @param None
 * @return None
 */
void ui_speed_curve();

/**
 * @brief 绘制距离曲线界面
 *
 * 该函数用于在用户界面上绘制距离曲线，包括当前编码器数值曲线和目标距离的波形点。
 * 在绘制过程中，禁止任务调度以避免界面刷新时的闪烁或异常。
 *
 * @param None
 * @return None
 */
void ui_distance_curve(void);

#endif