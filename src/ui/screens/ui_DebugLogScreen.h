#ifndef UI_DEBUGLOG_H
#define UI_DEBUGLOG_H

#include "lvgl.h"

// Forward declaration of the WiFi connect screen
extern lv_obj_t *ui_WiFiConnectScreen;

// Debug Log screen objects
extern lv_obj_t *ui_DebugLogScreen;
extern lv_obj_t *ui_LogContainer;
extern lv_obj_t *ui_LogLabel;

// Initialize the Debug Log screen
void ui_DebugLog_screen_init();

#endif // UI_DEBUGLOG_H
