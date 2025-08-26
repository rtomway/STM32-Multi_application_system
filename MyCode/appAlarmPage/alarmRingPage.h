#ifndef ALARMRINGPAGE_H
#define ALARMRINGPAGE_H

#include "systemStatus/systemStatus.h"
#include "appAlarmPage.h"

extern SystemState alarm_ringPage_state;

void alarmRingPage_init();
void alarmRingPage_draw();
void alarmRingPage_confirm_handler();
void alarmRingPage_cancle_handler();
void alarmRingPage_next_handler();
void alarmRingPage_back_handler();

#endif // ALARMRINGPAGE_H