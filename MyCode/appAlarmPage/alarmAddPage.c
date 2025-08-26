#include "alarmAddPage.h"
#include "alarmMainPage.h"
#include "u8g2/u8g2.h"
#include "myU8g2/myU8g2.h"
#include "alarmManager.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "timers.h"
#include "rtc.h"

SystemState alarm_addPage_state = {
    .handle_confirm = alarmAddPage_confirm_handler,
    .handle_cancel = alarmAddPage_cancle_handler,
    .handle_next = alarmAddPage_next_handler,
    .handle_back = alarmAddPage_back_handler,
    .draw = alarmAddPage_draw,
};

static AddPageState current_add_state = ADD_PAGE_SET_HOUR;
static uint8_t new_alarm_hour = 0;
static uint8_t new_alarm_minute = 0;
static bool show_error = false; 
static TimerHandle_t error_timer = NULL;

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：error_timer_callback
 *  * 功能描述：错误警告定时器回调函数
 *  * 输入参数：xTimer {type}
 ***********************************************************************************************************************/
static void error_timer_callback(TimerHandle_t xTimer)
{
    show_error = false;
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmAddPage_init
 *  * 功能描述：初始化闹钟添加页面
 ***********************************************************************************************************************/
void alarmAddPage_init()
{
    current_add_state = ADD_PAGE_SET_HOUR;
    show_error = false;

    // 错误警告定时器
    if (error_timer == NULL)
    {
        error_timer = xTimerCreate("ErrorTimer",pdMS_TO_TICKS(3000),pdFALSE,(void *)0,error_timer_callback );
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmAddPage_draw
 *  * 功能描述：闹钟添加页面绘制函数
 ***********************************************************************************************************************/
void alarmAddPage_draw()
{
    u8g2_ClearBuffer(&u8g2);

    // 显示标题
    u8g2_DrawStr(&u8g2, 0, 12, "Add Alarm");
    u8g2_DrawHLine(&u8g2, 0, 14, 128);

    if (show_error)
    {
        u8g2_DrawStr(&u8g2, 0, 30, "Alarm already exists!");
        u8g2_DrawStr(&u8g2, 0, 45, "Please set different time");
    }
    else
    {
        char time_str[20];
        snprintf(time_str, sizeof(time_str), "Time: %02d:%02d", new_alarm_hour, new_alarm_minute);
        u8g2_DrawStr(&u8g2, 0, 30, time_str);

        switch (current_add_state)
        {
        case ADD_PAGE_SET_HOUR:
            u8g2_DrawStr(&u8g2, 54, 45, "^");
            break;
        case ADD_PAGE_SET_MINUTE:
            u8g2_DrawStr(&u8g2, 77, 45, "^");
            break;
        case ADD_PAGE_CONFIRM:
            u8g2_DrawStr(&u8g2, 0, 45, "Save<");
            break;
        }
    }

    char count_str[20];
    snprintf(count_str, sizeof(count_str), "Total: %d/%d", alarm_get_count(), MAX_ALARMS);
    u8g2_DrawStr(&u8g2, 0, 60, count_str);

    u8g2_SendBuffer(&u8g2);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmAddPage_confirm_handler
 *  * 功能描述：确认按钮事件处理函数,负责添加闹钟的下一步编辑或保存
 ***********************************************************************************************************************/
void alarmAddPage_confirm_handler()
{
    if (show_error)
    {
        xTimerStop(error_timer, 0);
        show_error = false;
        return;
    }

    switch (current_add_state)
    {
    case ADD_PAGE_SET_HOUR:
        current_add_state = ADD_PAGE_SET_MINUTE;
        break;
    case ADD_PAGE_SET_MINUTE:
        current_add_state = ADD_PAGE_CONFIRM;
        break;
    case ADD_PAGE_CONFIRM:
        if (alarm_add(new_alarm_hour, new_alarm_minute))
        {
            alarmAddPage_init();
        }
        else
        {
            show_error = true;
            xTimerStart(error_timer, 0);
        }
        break;
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmAddPage_cancle_handler
 *  * 功能描述：取消按钮事件处理函数,负责返回上一页或取消编辑
 ***********************************************************************************************************************/
void alarmAddPage_cancle_handler()
{
    if (show_error)
    {
        xTimerStop(error_timer, 0);
        show_error = false;
        return;
    }

    switch (current_add_state)
    {
    case ADD_PAGE_SET_HOUR:
        alarm_transPage(&alarm_mainPage_state);
        break;
    case ADD_PAGE_SET_MINUTE:
        current_add_state = ADD_PAGE_SET_HOUR;
        break;
    case ADD_PAGE_CONFIRM:
        current_add_state = ADD_PAGE_SET_MINUTE;
        break;
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmAddPage_next_handler
 *  * 功能描述：负责闹钟添加页面的闹钟增大时间数字
 ***********************************************************************************************************************/
void alarmAddPage_next_handler()
{
    switch (current_add_state)
    {
    case ADD_PAGE_SET_HOUR:
        new_alarm_hour = (new_alarm_hour + 1) % 24;
        break;
    case ADD_PAGE_SET_MINUTE:
        new_alarm_minute = (new_alarm_minute + 1) % 60;
        break;
    case ADD_PAGE_CONFIRM:
        break;
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmAddPage_back_handler
 *  * 功能描述：负责闹钟添加页面的闹钟减小时间数字
 ***********************************************************************************************************************/
void alarmAddPage_back_handler()
{
    switch (current_add_state)
    {
    case ADD_PAGE_SET_HOUR:
        new_alarm_hour = (new_alarm_hour == 0) ? 23 : new_alarm_hour - 1;
        break;
    case ADD_PAGE_SET_MINUTE:
        new_alarm_minute = (new_alarm_minute == 0) ? 59 : new_alarm_minute - 1;
        break;
    case ADD_PAGE_CONFIRM:
        break;
    }
}