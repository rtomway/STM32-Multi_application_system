#include "alarmMainPage.h"
#include "alarmAddPage.h"
#include "alarmCancelPage.h"
#include "u8g2/u8g2.h"
#include "myU8g2/myU8g2.h"
#include "menuList/menuList.h"

#define ALARM_MAIN_PAGE_FONT u8g2_font_helvB12_tr

SystemState alarm_mainPage_state =
    {
        .handle_confirm = alarmMainPage_confirm_handler,
        .handle_cancel = alarmMainPage_cancle_handler,
        .handle_next = alarmMainPage_next_handler,
        .handle_back = alarmMainPage_back_handler,
        .draw = alarmMainPage_draw,
};

Menu_Typedef alarm_main_menu;
Menu_Option_Typedef alarm_menu_options[] = {{"addAlarm", NULL}, {"cancelAlarm", NULL}};

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmMainPage_init
 *  * 功能描述：闹钟主页面初始化 构建闹钟菜单
 ***********************************************************************************************************************/
void alarmMainPage_init()
{
    Add_Menu("Alarm_Menu", &alarm_main_menu, NULL, NULL);
    Add_Option_List(&alarm_main_menu, alarm_menu_options, sizeof(alarm_menu_options) / sizeof(Menu_Option_Typedef));
}


/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmMainPage_draw
 *  * 功能描述：闹钟主页面绘制
 ***********************************************************************************************************************/
void alarmMainPage_draw()
{
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, ALARM_MAIN_PAGE_FONT);
    Draw_Menu(&alarm_main_menu);
    Draw_Cursor(&alarm_main_menu);
    u8g2_SendBuffer(&u8g2);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmMainPage_confirm_handler
 *  * 功能描述：确认按钮事件处理  进入当前选中项页面
 ***********************************************************************************************************************/
void alarmMainPage_confirm_handler()
{
   if(alarm_main_menu.current_index == 0)
   {
       alarm_transPage(&alarm_addPage_state);
   }
   else if (alarm_main_menu.current_index == 1)
   {
       alarm_transPage(&alarm_cancelPage_state);
   }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmMainPage_cancle_handler
 *  * 功能描述：返回按钮事件处理  返回菜单导航页面
 ***********************************************************************************************************************/
void alarmMainPage_cancle_handler()
{
    systemState_trans_menuNavPage(&g_current_state);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmMainPage_next_handler
 *  * 功能描述：下一个按钮事件处理  移动到下一个闹钟设置选项
 ***********************************************************************************************************************/
void alarmMainPage_next_handler()
{
    Move_Cursor(&alarm_main_menu, 1);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmMainPage_back_handler
 *  * 功能描述：移动到上一个闹钟设置选项
 ***********************************************************************************************************************/
void alarmMainPage_back_handler()
{
    Move_Cursor(&alarm_main_menu, 0);
}