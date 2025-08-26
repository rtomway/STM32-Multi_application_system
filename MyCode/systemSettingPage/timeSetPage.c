#include "timeSetPage.h"
#include "rtc.h"
#include "stdio.h"
#include "u8g2/u8g2.h"
#include "myU8g2/myU8g2.h"
#include "settingsMainPage.h"

SystemState systemSettings_timeSetPage_state = {
    .handle_confirm = systemSettingsTimeSetPage_confirm_handler,
    .handle_cancel = systemSettingsTimeSetPage_cancle_handler,
    .handle_next = systemSettingsTimeSetPage_next_handler,
    .handle_back = systemSettingsTimeSetPage_back_handler,
    .draw = systemSettingsTimeSetPage_draw,
};

// 时间编辑状态
typedef enum
{
    TIME_SET_EDIT,
    TIME_SET_HOUR,
    TIME_SET_MINUTE,
    TIME_SET_CONFIRM
} TimeSetState;

static TimeSetState current_time_set_state = TIME_SET_EDIT;
static RTC_TimeTypeDef new_time;

/*******************************************************************************************************************
 * @author xu
 *  * 函数名称：systemSettingsTimeSetPage_init
 *  * 功能描述：初始化时间设置页面
 ********************************************************************************************************************/
void systemSettingsTimeSetPage_init()
{
    // 读取当前RTC时间
    HAL_RTC_GetTime(&hrtc, &new_time, RTC_FORMAT_BIN);
    current_time_set_state = TIME_SET_EDIT;
}

/******************************************************************************
 * @author xu
 *  * 函数名称：systemSettingsTimeSetPage_draw
 *  * 功能描述：绘制时间设置页面
 *******************************************************************************/
void systemSettingsTimeSetPage_draw()
{
    u8g2_ClearBuffer(&u8g2);

    // 显示标题
    u8g2_SetFont(&u8g2, u8g2_font_helvB12_tr);
    u8g2_DrawStr(&u8g2, 0, 12, "Set Time");
    u8g2_DrawHLine(&u8g2, 0, 14, 128);

    // 显示时间
    u8g2_SetFont(&u8g2, u8g2_font_helvB24_tr);
    char time_str[20];
    snprintf(time_str, sizeof(time_str), "%02d:%02d", new_time.Hours, new_time.Minutes);
    u8g2_DrawStr(&u8g2, 10, 45, time_str);

    // 切换正常字体
    u8g2_SetFont(&u8g2, u8g2_font_helvB12_tr);

    // 显示当前选中的项目
    switch (current_time_set_state)
    {
    case TIME_SET_EDIT:
        u8g2_DrawStr(&u8g2, 0, 60, "Edit<");
        break;
    case TIME_SET_HOUR:
        u8g2_DrawStr(&u8g2, 22, 60, "^");
        break;
    case TIME_SET_MINUTE:
        u8g2_DrawStr(&u8g2, 66, 60, "^");
        break;
    case TIME_SET_CONFIRM:
        u8g2_DrawStr(&u8g2, 30, 60, "Save<");
        break;
    }

    u8g2_SendBuffer(&u8g2);
}

/******************************************************************************
 * @author xu
 *  * 函数名称：systemSettingsTimeSetPage_confirm_handler
 *  * 功能描述：将按键输入转换为时间设置状态
 *******************************************************************************/
void systemSettingsTimeSetPage_confirm_handler()
{
    switch (current_time_set_state)
    {
    case TIME_SET_EDIT:
        current_time_set_state = TIME_SET_HOUR;
        break;
    case TIME_SET_HOUR:
        current_time_set_state = TIME_SET_MINUTE;
        break;
    case TIME_SET_MINUTE:
        current_time_set_state = TIME_SET_CONFIRM;
        break;
    case TIME_SET_CONFIRM:
        // 保存时间到RTC
        HAL_RTC_SetTime(&hrtc, &new_time, RTC_FORMAT_BIN);
        systemSettingsTimeSetPage_init();
        break;
    }
}

/***********************************************************************************************************************
 * @author xu 
 *  * 函数名称：无
 *  * 功能描述：
***********************************************************************************************************************/
void systemSettingsTimeSetPage_cancle_handler()
{
    switch (current_time_set_state)
    {
    case TIME_SET_EDIT:
        systemSettings_transPage(&systemSettings_mainPage_state);
        break;
    case TIME_SET_HOUR:
        current_time_set_state = TIME_SET_EDIT;
        break;
    case TIME_SET_MINUTE:
        current_time_set_state = TIME_SET_HOUR;
        break;
    case TIME_SET_CONFIRM:
        current_time_set_state = TIME_SET_MINUTE;
        break;
    }
}

/***********************************************************************************************************************
 * @author xu 
 *  * 函数名称：无
 *  * 功能描述：
***********************************************************************************************************************/
void systemSettingsTimeSetPage_next_handler()
{
    switch (current_time_set_state)
    {
    case TIME_SET_HOUR:
        new_time.Hours = (new_time.Hours + 1) % 24;
        break;
    case TIME_SET_MINUTE:
        new_time.Minutes = (new_time.Minutes + 1) % 60;
        break;
    default:
        break;
    }
}

/***********************************************************************************************************************
 * @author xu 
 *  * 函数名称：无
 *  * 功能描述：
***********************************************************************************************************************/
void systemSettingsTimeSetPage_back_handler()
{
    switch (current_time_set_state)
    {
    case TIME_SET_HOUR:
        new_time.Hours = (new_time.Hours == 0) ? 23 : new_time.Hours - 1;
        break;
    case TIME_SET_MINUTE:
        new_time.Minutes = (new_time.Minutes == 0) ? 59 : new_time.Minutes - 1;
        break;
    default:
        break;
    }
}