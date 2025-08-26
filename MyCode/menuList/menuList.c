#include "menuList.h"
#include "u8g2/u8g2.h"
#include "myU8g2/myU8g2.h"
#include "string.h"

/******************************************************************************
 * @author xu
 *  * 函数名称：Add_Option_List
 *  * 功能描述：向菜单中添加选项列表
 *  * 输入参数：menu {type}
 *  * 输入参数：option_list {type}
 *  * 输入参数：size {type}
 *******************************************************************************/
void Add_Option_List(Menu_Typedef *menu, Menu_Option_Typedef *option_list, int size)
{
    if (option_list == NULL || menu == NULL)
    {
        return;
    }

    Menu_Option_Typedef *head = &option_list[0];
    for (int i = 1; i < size; i++)
    {
        // 组建链表
        option_list[i - 1].next = &option_list[i];
        option_list[i].prev = &option_list[i - 1];
    }
    menu->option_num = size;
    menu->options = head;
}

/******************************************************************************
 * @author xu
 *  * 函数名称：Add_Menu
 *  * 功能描述：添加菜单
 *  * 输入参数：menu_name {type}
 *  * 输入参数：menu {type}
 *  * 输入参数：parent_menu {type}
 *  * 输入参数：option_name {type}
 *******************************************************************************/
void Add_Menu(const char *menu_name, Menu_Typedef *menu, Menu_Typedef *parent_menu, const char *option_name)
{
    // 设置menu结构体中的成员
    menu->menu_name = menu_name;
    menu->parent_menu = parent_menu;
    menu->current_index = 0;

    if (parent_menu == NULL)
    {
        return;
    }
    Menu_Option_Typedef *option = parent_menu->options;
    // 逐个遍历上一级option，直到找到当前menu所属的option
    while (option != NULL)
    {
        // 将上一级option绑定上当前menu
        if (option->option_name == option_name)
        {
            option->child_menu = menu;
            return;
        }
        option = option->next;
    }
}

/******************************************************************************
 * @author xu
 *  * 函数名称：Draw_Menu
 *  * 功能描述：绘制指定菜单
 *  * 输入参数：menu {type}
 *******************************************************************************/
void Draw_Menu(Menu_Typedef *menu)
{
    for (int i = 0; i < menu->option_num; i++)
    {
        Menu_Option_Typedef *option = &menu->options[i];
        option->position.x = SCREEN_START_X + PARAGRAPH_SPACING;
        option->position.y = i * (FONT_HEIGHT + FONT_SPACING) + SCREEN_START_Y;
        u8g2_DrawStr(&u8g2, option->position.x, option->position.y, option->option_name);
    }
}

/******************************************************************************
 * @author xu
 *  * 函数名称：Get_Option_By_Index
 *  * 功能描述：获取指定索引的选项
 *  * 输入参数：menu {type}
 *  * 输入参数：index {type}
 *  * 返 回 值：Menu_Option_Typedef
 *******************************************************************************/
Menu_Option_Typedef Get_Option_By_Index(Menu_Typedef *menu, int index)
{
    Menu_Option_Typedef option = menu->options[index];
    return option;
}

/******************************************************************************
 * @author xu
 *  * 函数名称：Draw_Cursor
 *  * 功能描述：绘制当前光标位置
 *  * 输入参数：menu {type}
 *******************************************************************************/
void Draw_Cursor(Menu_Typedef *menu)
{
    int cursor = menu->current_index;

    Menu_Option_Typedef option = Get_Option_By_Index(menu, cursor);

    int cursor_x = option.position.x - CURSOR_PADDING;
    int cursor_y = option.position.y - CURSOR_PADDING - FONT_HEIGHT / 2 - 2;
    int text_width = strlen(option.option_name) * FONT_WIDTH;
    // 画空心矩形
    u8g2_DrawFrame(&u8g2, cursor_x, cursor_y, text_width + 2 * CURSOR_PADDING, FONT_HEIGHT + CURSOR_PADDING);
}

/******************************************************************************
 * @author xu
 *  * 函数名称：Move_Cursor
 *  * 功能描述：移动光标位置
 *  * 输入参数：menu {type}
 *  * 输入参数：direction {type}
 *******************************************************************************/
void Move_Cursor(Menu_Typedef *menu, int direction)
{
    int option_num = menu->option_num;
    if(direction == 1)
    {
        menu->current_index = (++menu->current_index) % option_num;
    }else if(direction == 0)
    {
        if (menu->current_index == 0)
        {
            menu->current_index = option_num-1;
        }
        else
        {
            menu->current_index = (--menu->current_index) % option_num;
        }
    }
}
