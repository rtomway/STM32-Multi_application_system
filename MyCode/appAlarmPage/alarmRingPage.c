#include "alarmRingPage.h"
#include "alarmMainPage.h"
#include "mainPage/mainPage.h"
#include "stdio.h"
#include "rtc.h"

// 字体设置
#define TIME_FONT u8g2_font_logisoso26_tn

// 时间显示位置
typedef struct
{
    int x[4];
    int y;
} TimePosition;

TimePosition alarm_time_pos = {
    .x = {15, 45, 75, 105},
    .y = 35};

SystemState alarm_ringPage_state = {
    .handle_confirm = alarmRingPage_confirm_handler,
    .handle_cancel = alarmRingPage_cancle_handler,
    .handle_next = alarmRingPage_next_handler,
    .handle_back = alarmRingPage_back_handler,
    .draw = alarmRingPage_draw,
};

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmRingPage_init
 *  * 功能描述：闹钟响铃页面初始化
 ***********************************************************************************************************************/
void alarmRingPage_init()
{

}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmRingPage_draw
 *  * 功能描述：闹钟响铃页面绘制
 ***********************************************************************************************************************/
void alarmRingPage_draw()
{
    char time_str[6];
    RTC_AlarmTypeDef alarm_time;

    // 获取闹钟时间
    HAL_RTC_GetAlarm(&hrtc, &alarm_time, RTC_ALARM_A, RTC_FORMAT_BIN);

    u8g2_ClearBuffer(&u8g2);

    // 显示闹钟时间
    u8g2_SetFont(&u8g2, TIME_FONT);
    snprintf(time_str, sizeof(time_str), "%02d:%02d", alarm_time.AlarmTime.Hours, alarm_time.AlarmTime.Minutes);
    int time_width = u8g2_GetStrWidth(&u8g2, time_str);
    int time_x = (128 - time_width) / 2;
    u8g2_DrawStr(&u8g2, time_x, alarm_time_pos.y, time_str);

    u8g2_SetFont(&u8g2, u8g2_font_helvB10_tr);
    u8g2_DrawStr(&u8g2, 15, 55, "alarm ringing");

    u8g2_SendBuffer(&u8g2);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmRingPage_confirm_handler
 *  * 功能描述：闹钟界面按键处理 恢复界面
 ***********************************************************************************************************************/
void alarmRingPage_confirm_handler()
{
    systemState_rollBackOneStep();
    alarm_transPage(&alarm_mainPage_state);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmRingPage_cancle_handler
 *  * 功能描述：闹钟界面按键处理 恢复界面
 ***********************************************************************************************************************/
void alarmRingPage_cancle_handler()
{
    systemState_rollBackOneStep();
    alarm_transPage(&alarm_mainPage_state);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmRingPage_next_handler
 *  * 功能描述：闹钟界面按键处理    恢复界面
 ***********************************************************************************************************************/
void alarmRingPage_next_handler()
{
    systemState_rollBackOneStep();
    alarm_transPage(&alarm_mainPage_state);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmRingPage_back_handler
 *  * 功能描述：闹钟界面按键处理    恢复界面
 ***********************************************************************************************************************/
void alarmRingPage_back_handler()
{
    systemState_rollBackOneStep();
    alarm_transPage(&alarm_mainPage_state);
}