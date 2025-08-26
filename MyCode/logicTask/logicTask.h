#ifndef __LOGICTASK
#define __LOGICTASK

#include "FreeRTOS.h"
#include "queue.h"

void logicTask(void *argument);

extern QueueHandle_t xKeyQueue;
extern TaskHandle_t logicTaskHandle;

#endif /* !__LOGICTASK */