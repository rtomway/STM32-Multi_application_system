#ifndef __MENULIST_H
#define __MENULIST_H
#include "main.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_START_X 0
#define SCREEN_START_Y 15

#define FONT_HEIGHT 15
#define FONT_SPACING 5
#define FONT_WIDTH 9

#define CURSOR_WIDTH 80
#define CURSOR_PADDING 5

// 段前间距
#define PARAGRAPH_SPACING 5

typedef struct Menu_Option_Typedef
{
    const char *option_name;
    void (*func)(void);

    struct position
    {
        int x;
        int y;
    } position;

    struct Menu_Option_Typedef *next;
    struct Menu_Option_Typedef *prev;

    struct Menu_Typedef *child_menu;
} Menu_Option_Typedef;

typedef struct Menu_Typedef
{
    const char *menu_name;
    Menu_Option_Typedef *options;
    struct Menu_Typedef *parent_menu;
    uint8_t current_index;
    uint8_t option_num;
} Menu_Typedef;

void Add_Option_List(Menu_Typedef *menu, Menu_Option_Typedef *option_list, int size);
void Add_Menu(const char *menu_name, Menu_Typedef *menu, Menu_Typedef *parent_menu, const char *option_name);
Menu_Option_Typedef Get_Option_By_Index(Menu_Typedef *menu, int index);
void Draw_Menu(Menu_Typedef *menu);
void Draw_Cursor(Menu_Typedef *menu);
void Move_Cursor(Menu_Typedef *menu, int direction);


#endif // !__MENULIST_H
