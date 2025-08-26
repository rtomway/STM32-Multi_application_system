#include "mainPage.h"
#include "mainPageIcon.h"

#include "u8g2/u8g2.h"
#include "myU8g2/myU8g2.h"
#include "rtc.h"
#include "stdio.h"

//oled屏幕
#define OLED_HEIGHT 64
#define OLED_WIDTH 128

// 字体设置
#define TIME_FONT u8g2_font_logisoso26_tn
#define DATE_FONT u8g2_font_helvB10_tr
#define WEEK_FONT u8g2_font_helvB10_tr

// 显示位置定义
#define DATE_Y_POS 15
#define WEEK_Y_POS 60

SystemState g_mainPage_state =
{
        .handle_confirm = mainPage_confirm_handler,
        .handle_cancel = mainPage_cancle_handler,
        .handle_next = mainPage_next_handler,
        .handle_back = mainPage_back_handler,
        .draw = mainPage_draw,
};

// 星期数组
const char *weekday[] = {"Sunday", "Monday", "Tuesday", "Wednesday","Thursday", "Friday", "Saturday"};

// 时间显示位置
typedef struct
{
    int x[4]; 
    int y;    
} TimePosition;

TimePosition time_pos = {
    .x = {15, 45, 75, 105}, 
    .y = 45                 
};

/******************************************************************************
 * @author xu
 *  * 函数名称：mainPage_init
 *  * 功能描述：主页初始化函数
 ******************************************************************************/
void mainPage_init()
{
   
}

/******************************************************************************
 * @author xu 
 *  * 函数名称：mainPage_draw
 *  * 功能描述：主页面绘制函数
*******************************************************************************/
void mainPage_draw()
{

    char time_str[6]; 
    char date_str[20];
    RTC_DateTypeDef current_date;
    RTC_TimeTypeDef current_time;


    // 获取当前时间和日期
    HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BIN);

    u8g2_ClearBuffer(&u8g2);

    //日期
    u8g2_SetFont(&u8g2, DATE_FONT);
    snprintf(date_str, sizeof(date_str), "20%02d-%02d-%02d",current_date.Year, current_date.Month, current_date.Date);
    int date_width = u8g2_GetStrWidth(&u8g2, date_str);
    int date_x = (OLED_WIDTH - date_width) / 2;
    u8g2_DrawStr(&u8g2, date_x, DATE_Y_POS, date_str);

    //时间
    u8g2_SetFont(&u8g2, TIME_FONT);
    snprintf(time_str, sizeof(time_str), "%02d:%02d",current_time.Hours, current_time.Minutes);
    int time_width = u8g2_GetStrWidth(&u8g2, time_str);
    int time_x = (OLED_WIDTH - time_width) / 2;
    u8g2_DrawStr(&u8g2, time_x, time_pos.y, time_str);

    //星期
    u8g2_SetFont(&u8g2, WEEK_FONT);
    if (current_date.WeekDay < 7)
    {
        int week_width = u8g2_GetStrWidth(&u8g2, weekday[current_date.WeekDay]);
        u8g2_DrawStr(&u8g2, (OLED_WIDTH - week_width) / 2, WEEK_Y_POS, weekday[current_date.WeekDay]);
    }

    u8g2_SendBuffer(&u8g2);
}

/******************************************************************************
 * @author xu
 *  * 函数名称：mainPage_confirm_handler
 *  * 功能描述：主页按键处理函数
 *******************************************************************************/
void mainPage_confirm_handler()
{
    systemState_trans_menuNavPage(&g_current_state);
}

/******************************************************************************
 * @author xu
 *  * 函数名称：mainPage_cancle_handler
 *  * 功能描述：主页按键处理函数
 *******************************************************************************/
void mainPage_cancle_handler()
{
    systemState_trans_menuNavPage(&g_current_state);
}

/******************************************************************************
 * @author xu
 *  * 函数名称：mainPage_next_handler
 *  * 功能描述：主页按键处理函数
 *******************************************************************************/
void mainPage_next_handler()
{
    systemState_trans_menuNavPage(&g_current_state);
}

/******************************************************************************
 * @author xu
 *  * 函数名称：mainPage_back_handler
 *  * 功能描述：主页按键处理函数
 *******************************************************************************/
void mainPage_back_handler()
{
    systemState_trans_menuNavPage(&g_current_state);
}


