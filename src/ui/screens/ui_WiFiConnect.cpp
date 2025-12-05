#include "ui/ui.h"
#include "ui/screens/labels/ui_GlobalLabels.h"
#include "ui/screens/ui_GlobalButtons.h"
#include "Globals.h"
#include "ui/events/events.h"
#include "ui_WiFiConnect.h"
#include "lvgl.h"

// Screen objects
lv_obj_t *ui_WiFiConnectScreen = nullptr;
lv_obj_t *ui_WiFiDropdown = nullptr;
lv_obj_t *ui_ConnectWiFiButton = nullptr;
lv_obj_t *ui_SelectSSIDLabel = nullptr;
lv_obj_t *ui_WiFiStatusLabel = nullptr;

// Event handlers
void ui_event_wifi_dropdown(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED)
    {
        handleWifiDropdownEvent(e);
    }
}

void ui_event_ConnectWiFiButton(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        handleConnectWiFiButton(e);
    }
}

// ---------------------------
// Private helper: perform WiFi scan and update UI
// ---------------------------
static void _performWiFiScan()
{
    String options;

    // Try scanning networks
    int n = -1;
    try
    {
        n = WiFi.scanNetworks();
    }
    catch (...)
    {
        logger.error("[uiWiFiConnect] WiFi scan failed");
        lv_label_set_text(ui_WiFiStatusLabel, "Status: Scan error");
        lv_dropdown_set_options(ui_WiFiDropdown, "Error");
        return;
    }

    if (n > 0)
    {
        for (int i = 0; i < n; i++)
        {
            options += WiFi.SSID(i);
            if (i < n - 1)
                options += "\n";
        }
    }
    else
    {
        options = "No networks found";
    }

    lv_dropdown_set_options(ui_WiFiDropdown, options.c_str());
    lv_label_set_text(ui_WiFiStatusLabel, "Status: Done");
}

