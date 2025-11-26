#ifndef UI_MENU_SELECTION_SCREEN_H
#define UI_MENU_SELECTION_SCREEN_H

#include "lvgl.h"

/**
 * @file ui_MenuSelectionScreen.h
 * @brief Menu selection screen with network status label.
 */

#ifdef __cplusplus
extern "C"
{
#endif

    extern lv_obj_t *ui_MenuSelectionScreen;
    extern lv_obj_t *ui_MenuDropdown;
    extern lv_obj_t *ui_ConfirmMenuButton;
    extern lv_obj_t *ui_MenuDisplayLabel;
    extern lv_obj_t *ui_NetworkStatusLabel;
    extern lv_obj_t *ui_MenuLogo;

    // Initialize the screen and all child elements
    void ui_MenuSelection_screen_init();
    void ui_MenuSelection_screen_start();

    // Destroy the screen and reset pointers
    void ui_MenuSelection_screen_destroy();

    // Event callbacks
    void ui_event_menu_dropdown(lv_event_t *e);
    void ui_event_confirm_menu_button(lv_event_t *e);

    // Update the network status label color/text
    void ui_update_network_status();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // UI_MENU_SELECTION_SCREEN_H
