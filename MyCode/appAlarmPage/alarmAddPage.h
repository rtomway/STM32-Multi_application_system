#ifndef ALARMADDPAGE_H
#define ALARMADDPAGE_H

#include "systemStatus/systemStatus.h"
#include "appAlarmPage.h"
#include "menuList/menuList.h"

extern SystemState alarm_addPage_state;

typedef enum
{
    ADD_PAGE_SET_HOUR,
    ADD_PAGE_SET_MINUTE,
    ADD_PAGE_SET_ENABLE,
    ADD_PAGE_CONFIRM
} AddPageState;

void alarmAddPage_init();

void alarmAddPage_draw();
void alarmAddPage_confirm_handler();
void alarmAddPage_cancle_handler();
void alarmAddPage_next_handler();
void alarmAddPage_back_handler();

#endif // ALARMADDPAGE_H