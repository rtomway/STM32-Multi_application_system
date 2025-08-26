#ifndef TIMESETPAGE_H_
#define TIMESETPAGE_H_

#include "systemStatus/systemStatus.h"
#include "systemSettingPage.h"

extern SystemState systemSettings_timeSetPage_state;

void systemSettingsTimeSetPage_init();
void systemSettingsTimeSetPage_draw();
void systemSettingsTimeSetPage_confirm_handler();
void systemSettingsTimeSetPage_cancle_handler();
void systemSettingsTimeSetPage_next_handler();
void systemSettingsTimeSetPage_back_handler();

#endif /* TIMESETPAGE_H_ */