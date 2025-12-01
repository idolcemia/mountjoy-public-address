#pragma once

#include "lvgl.h"

#ifdef __cplusplus
extern "C"
{
#endif
    void handleUserDropdownEvent(lv_event_t *e);
    void handleConfirmUserButton(lv_event_t *e);
    
    void handleConnectWiFiButton(lv_event_t *e);
    void handleWifiDropdownEvent(lv_event_t *e);

    void handleMenuSelectionDropdownEvent(lv_event_t *e);
    void handleMenuSelectionButtonEvent(lv_event_t *e);

    // HELPERS
    uint16_t getDropdownSelection(lv_event_t *e);

#ifdef __cplusplus
} /* extern "C" */
#endif
