#include <Arduino.h>
#include "events.h"
#include "ui/ui.h"
#include "Globals.h"

void handleUserDropdownEvent(lv_event_t *e)
{
    // Cast void* to lv_obj_t* (required in LVGL 9)
    lv_obj_t *dd = (lv_obj_t *)lv_event_get_target(e);
    uint16_t selected = lv_dropdown_get_selected(dd);

    logger.info("Dropdown1 selected index: ");
    logger.info(selected);

    if (users && selected < users->getUserCount())
    {
        const char *name = users->getUsers()[selected].getName().c_str();
        lv_label_set_text(ui_UserDisplayLabel, name);

        logger.info("Dropdown1 selected name: ");
        logger.info(String(name));
    }
    else
    {
        logger.info("Unknown name");
        lv_label_set_text(ui_UserDisplayLabel, "Unknown");
    }

    // Refresh label to show new text
    lv_obj_invalidate(ui_UserDisplayLabel);
}

void handleConnectWiFiButton(lv_event_t *e)
{
    if (!ui_WiFiDropdown)
        return;

    char ssid[64];
    lv_dropdown_get_selected_str(ui_WiFiDropdown, ssid, sizeof(ssid));
    if (strlen(ssid) == 0)
        return;

    // --- SHOW CONNECTING ---
    lv_label_set_text(ui_WiFiStatusLabel, "Connecting...");
    lv_obj_invalidate(ui_WiFiStatusLabel);

    // Force LVGL to draw before blocking
    lv_refr_now(NULL);  // <--- STRONGEST WAY
    lv_timer_handler(); // also ok
    delay(20);          // tiny delay to allow flush

    logger.info(String("[WiFiScreen] Connecting to: ") + ssid);

    // --- BLOCKING CONNECT ---
    network->begin();
    network->printStatus();

    // --- FINAL RESULT ---
    lv_label_set_text_fmt(
        ui_WiFiStatusLabel,
        "Status: %s",
        WiFi.status() == WL_CONNECTED ? "Connected" : "Failed");
    lv_obj_invalidate(ui_WiFiStatusLabel);
}

void handleWifiDropdownEvent(lv_event_t *e) {}
