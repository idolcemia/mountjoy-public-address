// ui_WiFiConnect.cpp
// Production-ready WiFi Connect screen modeled after ui_UserSelection

#include "ui/ui.h"
#include "Globals.h"
#include "ui/events/events.h"
#include "ui_WiFiConnect.h"
#include "lvgl.h"

// mirror the pattern used by ui_UserSelection
lv_obj_t *uic_WiFiStatusLabel;
lv_obj_t *uic_SelectSSIDLabel;
lv_obj_t *uic_ConnectWiFiButton;
lv_obj_t *ui_WiFiConnectScreen = NULL;
lv_obj_t *ui_WiFiDropdown = NULL;
lv_obj_t *ui_ConnectWiFiButton = NULL;
lv_obj_t *ui_SelectSSIDLabel = NULL;
lv_obj_t *ui_WiFiStatusLabel = NULL;

static String buildDropdownOptionsFromScan()
{
    String options;
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; i++)
    {
        options += WiFi.SSID(i);
        if (i < n - 1)
            options += '\n';
    }
    return options;
}

// events are thin — actual logic in your handlers (events.h / events.cpp)
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

void ui_WiFiConnect_screen_init(void)
{
    ui_WiFiConnectScreen = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_WiFiConnectScreen, LV_OBJ_FLAG_SCROLLABLE);

    // Background styling to match UserSelection screen
    lv_obj_set_style_bg_color(ui_WiFiConnectScreen, lv_color_hex(0xEE7B01), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_WiFiConnectScreen, 255, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(ui_WiFiConnectScreen, lv_color_hex(0x2E1A05), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(ui_WiFiConnectScreen, LV_GRAD_DIR_VER, LV_PART_MAIN);

    // BuildShift brand image (same as user selection)
    lv_obj_t *ui_BSLogo = lv_image_create(ui_WiFiConnectScreen);
    lv_image_set_src(ui_BSLogo, &ui_img_buildshift_brand_png);
    lv_obj_set_align(ui_BSLogo, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_BSLogo, -157);

    // Title / select label
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

    // Dropdown (populated from WiFi scan)
    ui_WiFiDropdown = lv_dropdown_create(ui_WiFiConnectScreen);

    String options = buildDropdownOptionsFromScan();
    if (options.length() == 0)
        options = "No networks found";
    lv_dropdown_set_options(ui_WiFiDropdown, options.c_str());

    lv_obj_set_width(ui_WiFiDropdown, 314);
    lv_obj_set_height(ui_WiFiDropdown, LV_SIZE_CONTENT);

    // **FIX: proper centered layout, no conflicting x/y forcing**
    lv_obj_align(ui_WiFiDropdown, LV_ALIGN_CENTER, 0, 50);

    lv_obj_add_flag(ui_WiFiDropdown, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_set_style_bg_color(ui_WiFiDropdown, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_WiFiDropdown, 255, LV_PART_MAIN);

    // Connect Button (centered)
    ui_ConnectWiFiButton = lv_button_create(ui_WiFiConnectScreen);
    lv_obj_set_size(ui_ConnectWiFiButton, 266, 50);
    lv_obj_align(ui_ConnectWiFiButton, LV_ALIGN_CENTER, 0, 150);
    lv_obj_set_style_bg_color(ui_ConnectWiFiButton, lv_color_hex(0xF0A31E), LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_ConnectWiFiButton, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    lv_obj_t *ui_ConnectLabel = lv_label_create(ui_ConnectWiFiButton);
    lv_label_set_text(ui_ConnectLabel, "Connect");
    lv_obj_set_style_text_font(ui_ConnectLabel, &lv_font_montserrat_40, LV_PART_MAIN);
    lv_obj_align(ui_ConnectLabel, LV_ALIGN_CENTER, 0, 0);

    // Status Label (centered above button)
    ui_WiFiStatusLabel = lv_label_create(ui_WiFiConnectScreen);
    lv_label_set_text(ui_WiFiStatusLabel, "Status: Idle");
    lv_obj_set_style_text_color(ui_WiFiStatusLabel, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui_WiFiStatusLabel, &lv_font_montserrat_40, LV_PART_MAIN);

    lv_obj_align_to(ui_WiFiStatusLabel,
                    ui_ConnectWiFiButton,
                    LV_ALIGN_OUT_TOP_MID,
                    0, -10);

    // Hook events
    lv_obj_add_event_cb(ui_WiFiDropdown, ui_event_wifi_dropdown, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ConnectWiFiButton, ui_event_ConnectWiFiButton, LV_EVENT_ALL, NULL);

    // Expose uic_ vars
    uic_SelectSSIDLabel = ui_SelectSSIDLabel;
    uic_ConnectWiFiButton = ui_ConnectWiFiButton;
    uic_WiFiStatusLabel = ui_WiFiStatusLabel;

    // Load this screen
    lv_scr_load(ui_WiFiConnectScreen);
}

void ui_WiFiConnect_screen_destroy(void)
{
    if (ui_WiFiConnectScreen)
        lv_obj_del(ui_WiFiConnectScreen);

    ui_WiFiConnectScreen = NULL;
    ui_WiFiDropdown = NULL;
    uic_SelectSSIDLabel = NULL;
    ui_SelectSSIDLabel = NULL;
    uic_ConnectWiFiButton = NULL;
    ui_ConnectWiFiButton = NULL;
    uic_WiFiStatusLabel = NULL;
    ui_WiFiStatusLabel = NULL;
}
