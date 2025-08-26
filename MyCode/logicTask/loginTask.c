#include "logicTask.h"
#include "key/key.h"
#include "systemStatus/systemStatus.h"

#include "appAlarmPage/alarmManager.h"
#include "appAlarmPage/alarmRingPage.h"

TaskHandle_t logicTaskHandle = NULL;

/******************************************************************************
 * @author xu
 *  * 函数名称：logicTask
 *  * 功能描述：负责将按键输入消息转发给当前状态机系统状态接口
 *  * 输入参数：argument {type}
 *******************************************************************************/
void logicTask(void *argument)
{
    Key_Message_t keyMsg;
    for (;;)
    {
        // 阻塞等待按键消息
        if (xQueueReceive(xKeyQueue, &keyMsg, portMAX_DELAY) == pdTRUE)
        {
            switch (keyMsg.key_code)
            {
            case KEY_CODE_CONFIRM:
                g_current_state->handle_confirm(&g_current_state);
                break;
            case KEY_CODE_CANCEL:
                g_current_state->handle_cancel(&g_current_state);
                break;
            case KEY_CODE_NEXT:
                g_current_state->handle_next(&g_current_state);
                break;
            case KEY_CODE_BACK:
                g_current_state->handle_back(&g_current_state);
                break;
            default:
                break;
            }
        }
    }
}

