#include "settingsMainPage.h"
#include "systemSettingPage.h"
#include "u8g2/u8g2.h"
#include "myU8g2/myU8g2.h"
#include "menuList/menuList.h"
#include "timeSetPage.h"
#include "lightSetPage.h"

SystemState systemSettings_mainPage_state = {
    .handle_confirm = systemSettingsMainPage_confirm_handler,
    .handle_cancel = systemSettingsMainPage_cancle_handler,
    .handle_next = systemSettingsMainPage_next_handler,
    .handle_back = systemSettingsMainPage_back_handler,
    .draw = systemSettingsMainPage_draw,
};



Menu_Typedef systemSettings_main_menu;
Menu_Option_Typedef systemSettings_menu_options[] = {{"timeSet", NULL}, {"lightSet", NULL}};

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：systemSettingsMainPage_init
 *  * 功能描述： 系统设置主页面初始化 构建系统设置主页面菜单
 ***********************************************************************************************************************/
void systemSettingsMainPage_init()
{
    Add_Menu("SystemSettings_Menu", &systemSettings_main_menu, NULL, NULL);
    Add_Option_List(&systemSettings_main_menu, systemSettings_menu_options,sizeof(systemSettings_menu_options)/sizeof(Menu_Option_Typedef));
}


/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：systemSettingsMainPage_draw
 *  * 功能描述：绘制系统设置主页面
 ***********************************************************************************************************************/
void systemSettingsMainPage_draw()
{
    u8g2_ClearBuffer(&u8g2);
    Draw_Menu(&systemSettings_main_menu);
    Draw_Cursor(&systemSettings_main_menu);
    u8g2_SendBuffer(&u8g2);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：systemSettingsMainPage_confirm_handler
 *  * 功能描述：确认按钮事件处理 跳转到对应子页面
 ***********************************************************************************************************************/
void systemSettingsMainPage_confirm_handler()
{
    if (systemSettings_main_menu.current_index == 0)
    {
        systemSettings_transPage(&systemSettings_timeSetPage_state);
    }
    else if (systemSettings_main_menu.current_index == 1)
    {
        systemSettings_transPage(&systemSettings_lightSetPage_state);
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：systemSettingsMainPage_cancle_handler
 *  * 功能描述：取消按钮事件处理 切换到菜单导航页面
 ***********************************************************************************************************************/
void systemSettingsMainPage_cancle_handler()
{
    systemState_trans_menuNavPage(&g_current_state);
}
/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：systemSettingsMainPage_next_handler
 *  * 功能描述：下一个按键事件处理 切换到下一个选项
 ***********************************************************************************************************************/
void systemSettingsMainPage_next_handler()
{
    Move_Cursor(&systemSettings_main_menu, 1);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：systemSettingsMainPage_back_handler
 *  * 功能描述：上一个按键事件处理 切换到上一个选项
 ***********************************************************************************************************************/
void systemSettingsMainPage_back_handler()
{
    Move_Cursor(&systemSettings_main_menu, 0);
}