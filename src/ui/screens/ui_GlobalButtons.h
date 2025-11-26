#pragma once
#include "lvgl.h"
#include "Globals.h"                           // for logger, network, menus, etc.
#include "ui/screens/ui_MenuSelectionScreen.h" // your home screen init

namespace ui_GlobalButtons
{
    lv_obj_t *homeButton = nullptr;

    /**
     * @brief Initialize global buttons. Must be called once during setup.
     * @param parent The parent LVGL object (typically lv_scr_act() or a top-level container)
     */
    void initGlobalButtons(lv_obj_t *parent = nullptr)
    {
        if (homeButton)
            return; // already initialized

        if (!parent)
        {
            parent = lv_layer_top();
        }

        // Create small home button
        homeButton = lv_btn_create(parent);
        lv_obj_set_size(homeButton, 80, 40);                       // small button
        lv_obj_align(homeButton, LV_ALIGN_BOTTOM_RIGHT, -10, -20); // bottom-right corner
        lv_obj_set_style_bg_color(homeButton, lv_color_hex(0xF0A31E), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(homeButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        // Label for the button
        lv_obj_t *lbl = lv_label_create(homeButton);
        lv_label_set_text(lbl, "Home");
        lv_obj_center(lbl);

        // Event callback to load home screen
        lv_obj_add_event_cb(
            homeButton,
            [](lv_event_t *e)
            {
                if (!homeButton)
                    return;

                logger.info("Global Home button pressed");
                // Call your home screen init function
                Menus::getInstance().menus.loadMenu("Menu Selection");
            },
            LV_EVENT_CLICKED, nullptr);
    }

    /**
     * @brief Destroy global buttons (if needed, usually on app shutdown)
     */
    void destroyGlobalButtons()
    {
        if (homeButton)
        {
            lv_obj_del(homeButton);
            homeButton = nullptr;
        }
    }
}
