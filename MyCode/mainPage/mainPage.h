#ifndef __MAINPAGE
#define __MAINPAGE

#include "systemStatus/systemStatus.h"
#include "FreeRTOS.h"
#include "timers.h"

extern SystemState g_mainPage_state;

void mainPage_init();
void mainPage_draw();
void mainPage_confirm_handler();
void mainPage_cancle_handler();
void mainPage_next_handler();
void mainPage_back_handler();



#endif /* __MAINPAGE */
