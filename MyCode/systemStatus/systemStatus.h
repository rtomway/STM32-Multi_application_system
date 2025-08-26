#pragma once

typedef enum
{
    APP_HOME,
    APP_MENU,
    APP_ALARM,
    APP_TIMER,
    APP_SETTINGS,
    APP_COUNT
} AppType;

typedef struct SystemState SystemState;

struct SystemState
{
    void (*handle_confirm)(SystemState **current_state);
    void (*handle_cancel)(SystemState **current_state);
    void (*handle_next)(SystemState **current_state);
    void (*handle_back)(SystemState **current_state);
    void (*draw)(void);
    AppType current_app;
};

extern SystemState *g_current_state;
extern SystemState *g_previous_state;

void systemState_init();
void systemState_rollBackOneStep();
void systemState_trans_mainPage(SystemState **current_state);
void systemState_trans_menuNavPage(SystemState **current_state);
void systemState_trans_appAlarmPage(SystemState **current_state);
void systemState_trans_systemSettingsPage(SystemState **current_state);
