#ifndef SYSTEMSETTINGPAGE_H_
#define SYSTEMSETTINGPAGE_H_

#include "systemStatus/systemStatus.h"

extern SystemState g_systemSettingPage_state;
extern SystemState *g_systemSettingPage_state_ptr;

void systemSettings_init();
void systemSettings_draw();
void systemSettings_confirm_handler();
void systemSettings_cancle_handler();
void systemSettings_next_handler();
void systemSettings_back_handler();

void systemSettings_transPage(SystemState *state);

#endif /* SYSTEMSETTINGPAGE_H_ */