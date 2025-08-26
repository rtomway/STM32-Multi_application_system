#ifndef __KEY_H
#define __KEY_H

#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"

typedef struct Key_TypeDef
{
    GPIO_TypeDef *GPIO_Port;
    uint16_t GPIO_Pin;
} Key_TypeDef;

//按键事件
typedef enum
{
    KEY_EVENT_PRESS = 0,   
} Key_Event_t;

//各按键
typedef enum
{
    KEY_CODE_CONFIRM = 0,
    KEY_CODE_CANCEL,
    KEY_CODE_NEXT,
    KEY_CODE_BACK,
    KEY_CODE_NUM 
} Key_Code_t;

//按键消息
typedef struct
{
    Key_Code_t key_code; 
    Key_Event_t event;   
} Key_Message_t;

//按键状态
typedef enum
{
    KEY_STATE_RELEASED, 
    KEY_STATE_DEBOUNCE, 
    KEY_STATE_PRESSED  
} KeyState;

//按键队列
extern QueueHandle_t xKeyQueue;

//按键扫描任务
void keySacnTask(void *argument);

#endif // !__KEY_H
