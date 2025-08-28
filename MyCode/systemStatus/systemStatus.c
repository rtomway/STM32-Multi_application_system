#include "systemStatus.h"

#include "mainPage/mainPage.h"
#include "menuNavPage/menuNavPage.h"
#include "appAlarmPage/appAlarmPage.h"
#include "systemSettingPage/systemSettingPage.h"
#include "configStorage/configStorage.h"



SystemState *g_current_state = NULL;
SystemState *g_previous_state=NULL;

/******************************************************************************
 * @author xu 
 *  * 函数名称：systemState_init
 *  * 功能描述：系统状态初始化
*******************************************************************************/
void systemState_init()
{
    ConfigStorage_Init();

    myU8g2_Init(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_helvB12_tr);

    mainPage_init();
    menuNavPage_init();
    appAlarmPage_init();
    systemSettings_init();

    g_current_state = &g_mainPage_state;
    g_previous_state = NULL;
    g_current_state->draw();
}

/******************************************************************************
 * @author xu
 *  * 函数名称：systemState_trans_mainPage
 *  * 功能描述：切换到主页
 *  * 输入参数：current_state {type}
 *******************************************************************************/
void systemState_trans_mainPage(SystemState **current_state)
{
    g_previous_state = g_current_state;
    *current_state = &g_mainPage_state;
}

/******************************************************************************
 * @author xu
 *  * 函数名称：systemState_trans_menuNavPage
 *  * 功能描述：切换到菜单导航页
 *  * 输入参数：current_state {type}
 *******************************************************************************/
void systemState_trans_menuNavPage(SystemState **current_state)
{
    g_previous_state = g_current_state;
    *current_state = &g_menuNavPage_state;
}

/******************************************************************************
 * @author xu 
 *  * 函数名称：systemState_trans_appAlarmPage
 *  * 功能描述：切换到闹钟页
 *  * 输入参数：current_state {type}
*******************************************************************************/
void systemState_trans_appAlarmPage(SystemState **current_state)
{
    g_previous_state = g_current_state;
    *current_state = &g_appAlarmPage_state;
}

/******************************************************************************
 * @author xu 
 *  * 函数名称：systemState_trans_systemSettingsPage
 *  * 功能描述：切换到系统设置页
 *  * 输入参数：current_state {type}
*******************************************************************************/
void systemState_trans_systemSettingsPage(SystemState **current_state)
{
    g_previous_state = g_current_state;
    *current_state = &g_systemSettingPage_state;
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：systemState_rollBackOneStep
 *  * 功能描述：系统状态回滚一步
 ***********************************************************************************************************************/
void systemState_rollBackOneStep()
{
    if (g_previous_state != NULL)
    {
        g_current_state = g_previous_state;
    }
}
