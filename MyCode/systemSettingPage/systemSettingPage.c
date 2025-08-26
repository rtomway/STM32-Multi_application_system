#include "systemSettingPage.h"

#include "u8g2/u8g2.h"
#include "myU8g2/myU8g2.h"
#include "menuList/menuList.h"
#include "rtc.h"

#include "settingsMainPage.h"
#include "timeSetPage.h"
#include "lightSetPage.h"

SystemState g_systemSettingPage_state;
SystemState *g_systemSettingPage_state_ptr=NULL;

/******************************************************************************
 * @author xu
 *  * 函数名称：systemSettings_init
 *  * 功能描述：系统设置页面初始化
 *******************************************************************************/
void systemSettings_init()
{
    systemSettingsMainPage_init();
    systemSettingsTimeSetPage_init();
    systemSettings_lightSetPage_init();

    g_systemSettingPage_state = systemSettings_mainPage_state;
    g_systemSettingPage_state_ptr=&g_systemSettingPage_state;
}


/******************************************************************************
 * @author xu 
 *  * 函数名称：systemSettings_draw
 *  * 功能描述：系统设置页面绘制
*******************************************************************************/
void systemSettings_draw()
{
    g_systemSettingPage_state.draw();
}

/******************************************************************************
 * @author xu
 *  * 函数名称：systemSettings_confirm_handler
 *  * 功能描述：将外部接收的按键输入事件传递给当前状态处理
 *******************************************************************************/
void systemSettings_confirm_handler()
{
    g_systemSettingPage_state.handle_confirm(&g_systemSettingPage_state_ptr);
}

/******************************************************************************
 * @author xu
 *  * 函数名称：systemSettings_confirm_handler
 *  * 功能描述：将外部接收的按键输入事件传递给当前状态处理
 *******************************************************************************/
void systemSettings_cancle_handler()
{
    g_systemSettingPage_state.handle_cancel(&g_systemSettingPage_state_ptr);
}

/******************************************************************************
 * @author xu
 *  * 函数名称：systemSettings_confirm_handler
 *  * 功能描述：将外部接收的按键输入事件传递给当前状态处理
 *******************************************************************************/
void systemSettings_next_handler()
{
    g_systemSettingPage_state.handle_next(&g_systemSettingPage_state_ptr);
}

/******************************************************************************
 * @author xu
 *  * 函数名称：systemSettings_confirm_handler
 *  * 功能描述：将外部接收的按键输入事件传递给当前状态处理
 *******************************************************************************/
void systemSettings_back_handler()
{
    g_systemSettingPage_state.handle_back(&g_systemSettingPage_state_ptr);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：systemSettings_transPage
 *  * 功能描述：切换系统设置页面状态
 *  * 输入参数：state {type}
 ***********************************************************************************************************************/
void systemSettings_transPage(SystemState *state)
{
    g_systemSettingPage_state = *state;
}