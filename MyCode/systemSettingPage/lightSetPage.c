#include "lightSetPage.h"
#include "u8g2/u8g2.h"
#include "myU8g2/myU8g2.h"
#include "settingsMainPage.h"
#include "systemSettingPage.h"
#include "stdio.h"
#include "configStorage/configStorage.h"

SystemState systemSettings_lightSetPage_state = {
    .handle_confirm = systemSettings_lightSetPage_confirm_handler,
    .handle_cancel = systemSettings_lightSetPage_cancle_handler,
    .handle_next = systemSettings_lightSetPage_next_handler,
    .handle_back = systemSettings_lightSetPage_back_handler,
    .draw = systemSettings_lightSetPage_draw,
};

// 亮度10个层次
static uint8_t current_brightness;
static uint8_t brightness_step = 5; 
static uint8_t edit_mode = 0;    

static const uint8_t step_to_brightness[11] = {10, 25, 51, 76, 102,128,153,179,204,230,255 };

Config *systemConfig = NULL;

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：systemSettings_lightSetPage_init
 *  * 功能描述：初始化亮度设置页面  设置当前亮度
 ***********************************************************************************************************************/
void systemSettings_lightSetPage_init()
{

    // 从配置文件中读取亮度设置
    systemConfig = ConfigStorage_GetConfig();
    current_brightness = systemConfig->config_brightness;
    if (current_brightness==0)
    {
        current_brightness=128;
    }
    // 根据当前亮度找到对应的步进
    for (uint8_t i = 0; i <= 10; i++)
    {
        if (current_brightness <= step_to_brightness[i])
        {
            brightness_step = i;
            break;
        }
    }

    edit_mode = 0;
    u8g2_SetContrast(&u8g2, step_to_brightness[brightness_step]);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：systemSettings_lightSetPage_draw
 *  * 功能描述：亮度设置页面绘制函数  显示亮度等级和进度条
 ***********************************************************************************************************************/
void systemSettings_lightSetPage_draw()
{
    // 实时应用亮度设置
    u8g2_SetContrast(&u8g2, step_to_brightness[brightness_step]);

    u8g2_ClearBuffer(&u8g2);

    u8g2_SetFont(&u8g2, u8g2_font_helvB12_tr);
    u8g2_SetDrawColor(&u8g2, 1);

    u8g2_DrawStr(&u8g2, 0, 15, "Brightness");
    u8g2_DrawHLine(&u8g2, 10, 20, 108);

    // 显示亮度等级
    u8g2_SetFont(&u8g2, u8g2_font_helvB10_tr);
    char step_str[20];
    snprintf(step_str, sizeof(step_str), "Level: %d/10", brightness_step);
    u8g2_DrawStr(&u8g2, 15, 35, step_str);

    // 绘制进度条背景
    u8g2_DrawFrame(&u8g2, 15, 40, 100, 10);
    // 绘制进度条
    uint8_t progress_width = brightness_step * 10;
    u8g2_DrawBox(&u8g2, 15, 40, progress_width, 10);

    // 绘制左右箭头指示
    if (edit_mode == 1)
    {
        u8g2_DrawStr(&u8g2, 5, 51, "<");
        u8g2_DrawStr(&u8g2, 120, 51, ">");
    }

    // 显示操作提示
    u8g2_SetFont(&u8g2, u8g2_font_helvB12_tr);
    if (edit_mode == 0)
    {
        u8g2_DrawStr(&u8g2, 30, 63, "Edit?<");
    }else
    {
        u8g2_DrawStr(&u8g2, 66, 63, "^");
    }
       
    u8g2_SendBuffer(&u8g2);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：systemSettings_lightSetPage_confirm_handler
 *  * 功能描述：确认按钮事件处理函数  切换编辑模式
 ***********************************************************************************************************************/
void systemSettings_lightSetPage_confirm_handler()
{
    if (edit_mode == 0)
    {
        edit_mode = 1;
    }
    else
    {
        edit_mode = 0;
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：systemSettings_lightSetPage_cancle_handler
 *  * 功能描述：取消按钮事件处理函数  切换到上一页/退出编辑模式
 ***********************************************************************************************************************/
void systemSettings_lightSetPage_cancle_handler()
{
    if (edit_mode == 1)
    {
        edit_mode = 0;
    }
    else
    {
      //bright_value = current_brightness;
      systemConfig->config_brightness = current_brightness;
       ConfigStorage_Save();
       systemSettings_transPage(&systemSettings_mainPage_state);
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：systemSettings_lightSetPage_next_handler
 *  * 功能描述：下一页按钮事件处理函数  切换到下一层亮度
 ***********************************************************************************************************************/
void systemSettings_lightSetPage_next_handler()
{
    if (edit_mode == 1)
    {
        if (brightness_step < 10)
        {
            brightness_step++;
            current_brightness = step_to_brightness[brightness_step];
        }
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：systemSettings_lightSetPage_back_handler
 *  * 功能描述：上一个按键事件处理函数  切换到上一层亮度
 ***********************************************************************************************************************/
void systemSettings_lightSetPage_back_handler()
{
    if (edit_mode == 1)
    {
        if (brightness_step > 0)
        {
            brightness_step--;
            current_brightness = step_to_brightness[brightness_step];
        }
    }
}