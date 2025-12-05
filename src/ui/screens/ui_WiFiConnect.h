#ifndef UI_WIFICONNECT_H
#define UI_WIFICONNECT_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // SCREEN: ui_WiFiConnectScreen
    extern void ui_WiFiConnect_screen_init(void);
    extern void ui_WiFiConnectScreenUpdate();
    extern void ui_WiFiConnect_screen_destroy(void);
    extern lv_obj_t *ui_WiFiConnectScreen;

    // Dropdown / list of available networks
    extern lv_obj_t *ui_WiFiDropdown;
    extern void ui_event_wifi_dropdown(lv_event_t *e);

    // Connect button
    extern lv_obj_t *ui_ConnectWiFiButton;
    extern void ui_event_ConnectWiFiButton(lv_event_t *e);

    // Helpers
    // static void _performWiFiScan();

    // Labels for SSID selection and status messages
    extern lv_obj_t *ui_SelectSSIDLabel;
    extern lv_obj_t *ui_WiFiStatusLabel;

    // CUSTOM VARIABLES (optional)
    extern lv_obj_t *uic_SelectSSIDLabel;
    extern lv_obj_t *uic_ConnectWiFiButton;
    extern lv_obj_t *uic_WiFiStatusLabel;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
