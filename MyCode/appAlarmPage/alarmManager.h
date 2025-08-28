#ifndef ALARMMANAGER_H
#define ALARMMANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include "rtc.h"

#define MAX_ALARMS 3

// 闹钟结构体
typedef struct
{
    uint8_t hour;
    uint8_t minute;
} Alarm;

// 闹钟管理函数
void alarm_init(void);
bool alarm_add(uint8_t hour, uint8_t minute);
bool alarm_remove(uint8_t index);
uint8_t alarm_get_count(void);
Alarm *alarm_get_all(void);
bool alarm_exists(uint8_t hour, uint8_t minute);

//rtc配置函数
static bool rtc_alarmTimeSet(uint8_t hour, uint8_t minute);
static bool rtc_updateAlarm(void);
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc);

#endif // ALARMMANAGER_H