// ---------------------------
// Initialize WiFi Connect screen
// ---------------------------
void ui_WiFiConnect_screen_init(void)
{
    ui_WiFiConnectScreen = lv_obj_create(nullptr);
    lv_obj_remove_flag(ui_WiFiConnectScreen, LV_OBJ_FLAG_SCROLLABLE);

    // Background styling
    lv_obj_set_style_bg_color(ui_WiFiConnectScreen, lv_color_hex(0xEE7B01), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_WiFiConnectScreen, 255, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(ui_WiFiConnectScreen, lv_color_hex(0x2E1A05), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(ui_WiFiConnectScreen, LV_GRAD_DIR_VER, LV_PART_MAIN);

    // Logo
    lv_obj_t *ui_BSLogo = lv_image_create(ui_WiFiConnectScreen);
    lv_image_set_src(ui_BSLogo, &ui_img_buildshift_brand_png);
    lv_obj_set_align(ui_BSLogo, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_BSLogo, -157);

    // Title
    ui_SelectSSIDLabel = lv_label_create(ui_WiFiConnectScreen);
    lv_label_set_text(ui_SelectSSIDLabel, "Select WiFi Network");
    lv_obj_set_style_text_color(ui_SelectSSIDLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui_SelectSSIDLabel, &lv_font_montserrat_48, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_SelectSSIDLabel, lv_color_hex(0x956207), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_SelectSSIDLabel, 255, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(ui_SelectSSIDLabel, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(ui_SelectSSIDLabel, LV_GRAD_DIR_VER, LV_PART_MAIN);
    lv_obj_set_align(ui_SelectSSIDLabel, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_SelectSSIDLabel, -45);

    // Dropdown
    ui_WiFiDropdown = lv_dropdown_create(ui_WiFiConnectScreen);
    lv_dropdown_set_options(ui_WiFiDropdown, "Scanning...");
    lv_obj_set_width(ui_WiFiDropdown, 314);
    lv_obj_set_height(ui_WiFiDropdown, LV_SIZE_CONTENT);
    lv_obj_align(ui_WiFiDropdown, LV_ALIGN_CENTER, 0, 50);
    lv_obj_set_style_bg_color(ui_WiFiDropdown, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_WiFiDropdown, 255, LV_PART_MAIN);

    // Connect button
    ui_ConnectWiFiButton = lv_button_create(ui_WiFiConnectScreen);
    lv_obj_set_size(ui_ConnectWiFiButton, 266, 50);
    lv_obj_align(ui_ConnectWiFiButton, LV_ALIGN_CENTER, 0, 150);
    lv_obj_set_style_bg_color(ui_ConnectWiFiButton, lv_color_hex(0xF0A31E), LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_ConnectWiFiButton, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    lv_obj_t *ui_ConnectLabel = lv_label_create(ui_ConnectWiFiButton);
    lv_label_set_text(ui_ConnectLabel, "Connect");
    lv_obj_set_style_text_font(ui_ConnectLabel, &lv_font_montserrat_40, LV_PART_MAIN);
    lv_obj_align(ui_ConnectLabel, LV_ALIGN_CENTER, 0, 0);

    // Status label
    ui_WiFiStatusLabel = lv_label_create(ui_WiFiConnectScreen);
    lv_label_set_text(ui_WiFiStatusLabel, "Status: Scanning...");
    lv_obj_set_style_text_color(ui_WiFiStatusLabel, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui_WiFiStatusLabel, &lv_font_montserrat_40, LV_PART_MAIN);
    lv_obj_align_to(ui_WiFiStatusLabel, ui_ConnectWiFiButton, LV_ALIGN_OUT_TOP_MID, 0, -10);

    // Event callbacks
    lv_obj_add_event_cb(ui_WiFiDropdown, ui_event_wifi_dropdown, LV_EVENT_ALL, nullptr);
    lv_obj_add_event_cb(ui_ConnectWiFiButton, ui_event_ConnectWiFiButton, LV_EVENT_ALL, nullptr);

    // Timer to perform WiFi scan once after screen loads
    lv_timer_create(
        [](lv_timer_t *t)
        {
            _performWiFiScan();
            lv_timer_del(t);
        },
        250, nullptr);

    // Load screen
    lv_scr_load(ui_WiFiConnectScreen);
    ui_GlobalButtons::updateGlobalButtons(ui_WiFiConnectScreen);
    ui_GlobalLabels::updateNetworkStatus(ui_WiFiConnectScreen);
    ui_GlobalLabels::updateUserSelectionLabel(ui_WiFiConnectScreen);

    menuManager.setCachedScreen(PasteurizerMenu::MENU_WIFI_CONNECT, ui_WiFiConnectScreen);
}

// ---------------------------
// Update WiFi Connect screen (re-scan and refresh UI)
// ---------------------------
void ui_WiFiConnectScreenUpdate()
{
    if (!ui_WiFiConnectScreen)
        return; // Screen not initialized

    // Reset status
    lv_label_set_text(ui_WiFiStatusLabel, "Status: Scanning...");
    lv_dropdown_set_options(ui_WiFiDropdown, "Scanning...");

    // Perform scan
    lv_timer_create(
        [](lv_timer_t *t)
        {
            _performWiFiScan();
            lv_timer_del(t);
        },
        150, nullptr);

    // Ensure event callbacks still exist
    lv_obj_remove_event_cb(ui_WiFiDropdown, ui_event_wifi_dropdown);
    lv_obj_add_event_cb(ui_WiFiDropdown, ui_event_wifi_dropdown, LV_EVENT_ALL, nullptr);

    lv_obj_remove_event_cb(ui_ConnectWiFiButton, ui_event_ConnectWiFiButton);
    lv_obj_add_event_cb(ui_ConnectWiFiButton, ui_event_ConnectWiFiButton, LV_EVENT_ALL, nullptr);

    // Reset button state
    lv_obj_clear_state(ui_ConnectWiFiButton, LV_STATE_DISABLED);

    // Update global labels
    ui_GlobalLabels::updateNetworkStatus(ui_WiFiConnectScreen);
    ui_GlobalLabels::updateUserSelectionLabel(ui_WiFiConnectScreen);
    ui_GlobalButtons::updateGlobalButtons(ui_WiFiConnectScreen);

    menuManager.setCachedScreen(PasteurizerMenu::MENU_WIFI_CONNECT, ui_WiFiConnectScreen);
}

// ---------------------------
// Destroy WiFi Connect screen
// ---------------------------
void ui_WiFiConnect_screen_destroy(void)
{
    if (ui_WiFiConnectScreen)
    {
        lv_obj_clean(ui_WiFiConnectScreen); // remove children safely
    }

    ui_WiFiDropdown = nullptr;
    ui_SelectSSIDLabel = nullptr;
    ui_ConnectWiFiButton = nullptr;
    ui_WiFiStatusLabel = nullptr;
}
