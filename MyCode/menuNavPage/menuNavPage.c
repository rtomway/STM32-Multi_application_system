#include "menuNavPage.h"
#include "menuNavIcon.h"

#include "u8g2/u8g2.h"
#include "myU8g2/myU8g2.h"
#include "mainPage/mainPage.h"

#define BOX_R 1
#define DOCK_Y 58
#define DOCK_R 3

#define ANIMATION_STEPS 2 
#define ANIMATION_DELAY 2 

#define ICON_Y_NORMAL 27   
#define ICON_Y_RAISED 16   
#define SCREEN_WIDTH 128

#define SELECTION_BOX_X 49 
#define SELECTION_BOX_Y 16

#define MENUNAV_TITLE_FONT u8g2_font_helvB10_tr

SystemState g_menuNavPage_state =
{
        .handle_confirm = menuNavPage_confirm_handler,
        .handle_cancel = menuNavPage_cancle_handler,
        .handle_next = menuNavPage_next_handler,
        .handle_back = menuNavPage_back_handler,
        .draw = menuNavPage_draw,
};

// 应用图标数组
AppIcon menuNav_app[] =
    {
        {"clock", 0, clock_icon_data, {0, 0, 30, 30}},
        {"setting", 1, setting_icon_data, {0, 0, 30, 30}},
        {"light", 2, light_icon_data, {0, 0, 30, 30}},
        {"cleder", 3, cleder_icon_data, {0, 0, 30, 30}},
        {"torch", 4, torch_icon_data, {0, 0, 30, 30}},
};

// 当前应用图标指针
static AppIcon *menuNav_current_app = NULL;

// 三个固定显示区域
IconDisplayArea icon_positions[3] = {
    {9, ICON_Y_NORMAL, 30, 30},  
    {49, ICON_Y_RAISED, 30, 30}, 
    {89, ICON_Y_NORMAL, 30, 30}  
};

IconDisplayArea Left = {0, 0, 23, 10};
IconDisplayArea Right = {104, 0, 23, 10};
IconDisplayArea String = {44, 10, 0, 0};

uint8_t is_animating = 0;
uint8_t animation_direction = 0;
uint8_t animation_step = 0;

static uint8_t dock_positions[] = {46,54,62,70,78};

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：menuNavPage_init
 *  * 功能描述：菜单导航页面初始化 选择第一个应用
 ***********************************************************************************************************************/
