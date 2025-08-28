#include "appAlarmPage.h"

#include "alarmMainPage.h"
#include "alarmAddPage.h"
#include "alarmCancelPage.h"
#include "alarmRingPage.h"
#include "alarmManager.h"

SystemState g_appAlarmPage_state;
SystemState* g_appAlarmPage_state_ptr = NULL;

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：appAlarmPage_init
 *  * 功能描述：闹钟app管理的初始化函数  初始化各个子界面以及当前状态
 ***********************************************************************************************************************/
void appAlarmPage_init()
{
    alarmMainPage_init();
    alarmAddPage_init();
    alarmCancelPage_init();
    alarmRingPage_init();
    alarm_init();

    g_appAlarmPage_state_ptr = &g_appAlarmPage_state;
    g_appAlarmPage_state = alarm_mainPage_state;
}


/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：g_appAlarmPage_state
 *  * 功能描述：调用当前状态的draw函数
 ***********************************************************************************************************************/
void appAlarmPage_draw()
{
    g_appAlarmPage_state.draw();
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：appAlarmPage_confirm_handler
 *  * 功能描述：调用当前状态的confirm函数 传递确认按键事件
 ***********************************************************************************************************************/
void appAlarmPage_confirm_handler()
{
    g_appAlarmPage_state.handle_confirm(&g_appAlarmPage_state_ptr);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：appAlarmPage_cancle_handler
 *  * 功能描述：调用当前状态的cancel函数 传递取消按键事件
 ***********************************************************************************************************************/
void appAlarmPage_cancle_handler()
{
    g_appAlarmPage_state.handle_cancel(&g_appAlarmPage_state_ptr);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：appAlarmPage_next_handler
 *  * 功能描述：调用当前状态的next函数 传递下一个按键事件
 ***********************************************************************************************************************/
void appAlarmPage_next_handler()
{
    g_appAlarmPage_state.handle_next(&g_appAlarmPage_state_ptr);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：appAlarmPage_back_handler
 *  * 功能描述：调用当前状态的back函数 传递返回按键事件
 ***********************************************************************************************************************/
void appAlarmPage_back_handler()
{
    g_appAlarmPage_state.handle_back(&g_appAlarmPage_state_ptr);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarm_transPage
 *  * 功能描述：界面跳转函数 切换到指定状态
 *  * 输入参数：state {type}
 ***********************************************************************************************************************/
void alarm_transPage(SystemState *state)
{
    g_appAlarmPage_state = *state;
}
