#pragma once
#include "ui_GlobalLabels.h"
#include "lvgl.h"
#include "Globals.h"

namespace ui_GlobalLabels
{
    lv_obj_t *networkStatusLabel = nullptr;
    lv_obj_t *userSelectionLabel = nullptr;
    lv_obj_t *menuDisplayLabel = nullptr;
    uint32_t lastNetworkCheck = 0;

    void initNetworkStatus(lv_obj_t *parent)
    {
        logger.info("[ui_GlobalLabels] initNetworkStatus called");

        if (!parent)
        {
            logger.info("[ui_GlobalLabels] No parent provided, using lv_layer_top()");
            parent = lv_layer_top();
        }
        else
        {
            logger.info("[ui_GlobalLabels] Using provided parent: " + String((uintptr_t)parent, HEX));
        }

        if (!networkStatusLabel)
        {
            logger.info("[ui_GlobalLabels] Creating networkStatusLabel");
            networkStatusLabel = lv_label_create(parent);
            lv_label_set_text(networkStatusLabel, "Network: Checking...");
            lv_obj_set_style_text_font(networkStatusLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_align(networkStatusLabel, LV_ALIGN_BOTTOM_LEFT, 10, -5);
            lastNetworkCheck = millis();
        }
        else if (parent != lv_obj_get_parent(networkStatusLabel))
        {
            logger.info("[ui_GlobalLabels] Re-parenting networkStatusLabel");
            lv_obj_set_parent(networkStatusLabel, parent);
        }

        updateNetworkStatus();
    }

    void updateNetworkStatus(lv_obj_t *parent)
    {
        if (!networkStatusLabel)
        {
            logger.warn("[ui_GlobalLabels] updateNetworkStatus called but networkStatusLabel is nullptr");
            return;
        }

        if (parent && parent != lv_obj_get_parent(networkStatusLabel))
        {
            logger.info("[ui_GlobalLabels] Updating parent for networkStatusLabel");
            lv_obj_set_parent(networkStatusLabel, parent);
        }

        bool connected = network->isConnected();
        uint32_t elapsedSec = (millis() - lastNetworkCheck) / 1000;

        lv_label_set_text_fmt(networkStatusLabel, "Network: %s (%lus)",
                              connected ? "Connected" : "Disconnected", elapsedSec);

        lv_obj_set_style_text_color(
            networkStatusLabel,
            connected ? lv_color_hex(0x00FF00) : lv_color_hex(0xFF0000),
            LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_align(networkStatusLabel, LV_ALIGN_BOTTOM_LEFT, 10, -5);
        lv_obj_invalidate(networkStatusLabel);

        logger.info("[ui_GlobalLabels] Network status updated: " + String(connected ? "Connected" : "Disconnected"));
    }

    void initUserSelectionLabel(lv_obj_t *parent)
    {
        logger.info("[ui_GlobalLabels] initUserSelectionLabel called");

        if (!parent)
        {
            logger.info("[ui_GlobalLabels] No parent provided, using lv_layer_top()");
            parent = lv_layer_top();
        }

        if (!userSelectionLabel)
        {
            logger.info("[ui_GlobalLabels] Creating userSelectionLabel");
            userSelectionLabel = lv_label_create(parent);
            lv_label_set_text(userSelectionLabel, "No user selected");
            lv_obj_set_style_text_font(userSelectionLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

            if (networkStatusLabel)
                lv_obj_align_to(userSelectionLabel, networkStatusLabel, LV_ALIGN_OUT_TOP_LEFT, 0, -5);
            else
                lv_obj_align(userSelectionLabel, LV_ALIGN_BOTTOM_LEFT, 10, -25);
        }
        else if (parent != lv_obj_get_parent(userSelectionLabel))
        {
            logger.info("[ui_GlobalLabels] Re-parenting userSelectionLabel");
            lv_obj_set_parent(userSelectionLabel, parent);
        }

        updateUserSelectionLabel(parent);
    }

    void updateUserSelectionLabel(lv_obj_t *parent)
    {
        if (!userSelectionLabel)
        {
            logger.warn("[ui_GlobalLabels] updateUserSelectionLabel called but userSelectionLabel is nullptr");
            return;
        }

        if (parent && parent != lv_obj_get_parent(userSelectionLabel))
        {
            logger.info("[ui_GlobalLabels] Updating parent for userSelectionLabel");
            lv_obj_set_parent(userSelectionLabel, parent);
        }

        String userName = currentUser.getName();
        bool hasUser = !userName.isEmpty();

        lv_label_set_text_fmt(userSelectionLabel, "%s", hasUser ? userName.c_str() : "No user selected");

        lv_obj_set_style_text_color(
            userSelectionLabel,
            hasUser ? lv_color_hex(0x00FF00) : lv_color_hex(0xFF0000),
            LV_PART_MAIN | LV_STATE_DEFAULT);

        // Keep alignment consistent with network label
        if (networkStatusLabel)
            lv_obj_align_to(userSelectionLabel, networkStatusLabel, LV_ALIGN_OUT_TOP_LEFT, 0, -5);

        lv_obj_invalidate(userSelectionLabel);

        logger.info("[ui_GlobalLabels] User selection updated: " + (hasUser ? userName : "No user selected"));
    }

    void destroyLabels()
    {
        if (networkStatusLabel)
        {
            lv_obj_del(networkStatusLabel);
            networkStatusLabel = nullptr;
            logger.info("[ui_GlobalLabels] networkStatusLabel destroyed");
        }

        if (userSelectionLabel)
        {
            lv_obj_del(userSelectionLabel);
            userSelectionLabel = nullptr;
            logger.info("[ui_GlobalLabels] userSelectionLabel destroyed");
        }

        if (menuDisplayLabel)
        {
            lv_obj_del(menuDisplayLabel);
            menuDisplayLabel = nullptr;
            logger.info("[ui_GlobalLabels] menuDisplayLabel destroyed");
        }
    }
}
