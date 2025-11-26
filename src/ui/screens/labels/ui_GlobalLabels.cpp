#include "ui_GlobalLabels.h"

namespace ui_GlobalLabels
{
    lv_obj_t *networkStatusLabel = nullptr;
    lv_obj_t *menuDisplayLabel = nullptr;
    uint32_t lastNetworkCheck = 0;

    void initNetworkStatus(lv_obj_t *parent)
    {
        // Create label at bottom-left
        networkStatusLabel = lv_label_create(parent);
        lv_label_set_text(networkStatusLabel, "Network: Checking...");
        lv_obj_set_style_text_font(networkStatusLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_align(networkStatusLabel, LV_ALIGN_BOTTOM_LEFT, 10, -5);

        lastNetworkCheck = millis();

        // Initial update
        updateNetworkStatus();
    }

    void updateNetworkStatus()
    {
        if (!networkStatusLabel)
            return;

        bool connected = network->isConnected();
        uint32_t elapsedSec = (millis() - lastNetworkCheck) / 1000;

        lv_label_set_text_fmt(
            networkStatusLabel,
            "Network: %s (%lus)",
            connected ? "Connected" : "Disconnected",
            elapsedSec);

        lv_obj_set_style_text_color(
            networkStatusLabel,
            connected ? lv_color_hex(0x00FF00) : lv_color_hex(0xFF0000),
            LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_invalidate(networkStatusLabel);
    }

    // Only update last check time when network is checked:
    void networkChecked()
    {
        lastNetworkCheck = millis();
    }
}
