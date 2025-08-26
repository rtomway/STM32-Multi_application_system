#include "alarmCancelPage.h"
#include "alarmMainPage.h"
#include "u8g2/u8g2.h"
#include "myU8g2/myU8g2.h"
#include "alarmManager.h"
#include <stdio.h>
#include "appAlarmPage/alarmRingPage.h"

SystemState alarm_cancelPage_state = {
    .handle_confirm = alarmCancelPage_confirm_handler,
    .handle_cancel = alarmCancelPage_cancle_handler,
    .handle_next = alarmCancelPage_next_handler,
    .handle_back = alarmCancelPage_back_handler,
    .draw = alarmCancelPage_draw,
};

static Menu_Typedef alarm_list_menu;
static Menu_Option_Typedef alarm_options[MAX_ALARMS];
static char alarm_option_names[MAX_ALARMS][20];

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmCancelPage_init
 *  * 功能描述：闹种取消页面初始化
 ***********************************************************************************************************************/
void alarmCancelPage_init()
{
    alarm_list_menu.menu_name = "Cancel Alarm";
    alarm_list_menu.parent_menu = NULL;
    alarm_list_menu.current_index = 0;
    alarm_list_menu.option_num = 0;
    alarm_list_menu.options = NULL;

    // 更新闹钟列表
    alarmCancelPage_update_list();
}

void alarmCancelPage_run()
{

}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmCancelPage_draw
 *  * 功能描述：闹钟取消页面绘制
 ***********************************************************************************************************************/
void alarmCancelPage_draw()
{

    alarmCancelPage_update_list();

    u8g2_ClearBuffer(&u8g2);

    if (alarm_list_menu.option_num == 0)
    {
        u8g2_DrawStr(&u8g2, 0, 12, "Delete Alarm");
        u8g2_DrawHLine(&u8g2, 0, 14, 128);
        // 没有闹钟提示
        u8g2_DrawStr(&u8g2, 0, 35, "No alarms");
        u8g2_DrawStr(&u8g2, 0, 50, "Press back");
    }
    else
    {
        Draw_Menu(&alarm_list_menu);
        Draw_Cursor(&alarm_list_menu);
    }

    u8g2_SendBuffer(&u8g2);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmCancelPage_confirm_handler
 *  * 功能描述：负责删除选择的闹钟
 ***********************************************************************************************************************/
void alarmCancelPage_confirm_handler()
{
    if (alarm_list_menu.option_num == 0)
    {
        alarm_transPage(&alarm_mainPage_state);
        return;
    }

    // 删除选中的闹钟
    if (alarm_remove(alarm_list_menu.current_index))
    {
        // 更新闹钟列表
        alarmCancelPage_update_list();

        if (alarm_list_menu.option_num == 0)
        {
            alarm_transPage(&alarm_mainPage_state);
        }
        else
        {
            if (alarm_list_menu.current_index >= alarm_list_menu.option_num)
            {
                alarm_list_menu.current_index = alarm_list_menu.option_num - 1;
            }
        }
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmCancelPage_cancle_handler
 *  * 功能描述：返回到闹钟设置主页
 ***********************************************************************************************************************/
void alarmCancelPage_cancle_handler()
{
    alarm_transPage(&alarm_mainPage_state);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmCancelPage_next_handler
 *  * 功能描述：移动到下一个闹钟
 ***********************************************************************************************************************/
void alarmCancelPage_next_handler()
{
    if (alarm_list_menu.option_num > 0)
    {
        Move_Cursor(&alarm_list_menu, 1);
    } 
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmCancelPage_back_handler
 *  * 功能描述：移动到上一个闹钟
 ***********************************************************************************************************************/
void alarmCancelPage_back_handler()
{
    if (alarm_list_menu.option_num > 0)
    {
        Move_Cursor(&alarm_list_menu, 0); 
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：alarmCancelPage_update_list
 *  * 功能描述：更新闹钟列表
 ***********************************************************************************************************************/
void alarmCancelPage_update_list()
{
    uint8_t count = alarm_get_count();
    Alarm *alarms = alarm_get_all();

    alarm_list_menu.option_num = count;

    for (int i = 0; i < count; i++)
    {
        //闹钟字符串
        snprintf(alarm_option_names[i], sizeof(alarm_option_names[i]),
                 "%02d:%02d",
                 alarms[i].hour,
                 alarms[i].minute
                );

        alarm_options[i].option_name = alarm_option_names[i];
        alarm_options[i].func = NULL;
        alarm_options[i].next = NULL;
        alarm_options[i].prev = NULL;
        alarm_options[i].child_menu = NULL;
    }

    if (count > 0)
    {
        for (int i = 0; i < count - 1; i++)
        {
            alarm_options[i].next = &alarm_options[i + 1];
        }
        for (int i = 1; i < count; i++)
        {
            alarm_options[i].prev = &alarm_options[i - 1];
        }
        alarm_list_menu.options = &alarm_options[0];
    }
    else
    {
        alarm_list_menu.options = NULL;
    }
}