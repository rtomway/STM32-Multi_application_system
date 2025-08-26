#ifndef MENU_NAV_PAGE_H
#define MENU_NAV_PAGE_H

#include "systemStatus/systemStatus.h"
#include "stdint.h"

// 图标显示区域
typedef struct
{
    int x;
    int y;
    int width;
    int height;
} IconDisplayArea;


// 应用图标结构体
typedef struct
{
    const char *name;
    int id;
    const uint8_t *icon_data;
    IconDisplayArea display_area;
} AppIcon;

typedef enum 
{
    SLIDE_RIGHT,
    SLIDE_LEFT,
}SlideDirection;

void menuNavPage_init();
void menuNavPage_run();
void menuNavPage_draw();
void menuNavPage_confirm_handler();
void menuNavPage_cancle_handler();
void menuNavPage_next_handler();
void menuNavPage_back_handler();

extern SystemState g_menuNavPage_state;

AppIcon *find_app_by_id(int id);
AppIcon *get_prev_app();
AppIcon *get_next_app();
void animate_slide(uint8_t direction);

#endif // MENU_NAV_PAGE_H