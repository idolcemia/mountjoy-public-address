#include "ui_GlobalLabels.h"

namespace ui_GlobalLabels
{
    lv_obj_t *networkStatusLabel = nullptr;
    lv_obj_t *userSelectionLabel = nullptr;
    lv_obj_t *menuDisplayLabel = nullptr;
    uint32_t lastNetworkCheck = 0;

    void initNetworkStatus(lv_obj_t *parent)
    {
        // --- Network Label (bottom-left) ---
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

    void networkChecked()
    {
        lastNetworkCheck = millis();
    }

    // --- User Selection Label ---
    void initUserSelectionLabel(lv_obj_t *parent)
    {
        userSelectionLabel = lv_label_create(parent);
        lv_label_set_text(userSelectionLabel, "No user selected");
        lv_obj_set_style_text_font(userSelectionLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

        // Align above network label
        if (networkStatusLabel)
        {
            lv_obj_align_to(userSelectionLabel, networkStatusLabel, LV_ALIGN_OUT_TOP_LEFT, 0, -5);
        }
        else
        {
            lv_obj_align(userSelectionLabel, LV_ALIGN_BOTTOM_LEFT, 10, -25); // fallback position
        }

        updateUserSelectionLabel();
    }

    void updateUserSelectionLabel()
    {
        if (!userSelectionLabel)
            return;

        String userName = currentUser.getName();

        bool hasUser = !userName.isEmpty();

        lv_label_set_text_fmt(
            userSelectionLabel,
            "%s",
            hasUser ? userName.c_str() : "No user selected");

        lv_obj_set_style_text_color(
            userSelectionLabel,
            hasUser ? lv_color_hex(0x00FF00) : lv_color_hex(0xFF0000),
            LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_invalidate(userSelectionLabel);
    }

}
