#include "ui_MenuSelectionScreen.h"
#include "Globals.h" // for logger and network
#include "ui/ui.h"
#include "ui/events/events.h"
#include "ui/screens/ui_GlobalButtons.h"
#include "ui/screens/labels/ui_GlobalLabels.h"

lv_obj_t *ui_MenuSelectionScreen = nullptr;
lv_obj_t *ui_MenuDropdown = nullptr;
lv_obj_t *ui_ConfirmMenuButton = nullptr;
lv_obj_t *ui_MenuDisplayLabel = nullptr;
// lv_obj_t *ui_NetworkStatusLabel = nullptr;
lv_obj_t *ui_MenuLogo = nullptr;

// Event: dropdown changed
void ui_event_menu_dropdown(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_VALUE_CHANGED)
    {
        handleMenuSelectionDropdownEvent(e);
        // lv_obj_t *dropdown = (lv_obj_t *)lv_event_get_target(e);
        // const char *selected = lv_dropdown_get_selected_str(dropdown);
        // lv_label_set_text(ui_MenuDisplayLabel, selected);
    }
}

// Event: confirm button clicked
void ui_event_confirm_menu_button(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        handleMenuSelectionButtonEvent(e);
        // const char *selected = lv_dropdown_get_selected_str(ui_MenuDropdown);

        // TODO: implement menu navigation based on selection
        // Example: _ui_screen_change(&ui_<SelectedMenuScreen>, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_<SelectedMenuScreen>_screen_init);
    }
}

void ui_MenuSelection_screen_start()
{
    menuManager.queueMenu(PasteurizerMenu::MENU_MENU_SELECTION);
    lv_timer_handler();
    // menuManager.setCachedScreen(PasteurizerMenu::MENU_MENU_SELECTION, ui_MenuSelectionScreen);
}

// Initialize the screen
void ui_MenuSelection_screen_init()
{
    // Create main screen
    ui_MenuSelectionScreen = lv_obj_create(nullptr);
    lv_obj_remove_flag(ui_MenuSelectionScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_MenuSelectionScreen, lv_color_hex(0xEE7B01), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_MenuSelectionScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_MenuSelectionScreen, lv_color_hex(0x2E1A05), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_MenuSelectionScreen, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // --- Dropdown ---
    ui_MenuDropdown = lv_dropdown_create(ui_MenuSelectionScreen);
    lv_dropdown_set_options(
        ui_MenuDropdown,
        menuManager.getMenusDelimit("\n", PasteurizerMenu::MENU_MENU_SELECTION).c_str());
    lv_obj_set_size(ui_MenuDropdown, 314, 50);
    lv_obj_align(ui_MenuDropdown, LV_ALIGN_CENTER, 0, 50);
    lv_obj_add_flag(ui_MenuDropdown, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_add_event_cb(ui_MenuDropdown, ui_event_menu_dropdown, LV_EVENT_ALL, nullptr);

    // --- Display label above dropdown ---
    ui_MenuDisplayLabel = lv_label_create(ui_MenuSelectionScreen);
    lv_label_set_text(ui_MenuDisplayLabel, "Make a selection");
    lv_obj_set_style_text_color(ui_MenuDisplayLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_MenuDisplayLabel, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align_to(ui_MenuDisplayLabel, ui_MenuDropdown, LV_ALIGN_OUT_TOP_MID, 0, -10);

    // --- Confirm button below dropdown ---
    ui_ConfirmMenuButton = lv_button_create(ui_MenuSelectionScreen);
    lv_obj_set_size(ui_ConfirmMenuButton, 266, 50);
    lv_obj_align_to(ui_ConfirmMenuButton, ui_MenuDropdown, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_add_flag(ui_ConfirmMenuButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_remove_flag(ui_ConfirmMenuButton, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_ConfirmMenuButton, lv_color_hex(0xF0A31E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ConfirmMenuButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_ConfirmMenuButton, ui_event_confirm_menu_button, LV_EVENT_ALL, nullptr);

    // Label inside confirm button
    lv_obj_t *lblConfirm = lv_label_create(ui_ConfirmMenuButton);
    lv_label_set_text(lblConfirm, "Confirm");
    lv_obj_center(lblConfirm);

    // --- Optional Logo (top-left) ---
    ui_MenuLogo = lv_image_create(ui_MenuSelectionScreen);
    lv_image_set_src(ui_MenuLogo, &ui_img_buildshift_brand_png);
    lv_obj_set_size(ui_MenuLogo, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_align(ui_MenuLogo, LV_ALIGN_TOP_MID, 0, 10);

    ui_GlobalButtons::initGlobalButtons(ui_MenuSelectionScreen);
    ui_GlobalLabels::initUserSelectionLabel(ui_MenuSelectionScreen);
    ui_GlobalLabels::initNetworkStatus(ui_MenuSelectionScreen);

    lv_scr_load(ui_MenuSelectionScreen);
    menuManager.setCachedScreen(PasteurizerMenu::MENU_MENU_SELECTION, ui_MenuSelectionScreen);
}

void ui_MenuSelection_screen_destroy()
{
    if (ui_MenuSelectionScreen)
        lv_obj_del(ui_MenuSelectionScreen);

    ui_MenuSelectionScreen = nullptr;
    ui_MenuDropdown = nullptr;
    ui_ConfirmMenuButton = nullptr;
    ui_MenuDisplayLabel = nullptr;
    // ui_NetworkStatusLabel = nullptr;
    ui_MenuLogo = nullptr;
}

void ui_MenuSelectionScreenUpdate()
{
    // --- 0. Ensure screen exists ---
    if (!ui_MenuSelectionScreen)
    {
        logger.info("[ui_MenuSelectionScreen] screen not initialized, skipping update.");
        return;
    }

    // --- 1. Refresh dropdown options (if menu list has changed) ---
    if (ui_MenuDropdown)
    {
        lv_dropdown_set_options(
            ui_MenuDropdown,
            menuManager.getMenusDelimit("\n", PasteurizerMenu::MENU_MENU_SELECTION).c_str());
    }

    // --- 2. Update display label to match current dropdown value ---
    if (ui_MenuDisplayLabel && ui_MenuDropdown)
    {
        char selected[64];
        lv_dropdown_get_selected_str(ui_MenuDropdown, selected, sizeof(selected));
        lv_label_set_text(
            ui_MenuDisplayLabel,
            selected && selected[0]
                ? selected
                : "Make a selection");
    }

    // --- 3. Update network status label ---
    ui_GlobalLabels::updateNetworkStatus(ui_MenuSelectionScreen);
    ui_GlobalLabels::updateUserSelectionLabel(ui_MenuSelectionScreen);
    ui_GlobalButtons::updateGlobalButtons(ui_MenuSelectionScreen);

    // --- 4. Reattach global buttons (if needed after Full Screen reload) ---
    // ui_GlobalButtons::syncToScreen(ui_MenuSelectionScreen);

    // --- 5. Reattach global labels ---
    // ui_GlobalLabels::syncUserSelectionLabel(ui_MenuSelectionScreen);

    // --- 6. Restore menuManager cache (prevents flicker on back navigation) ---
    menuManager.setCachedScreen(PasteurizerMenu::MENU_MENU_SELECTION, ui_MenuSelectionScreen);
}
