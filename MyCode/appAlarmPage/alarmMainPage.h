#ifndef ALARMMAINPAGE_H
#define ALARMMAINPAGE_H

#include "systemStatus/systemStatus.h"
#include "appAlarmPage.h"

extern SystemState alarm_mainPage_state;

void alarmMainPage_init();
void alarmMainPage_draw();
void alarmMainPage_confirm_handler();
void alarmMainPage_cancle_handler();
void alarmMainPage_next_handler();
void alarmMainPage_back_handler();

#endif // ALARMMAINPAGE_H