#ifndef ALARMCANCELPAGE_H
#define ALARMCANCELPAGE_H

#include "systemStatus/systemStatus.h"
#include "appAlarmPage.h"
#include "menuList/menuList.h"

extern SystemState alarm_cancelPage_state;

void alarmCancelPage_init();
void alarmCancelPage_draw();
void alarmCancelPage_confirm_handler();
void alarmCancelPage_cancle_handler();
void alarmCancelPage_next_handler();
void alarmCancelPage_back_handler();

void alarmCancelPage_update_list();

#endif // ALARMCANCELPAGE_H