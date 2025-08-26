#ifndef __APP_ALARM_PAGE_H__
#define __APP_ALARM_PAGE_H__

#include "systemStatus/systemStatus.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "appAlarmIcon.h"
#include "myU8g2/myU8g2.h"
#include "u8g2/u8g2.h"
#include "menuList/menuList.h"

extern SystemState g_appAlarmPage_state;
extern SystemState *g_appAlarmPage_state_ptr;

void appAlarmPage_init();
void appAlarmPage_draw();
void appAlarmPage_confirm_handler();
void appAlarmPage_cancle_handler();
void appAlarmPage_next_handler();
void appAlarmPage_back_handler();

void alarm_transPage(SystemState* state);


#endif /* __APP_ALARM_PAGE_H__ */
