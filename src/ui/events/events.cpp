#include <Arduino.h>
#include "events.h"
#include "ui/ui.h"
#include "Globals.h"
#include "ui/screens/ui_DebugLogScreen.h"
#include "ui/screens/ui_MenuSelectionScreen.h"
#include "ui/screens/labels/ui_GlobalLabels.h"
#include "ui/screens/ui_GlobalButtons.h"

void handleMenuSelectionDropdownEvent(lv_event_t *e)
{
    uint16_t selected = getDropdownSelection(e);
}

void handleMenuSelectionButtonEvent(lv_event_t *e)
{
    logger.info("Menu selection button pressed");

    if (!ui_MenuDropdown)
    {
        return;
    }

    uint16_t selected = lv_dropdown_get_selected(ui_MenuDropdown);
    logger.info("Dropdown selected index: " + String(selected));

    char buf[64];
    lv_dropdown_get_selected_str(ui_MenuDropdown, buf, sizeof(buf));
    String selectedText = String(buf);
    logger.info("Dropdown selected text: " + selectedText);

    menuManager.setCachedScreen(PasteurizerMenu::MENU_MENU_SELECTION, ui_MenuSelectionScreen);
    menuManager.queueMenu(selectedText);
    // menuManager.loadMenu(selectedText);
    // ui_MenuSelection_screen_destroy();
    // Menus::getInstance().menus.loadMenu(selectedText);
    // lv_obj_clean(lv_scr_act());

    // uint16_t selected = getDropdownSelection(e);
}

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

void handleConfirmUserButton(lv_event_t *e)
{
    logger.info("[events] Confirm user button pressed.");

    char buf[64];
    lv_dropdown_get_selected_str(ui_UserDropdown, buf, sizeof(buf));
    String selectedText = String(buf);
    logger.info("[events] Dropdown selected text: " + selectedText);

    _setCurrentUser(selectedText);
    ui_GlobalLabels::updateUserSelectionLabel(ui_UserSelectionScreen);
}

void handleConnectWiFiButton(lv_event_t *e)
{

    // Load the debug screen before connecting
    // ui_DebugLog_screen_init();

    logger.info("Starting WiFi connection...");

    if (!ui_WiFiDropdown)
        return;

    char ssid[64];
    lv_dropdown_get_selected_str(ui_WiFiDropdown, ssid, sizeof(ssid));
    if (strlen(ssid) == 0)
        return;

    // --- Disable button while connecting ---
    lv_obj_add_state(ui_ConnectWiFiButton, LV_STATE_DISABLED);

    // Change button label to "Connecting..."
    lv_obj_t *label = lv_obj_get_child(ui_ConnectWiFiButton, 0);
    lv_label_set_text(label, "Connecting...");

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
    bool connected = (WiFi.status() == WL_CONNECTED);
    network->printStatus();

    ui_GlobalLabels::networkChecked();

    // --- FINAL RESULT ---
    lv_label_set_text_fmt(
        ui_WiFiStatusLabel,
        "Status: %s",
        WiFi.status() == WL_CONNECTED ? "Connected" : "Failed");
    lv_obj_invalidate(ui_WiFiStatusLabel);

    // --- Update button after result ---
    lv_obj_clear_state(ui_ConnectWiFiButton, LV_STATE_DISABLED);

    if (connected)
    {
        lv_label_set_text(label, "Next"); // change button text
    }
    else
    {
        lv_label_set_text(label, "Connect"); // reset to original
    }

    menuManager.queueMenu(PasteurizerMenu::MENU_DEBUG_LOG);

    lv_refr_now(NULL);
}

void handleWifiDropdownEvent(lv_event_t *e) {}

uint16_t getDropdownSelection(lv_event_t *e)
{
    // Cast void* to lv_obj_t* (required in LVGL 9)
    lv_obj_t *dd = (lv_obj_t *)lv_event_get_target(e);
    uint16_t selected = lv_dropdown_get_selected(dd);

    logger.info("Dropdown1 selected index: ");
    logger.info(selected);

    return selected;
}