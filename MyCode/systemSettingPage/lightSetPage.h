#ifndef LIGHTSET_H_
#define LIGHTSET_H_

#include "systemStatus/systemStatus.h"

extern SystemState systemSettings_lightSetPage_state;

void systemSettings_lightSetPage_init();
void systemSettings_lightSetPage_draw();
void systemSettings_lightSetPage_confirm_handler();
void systemSettings_lightSetPage_cancle_handler();
void systemSettings_lightSetPage_next_handler();
void systemSettings_lightSetPage_back_handler();

#endif /* LIGHTSET_H_ */