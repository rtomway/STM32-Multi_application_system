#ifndef SETTINGSMAINPAGE_H_
#define SETTINGSMAINPAGE_H_

#include "systemStatus/systemStatus.h"

extern SystemState systemSettings_mainPage_state;

void systemSettingsMainPage_init();
void systemSettingsMainPage_draw();
void systemSettingsMainPage_confirm_handler();
void systemSettingsMainPage_cancle_handler();
void systemSettingsMainPage_next_handler();
void systemSettingsMainPage_back_handler();

#endif /* SETTINGSMAINPAGE_H_ */