#include "lvgl.h"
#include "Globals.h" // for logger
#include "ui/ui.h"

lv_obj_t *ui_DebugLogScreen = nullptr;
lv_obj_t *ui_LogContainer = nullptr;
lv_obj_t *ui_LogLabel = nullptr;

// Timer callback for checking network status and showing back button
static void backButtonTimerCallback(lv_timer_t *timer)
{
    lv_obj_t *btnBack = static_cast<lv_obj_t *>(lv_timer_get_user_data(timer));
    int status = network->getStatus();

    switch (status)
    {
    case WL_CONNECTED:
    case WL_NO_SSID_AVAIL:
    case WL_CONNECT_FAILED:
    case WL_CONNECTION_LOST:
    case WL_DISCONNECTED:
        lv_obj_clear_flag(btnBack, LV_OBJ_FLAG_HIDDEN);
        lv_timer_del(timer);
        break;
    default:
        // still connecting, do nothing
        break;
    }
}

void ui_DebugLog_screen_init()
{
    // Main screen
    ui_DebugLogScreen = lv_obj_create(nullptr);
    lv_obj_set_style_bg_color(ui_DebugLogScreen, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_DebugLogScreen, 255, LV_PART_MAIN);

    // Scrollable log container
    ui_LogContainer = lv_obj_create(ui_DebugLogScreen);
    lv_obj_set_size(ui_LogContainer, LV_HOR_RES, LV_VER_RES - 60);
    lv_obj_center(ui_LogContainer);
    lv_obj_set_scroll_dir(ui_LogContainer, LV_DIR_VER);
    lv_obj_set_style_bg_color(ui_LogContainer, lv_color_hex(0x101010), LV_PART_MAIN);
    lv_obj_set_style_pad_all(ui_LogContainer, 10, 0);

    // Log label
    ui_LogLabel = lv_label_create(ui_LogContainer);
    lv_label_set_text(ui_LogLabel, "");
    lv_label_set_long_mode(ui_LogLabel, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(ui_LogLabel, LV_PCT(100));
    lv_obj_set_style_text_color(ui_LogLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    // Back button (hidden initially)
    lv_obj_t *btnBack = lv_button_create(ui_DebugLogScreen);
    lv_obj_align(btnBack, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    lv_obj_add_flag(btnBack, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t *lbl = lv_label_create(btnBack);
    lv_label_set_text(lbl, "Back");
    lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    lv_obj_add_event_cb(
        btnBack,
        [](lv_event_t *e)
        {
            lv_scr_load(ui_WiFiConnectScreen);
        },
        LV_EVENT_CLICKED, nullptr);

    // Timer to check network status
    lv_timer_create(backButtonTimerCallback, 500, btnBack);

    // Load the screen
    lv_scr_load(ui_DebugLogScreen);

    // Seed logs with existing buffered text
    lv_label_set_text(ui_LogLabel, logger.getLogText().c_str());

    // Logger callback for real-time updates
    logger.setCallback(
        [](const String &line)
        {
            if (!ui_LogLabel)
                return;

            static String buffer;
            buffer += line + "\n";
            lv_label_set_text(ui_LogLabel, buffer.c_str());
            lv_obj_scroll_to_y(ui_LogContainer, LV_COORD_MAX, LV_ANIM_OFF);
        });
}
