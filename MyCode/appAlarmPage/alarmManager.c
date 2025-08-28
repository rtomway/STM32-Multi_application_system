#include "alarmManager.h"
#include "FreeRTOS.h"
#include "logicTask/logicTask.h"
#include "string.h"
#include "configStorage/configStorage.h"

static Alarm alarms[MAX_ALARMS];
static uint8_t alarm_count = 0;

static Config* systemConfig = NULL;

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarm_init
 *  * 功能描述：闹钟的初始化函数，将闹钟数组清空，并将闹钟计数器清零。
 ***********************************************************************************************************************/
void alarm_init(void)
{
    systemConfig=ConfigStorage_GetConfig();
    for (int i = 0; i < systemConfig->config_alarm_count; i++)
    {
            alarm_count++;
            alarms[i]=systemConfig->config_alarm[i];
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarm_add
 *  * 功能描述：添加闹钟函数，将闹钟添加到闹钟数组中，并将闹钟计数器加一。
 *  * 输入参数：hour {}
 *  * 输入参数：minute {type}
 *  * 返 回 值：true
 *  * 返 回 值：false
 ***********************************************************************************************************************/
bool alarm_add(uint8_t hour, uint8_t minute)
{
    if (alarm_count >= MAX_ALARMS || hour >= 24 || minute >= 60)
    {
        return false;
    }

    // 检查闹钟重复
    if (alarm_exists(hour, minute))
    {
        return false;
    }

    alarms[alarm_count].hour = hour;
    alarms[alarm_count].minute = minute;

    systemConfig->config_alarm[alarm_count]=alarms[alarm_count];
    systemConfig->config_alarm_count++;
    ConfigStorage_Save();

    alarm_count++;

    return rtc_updateAlarm();
}
/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarm_remove
 *  * 功能描述：删除闹钟函数，将闹钟从闹钟数组中删除，并将闹钟计数器减一。
 *  * 输入参数：index {type}
 *  * 返 回 值：true
 *  * 返 回 值：false
 ***********************************************************************************************************************/
bool alarm_remove(uint8_t index)
{
    if (index >= alarm_count)
        return false;

    // 获取当前RTC闹钟设置
    RTC_AlarmTypeDef active_alarm;
    bool need_update = false;

    if (HAL_RTC_GetAlarm(&hrtc, &active_alarm, RTC_ALARM_A, RTC_FORMAT_BIN) == HAL_OK)
    {
        need_update = (alarms[index].hour == active_alarm.AlarmTime.Hours &&
                       alarms[index].minute == active_alarm.AlarmTime.Minutes);
    }

    // 删除操作
    for (uint8_t i = index; i < alarm_count - 1; i++)
    {
        alarms[i] = alarms[i + 1];
    }
    alarm_count--;

    // 更新持久化存储
    systemConfig->config_alarm_count = alarm_count;
    for (uint8_t i = 0; i < alarm_count; i++)
    {
        systemConfig->config_alarm[i] = alarms[i];
    }
    for (uint8_t i = alarm_count; i < systemConfig->config_alarm_count; i++)
    {
        memset(&systemConfig->config_alarm[i], 0, sizeof(systemConfig->config_alarm[0]));
    }
    ConfigStorage_Save();

    // 必要时更新RTC
    return (need_update) ? rtc_updateAlarm() : true;
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarm_exists
 *  * 功能描述：检查闹钟是否重复函数，检查闹钟数组中是否有重复的闹钟。
 *  * 输入参数：hour {type}
 *  * 输入参数：minute {type}
 *  * 返 回 值：true
 *  * 返 回 值：false
 ***********************************************************************************************************************/
bool alarm_exists(uint8_t hour, uint8_t minute)
{
    for (uint8_t i = 0; i < alarm_count; i++)
    {
        if (alarms[i].hour == hour && alarms[i].minute == minute)
        {
            return true; // 找到重复的闹钟
        }
    }
    return false;
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarm_get_count
 *  * 功能描述：返回闹钟计数器函数，返回闹钟数量。
 *  * 返 回 值：uint8_t
 ***********************************************************************************************************************/
uint8_t alarm_get_count(void)
{
    return alarm_count;
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarm_get_all
 *  * 功能描述：获取所有闹钟函数，返回闹钟数组的指针。
 *  * 返 回 值：Alarm*
 ***********************************************************************************************************************/
Alarm *alarm_get_all(void)
{
    return alarms;
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：rtc_alarmTimeSet
 *  * 功能描述：设置rtc闹钟的时间函数，设置rtc的闹钟时间。
 *  * 输入参数：hour {type}
 *  * 输入参数：minute {type}
 ***********************************************************************************************************************/
bool rtc_alarmTimeSet(uint8_t hour, uint8_t minute)
{
    RTC_AlarmTypeDef sAlarm = {0};
    HAL_StatusTypeDef hal_status;

    // 参数有效性检查
    if (hour > 23 || minute > 59)
    {
        return false;
    }

    // 设置闹钟时间（24小时制）
    sAlarm.AlarmTime.Hours = hour;
    sAlarm.AlarmTime.Minutes = minute;
    sAlarm.AlarmTime.Seconds = 0;

    /* STM32F1xx特有配置 */
    sAlarm.Alarm = RTC_ALARM_A;

    // 必须先取消之前的闹钟
    HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

    // 设置闹钟并启用中断
    hal_status = HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);

    if (hal_status != HAL_OK)
    {
        return false;
    }

    return true;
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：rtc_updateAlarm
 *  * 功能描述：更新rtc闹钟时间函数，更新rtc的闹钟时间。
 *  * 返 回 值：true
 *  * 返 回 值：false
 ***********************************************************************************************************************/
bool rtc_updateAlarm(void)
{
    if (alarm_count == 0)
    {
        HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);
        return false;
    }

    // 获取当前时间
    RTC_TimeTypeDef now;
    HAL_RTC_GetTime(&hrtc, &now, RTC_FORMAT_BIN);

    // 查找下一个要触发的闹钟
    Alarm *next_alarm = NULL;
    uint32_t min_diff = UINT32_MAX;

    for (uint8_t i = 0; i < alarm_count; i++)
    {
        uint32_t alarm_min = alarms[i].hour * 60 + alarms[i].minute;
        uint32_t now_min = now.Hours * 60 + now.Minutes;
        uint32_t diff = (alarm_min > now_min) ? (alarm_min - now_min) : (24 * 60 - now_min + alarm_min);

        if (diff < min_diff)
        {
            min_diff = diff;
            next_alarm = &alarms[i];
        }
    }

    // 设置RTC闹钟
    return (next_alarm) ? rtc_alarmTimeSet(next_alarm->hour, next_alarm->minute) : false;
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：HAL_RTC_AlarmAEventCallback
 *  * 功能描述：RTC闹钟事件回调函数，当rtc的闹钟事件发生时，调用该函数。
 *  * 输入参数：hrtc {type}
 ***********************************************************************************************************************/
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xTaskNotifyFromISR(
        logicTaskHandle, 
        (1 << 0),
        eSetBits,
        &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}