void menuNavPage_init()
{
    menuNav_current_app = &menuNav_app[0];
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：menuNavPage_draw
 *  * 功能描述：菜单导航页面绘制 中间是当前应用图标，左右两侧是应用图标
 ***********************************************************************************************************************/
void menuNavPage_draw()
{
    u8g2_ClearBuffer(&u8g2);

    u8g2_SetFont(&u8g2, MENUNAV_TITLE_FONT);

    // 绘制左右箭头
    u8g2_DrawXBMP(&u8g2, Left.x, Left.y, Left.width, Left.height, LeftMove);
    u8g2_DrawXBMP(&u8g2, Right.x, Right.y, Right.width, Right.height, RightMove);

    // 绘制当前应用名称
    if (menuNav_current_app)
    {
        u8g2_DrawStr(&u8g2, String.x, String.y, menuNav_current_app->name);
    }

    // 左边应用图标
    AppIcon *left_app = get_prev_app();
    if (left_app)
    {
        u8g2_DrawXBMP(&u8g2, icon_positions[0].x, icon_positions[0].y,
                      left_app->display_area.width,
                      left_app->display_area.height,
                      left_app->icon_data
                    );
    }

    // 中间选中应用图标
    if (menuNav_current_app)
    {
        u8g2_DrawXBMP(&u8g2, icon_positions[1].x, icon_positions[1].y,
                      menuNav_current_app->display_area.width, 
                      menuNav_current_app->display_area.height,
                      menuNav_current_app->icon_data
                    );
    }

    // 右边图标
    AppIcon *right_app = get_next_app();
    if (right_app)
    {
        u8g2_DrawXBMP(&u8g2, icon_positions[2].x, icon_positions[2].y,
                      right_app->display_area.width, right_app->display_area.height,
                      right_app->icon_data);
    }

    // 绘制选择框（围绕中间图标）
    if (menuNav_current_app)
    {
        u8g2_DrawFrame(&u8g2,
                       SELECTION_BOX_X - 1,
                       SELECTION_BOX_Y - 1,
                       menuNav_current_app->display_area.width + 2,
                       menuNav_current_app->display_area.height + 2);
    }

    // 绘制底部导航点
    if (menuNav_current_app)
    {
        u8g2_DrawDisc(&u8g2, dock_positions[menuNav_current_app->id], DOCK_Y, DOCK_R, U8G2_DRAW_ALL);
        for (int i = 0; i < sizeof(menuNav_app) / sizeof(menuNav_app[0]); i++)
        {
            if (i != menuNav_current_app->id)
            {
                u8g2_DrawCircle(&u8g2, dock_positions[i], DOCK_Y, DOCK_R, U8G2_DRAW_ALL);
            }
        }
    }

    u8g2_SendBuffer(&u8g2);
}

/***********************************************************************************************************************
 * @author xu 
 *  * 函数名称：menuNavPage_confirm_handler
 *  * 功能描述：确认按键处理 进入对应应用
***********************************************************************************************************************/
void menuNavPage_confirm_handler()
{
    if (!menuNav_current_app)
        return;

    switch (menuNav_current_app->id)
    {
    case 0: // clock
      systemState_trans_appAlarmPage(&g_current_state);
        break;
    case 1: // setting
       systemState_trans_systemSettingsPage(&g_current_state);
        break;
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：menuNavPage_cancle_handler
 *  * 功能描述：返回按键处理 返回主页
 ***********************************************************************************************************************/
void menuNavPage_cancle_handler()
{
    systemState_trans_mainPage(&g_current_state);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：menuNavPage_next_handler
 *  * 功能描述：下一个按键处理 切换到下一个应用
 ***********************************************************************************************************************/
void menuNavPage_next_handler()
{
    if (!is_animating && get_next_app() != NULL)
    {
        animate_slide(SLIDE_RIGHT);
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：menuNavPage_back_handler
 *  * 功能描述：上一个按键处理 切换到上一个应用
 ***********************************************************************************************************************/
void menuNavPage_back_handler()
{
    if (!is_animating && get_prev_app() != NULL)
    {
        animate_slide(SLIDE_LEFT);
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：find_app_by_id
 *  * 功能描述：根据id查找应用图标
 *  * 输入参数：id {type}
 *  * 返 回 值：AppIcon*
 ***********************************************************************************************************************/
AppIcon *find_app_by_id(int id)
{
    for (int i = 0; i < sizeof(menuNav_app) / sizeof(menuNav_app[0]); i++)
    {
        if (menuNav_app[i].id == id)
        {
            return &menuNav_app[i];
        }
    }
    return NULL;
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：get_prev_app
 *  * 功能描述：Get the prev app object
 *  * 返 回 值：AppIcon*
 ***********************************************************************************************************************/
AppIcon *get_prev_app()
{
    if (!menuNav_current_app)
        return NULL;

    int prev_id = menuNav_current_app->id - 1; 
    if (prev_id < 0)
    {
        return NULL;
    }
    return find_app_by_id(prev_id);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：get_next_app
 *  * 功能描述：Get the next app object
 *  * 返 回 值：AppIcon*
 ***********************************************************************************************************************/
AppIcon *get_next_app()
{
    if (!menuNav_current_app)
        return NULL;

    int next_id = menuNav_current_app->id + 1; 
    int max_id = sizeof(menuNav_app) / sizeof(menuNav_app[0]) - 1;
    if (next_id > max_id)
    {
        return NULL;
    }
    return find_app_by_id(next_id);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：animate_slide
 *  * 功能描述：应用切换动画
 *  * 输入参数：direction {type}
 ***********************************************************************************************************************/
void animate_slide(uint8_t direction)
{
    if (is_animating)
        return;

    is_animating = 1;
    animation_direction = direction;
    animation_step = 0;

    // 保存原始位置
    IconDisplayArea original_positions[3];
    IconDisplayArea target_positions[3];

    for (int i = 0; i < 3; i++)
    {
        original_positions[i] = icon_positions[i];
        target_positions[i] = icon_positions[i];
    }

    // 设置动画目标位置
    if (direction == SLIDE_RIGHT)
    {                                                   
        target_positions[0].x = -40;                    
        target_positions[1].x = original_positions[0].x;
        target_positions[1].y = ICON_Y_NORMAL;
        target_positions[2].x = original_positions[1].x; 
        target_positions[2].y = ICON_Y_RAISED;
    }
    else if (direction == SLIDE_LEFT)
    {                                                    
        target_positions[0].x = original_positions[1].x; 
        target_positions[0].y = ICON_Y_RAISED;
        target_positions[1].x = original_positions[2].x; 
        target_positions[1].y = ICON_Y_NORMAL;
        target_positions[2].x = SCREEN_WIDTH + 40; 
    }

    // 执行动画
    while (animation_step < ANIMATION_STEPS)
    {
        animation_step++;
        float progress = (float)animation_step / ANIMATION_STEPS;

        for (int i = 0; i < 3; i++)
        {
            icon_positions[i].x = original_positions[i].x +(target_positions[i].x - original_positions[i].x) * progress;
            icon_positions[i].y = original_positions[i].y +(target_positions[i].y - original_positions[i].y) * progress;
        }
        menuNavPage_draw();

        vTaskDelay(ANIMATION_DELAY / portTICK_PERIOD_MS);
    }

    // 动画完成后更新当前应用
    if (direction == SLIDE_RIGHT)
    { 
        AppIcon *next_app = get_next_app();
        if (next_app)
        {
            menuNav_current_app = next_app; 
        }
    }
    else if (direction == SLIDE_LEFT)
    { 
        AppIcon *prev_app = get_prev_app();
        if (prev_app)
        {
            menuNav_current_app = prev_app; 
        }
    }

    for (int i = 0; i < 3; i++)
    {
        icon_positions[i] = original_positions[i];
    }

    is_animating = 0;
    animation_direction = 0;
}

