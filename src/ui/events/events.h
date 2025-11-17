#pragma once

#include "lvgl.h"

#ifdef __cplusplus
extern "C"
{
#endif
    void handleUserDropdownEvent(lv_event_t *e);
    void handleConnectWiFiButton(lv_event_t *e);
    void handleWifiDropdownEvent(lv_event_t *e);

#ifdef __cplusplus
} /* extern "C" */
#endif
