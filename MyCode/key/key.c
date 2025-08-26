#include "key.h"

#define KEY_SCAN_INTERVAL_MS 5   // 扫描间隔
#define KEY_DEBOUNCE_TIME_MS 150    // 按键去抖时间

// 按键读到低电平时，证明被按下
#define KEY_PRESSED GPIO_PIN_RESET
#define KEY_UNPRESSED GPIO_PIN_SET

// 按键的GPIO端口和引脚定义
#define KEY_GPIO_Port_1 GPIOA
#define KEY_CONFIRM_Pin GPIO_PIN_1
#define KEY_CANCEL_Pin GPIO_PIN_2

#define KEY_GPIO_Port_2 GPIOB
#define KEY_NEXT_Pin GPIO_PIN_13
#define KEY_BACK_Pin GPIO_PIN_14


static const Key_TypeDef keys[] =
{
    {KEY_GPIO_Port_1, KEY_CONFIRM_Pin},
    {KEY_GPIO_Port_1, KEY_CANCEL_Pin},
    {KEY_GPIO_Port_2, KEY_NEXT_Pin},
    {KEY_GPIO_Port_2, KEY_BACK_Pin}
};

QueueHandle_t xKeyQueue = NULL;
// 按键状态
static KeyState key_state[KEY_CODE_NUM] = {KEY_STATE_RELEASED};
static uint32_t key_change_timestamp[KEY_CODE_NUM] = {0};

/******************************************************************************
 * @author xu
 *  * 函数名称：keySacnTask
 *  * 功能描述：按键扫描任务 检测按键是否被按下，并将按键事件放入队列中
 *  * 输入参数：argument {type}
 *******************************************************************************/
void keySacnTask(void *argument)
{

    // 确保队列初始化
    if (xKeyQueue == NULL)
    {
        xKeyQueue = xQueueCreate(10, sizeof(Key_Message_t));
    }

    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(KEY_SCAN_INTERVAL_MS));

        uint32_t current_time = HAL_GetTick();
        for (int i = 0; i < KEY_CODE_NUM; i++)
        {
            GPIO_PinState pin_state = HAL_GPIO_ReadPin(keys[i].GPIO_Port, keys[i].GPIO_Pin);

            switch (key_state[i])
            {
            case KEY_STATE_RELEASED:
                if (pin_state == KEY_PRESSED)
                {
                    key_state[i] = KEY_STATE_DEBOUNCE;
                    key_change_timestamp[i] = current_time;
                }
                break;

            case KEY_STATE_DEBOUNCE:
                if (pin_state == KEY_PRESSED)
                {
                    if (current_time - key_change_timestamp[i] >= KEY_DEBOUNCE_TIME_MS)
                    {
                        Key_Message_t msg = {
                            .key_code = (Key_Code_t)i,
                            .event = KEY_EVENT_PRESS
                        };
                        xQueueSend(xKeyQueue, &msg, 0);
                        key_state[i] = KEY_STATE_PRESSED;
                    }
                }
                else
                {
                    key_state[i] = KEY_STATE_RELEASED;
                }
                break;

            case KEY_STATE_PRESSED:
                if (pin_state == KEY_UNPRESSED)
                {
                    key_state[i] = KEY_STATE_RELEASED;
                }
                break;
            }
        }
    }
}
