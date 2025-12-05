#include "ui/ui.h"
#include "Globals.h"
#include "ui/events/events.h"
#include "ui/screens/labels/ui_GlobalLabels.h"
#include "ui/screens/ui_GlobalButtons.h"
#include "lvgl.h"

// Exposed UI component pointers
lv_obj_t *uic_UserDisplayLabel;
lv_obj_t *uic_SelectUserLabel;
lv_obj_t *uic_ConfirmUserButton;
lv_obj_t *uic_ConfirmLabel1;
lv_obj_t *uic_NetworkWarningLabel;
lv_obj_t *uic_BSLogo;

lv_obj_t *ui_UserSelectionScreen = nullptr;
lv_obj_t *ui_UserDropdown = nullptr;
lv_obj_t *ui_UserDisplayLabel = nullptr;
lv_obj_t *ui_SelectUserLabel = nullptr;
lv_obj_t *ui_ConfirmUserButton = nullptr;
lv_obj_t *ui_ConfirmLabel1 = nullptr;
lv_obj_t *ui_BSLogo = nullptr;

// =========================
// Event Handlers (thin)
// =========================
void ui_event_user_dropdown(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED)
        handleUserDropdownEvent(e);
}

void ui_event_ConfirmUserButton(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
        handleConfirmUserButton(e);
}

// =========================
// Screen Init
// =========================
void ui_UserSelection_screen_init(void)
{
    logger.info("[UI] Initializing User Selection screen");

    /* -------------------------------------------------------
     *  Screen Base
     * -----------------------------------------------------*/
    ui_UserSelectionScreen = lv_obj_create(nullptr);
    lv_obj_remove_flag(ui_UserSelectionScreen, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_bg_color(ui_UserSelectionScreen, lv_color_hex(0xEE7B01), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_UserSelectionScreen, 255, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(ui_UserSelectionScreen, lv_color_hex(0x2E1A05), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(ui_UserSelectionScreen, LV_GRAD_DIR_VER, LV_PART_MAIN);

    bool connected = network->isConnected();

    /* -------------------------------------------------------
     *  Network Warning Label
     *  (Created always, updated based on connection state)
     * -----------------------------------------------------*/
    uic_NetworkWarningLabel = lv_label_create(ui_UserSelectionScreen);

    if (!connected)
    {
        lv_label_set_text(uic_NetworkWarningLabel, "⚠ Network Not Connected");
        lv_obj_set_style_text_color(uic_NetworkWarningLabel, lv_color_hex(0xFF0000), LV_PART_MAIN);
    }
    else
    {
        lv_label_set_text(uic_NetworkWarningLabel, "");
    }

    lv_obj_set_style_text_font(uic_NetworkWarningLabel, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_align(uic_NetworkWarningLabel, LV_ALIGN_TOP_MID, 0, 10);

    /* -------------------------------------------------------
     *  Logo
     * -----------------------------------------------------*/
    ui_BSLogo = lv_image_create(ui_UserSelectionScreen);
    lv_image_set_src(ui_BSLogo, &ui_img_buildshift_brand_png);
    lv_obj_align(ui_BSLogo, LV_ALIGN_CENTER, 0, -157);

    /* -------------------------------------------------------
     *  Title Label
     * -----------------------------------------------------*/
    ui_SelectUserLabel = lv_label_create(ui_UserSelectionScreen);
    lv_label_set_text(ui_SelectUserLabel, "Select User");

    lv_obj_set_style_text_color(ui_SelectUserLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui_SelectUserLabel, &lv_font_montserrat_48, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_SelectUserLabel, lv_color_hex(0x956207), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_SelectUserLabel, 255, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(ui_SelectUserLabel, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(ui_SelectUserLabel, LV_GRAD_DIR_VER, LV_PART_MAIN);

    lv_obj_align(ui_SelectUserLabel, LV_ALIGN_CENTER, 0, -45);

    /* -------------------------------------------------------
     *  User Dropdown
     * -----------------------------------------------------*/
    ui_UserDropdown = lv_dropdown_create(ui_UserSelectionScreen);

    if (!connected)
    {
        lv_obj_add_state(ui_UserDropdown, LV_STATE_DISABLED);

        lv_obj_set_style_bg_color(ui_UserDropdown, lv_color_hex(0xBBBBBB), LV_PART_MAIN);
        lv_obj_set_style_text_color(ui_UserDropdown, lv_color_hex(0x777777), LV_PART_MAIN);

        lv_dropdown_set_options(ui_UserDropdown, "No network");
    }
    else
    {
        if (users && users->getUserCount() > 0)
        {
            logger.info("Skipping fetch for users.");
        }
        else
        {
            logger.info("Fetching users.");
            _getUsers();
        }
        String list = users ? users->getUsersDelimit("\n") : "No users";
        lv_dropdown_set_options(ui_UserDropdown, list.c_str());

        lv_obj_set_style_bg_color(ui_UserDropdown, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        lv_obj_set_style_text_color(ui_UserDropdown, lv_color_hex(0x000000), LV_PART_MAIN);
    }

    lv_obj_set_width(ui_UserDropdown, 314);
    lv_obj_set_height(ui_UserDropdown, LV_SIZE_CONTENT);
    lv_obj_align(ui_UserDropdown, LV_ALIGN_CENTER, 0, 50);

    /* -------------------------------------------------------
     *  Selected User Label
     * -----------------------------------------------------*/
    ui_UserDisplayLabel = lv_label_create(ui_UserSelectionScreen);
    lv_label_set_text(ui_UserDisplayLabel, "Make a selection");

    lv_obj_set_style_text_color(ui_UserDisplayLabel, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui_UserDisplayLabel, &lv_font_montserrat_40, LV_PART_MAIN);

    lv_obj_align_to(ui_UserDisplayLabel, ui_UserDropdown, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    /* -------------------------------------------------------
     *  Confirm Button
     * -----------------------------------------------------*/
    ui_ConfirmUserButton = lv_button_create(ui_UserSelectionScreen);
    lv_obj_set_size(ui_ConfirmUserButton, 266, 50);
    lv_obj_align(ui_ConfirmUserButton, LV_ALIGN_CENTER, 0, 150);

    lv_obj_set_style_bg_color(ui_ConfirmUserButton, lv_color_hex(0xF0A31E), LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_ConfirmUserButton, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    ui_ConfirmLabel1 = lv_label_create(ui_ConfirmUserButton);
    lv_label_set_text(ui_ConfirmLabel1, "Confirm");
    lv_obj_set_style_text_font(ui_ConfirmLabel1, &lv_font_montserrat_40, LV_PART_MAIN);
    lv_obj_align(ui_ConfirmLabel1, LV_ALIGN_CENTER, 0, 0);

    /* -------------------------------------------------------
     *  Events
     * -----------------------------------------------------*/
    lv_obj_add_event_cb(ui_UserDropdown, ui_event_user_dropdown, LV_EVENT_ALL, nullptr);
    lv_obj_add_event_cb(ui_ConfirmUserButton, ui_event_ConfirmUserButton, LV_EVENT_ALL, nullptr);

    /* -------------------------------------------------------
     *  Save Global Pointers
     * -----------------------------------------------------*/
    uic_UserDisplayLabel = ui_UserDisplayLabel;
    uic_SelectUserLabel = ui_SelectUserLabel;
    uic_ConfirmUserButton = ui_ConfirmUserButton;
    uic_ConfirmLabel1 = ui_ConfirmLabel1;
    uic_BSLogo = ui_BSLogo;

    /* -------------------------------------------------------
     *  Network status label
     * -----------------------------------------------------*/
    // ui_GlobalLabels::initNetworkStatus(ui_UserSelectionScreen);
    // ui_GlobalLabels::initUserSelectionLabel(ui_UserSelectionScreen);

    /* -------------------------------------------------------
     *  Show Screen
     * -----------------------------------------------------*/
    lv_scr_load(ui_UserSelectionScreen);
    ui_GlobalButtons::updateGlobalButtons(ui_UserSelectionScreen);
    ui_GlobalLabels::updateNetworkStatus(ui_UserSelectionScreen);
    ui_GlobalLabels::updateUserSelectionLabel(ui_UserSelectionScreen);

    logger.info("[UI] User Selection screen loaded successfully");

    menuManager.setCachedScreen(PasteurizerMenu::MENU_USER_SELECTION, ui_UserSelectionScreen);
}

void ui_UserSelectionScreenUpdate()
{
    if (!ui_UserSelectionScreen)
        return;

    bool connected = network->isConnected();

    /* -------------------------------------------------------
     *  Update Network Warning Label
     * -----------------------------------------------------*/
    if (uic_NetworkWarningLabel)
    {
        if (!connected)
        {
            lv_label_set_text(uic_NetworkWarningLabel, "⚠ Network Not Connected");
            lv_obj_set_style_text_color(uic_NetworkWarningLabel, lv_color_hex(0xFF0000), LV_PART_MAIN);
        }
        else
        {
            lv_label_set_text(uic_NetworkWarningLabel, "");
        }
    }

    /* -------------------------------------------------------
     *  Update Dropdown
     * -----------------------------------------------------*/
    if (ui_UserDropdown)
    {
        if (!connected)
        {
            lv_obj_add_state(ui_UserDropdown, LV_STATE_DISABLED);
            lv_dropdown_set_options(ui_UserDropdown, "No network");

            lv_obj_set_style_bg_color(ui_UserDropdown, lv_color_hex(0xBBBBBB), LV_PART_MAIN);
            lv_obj_set_style_text_color(ui_UserDropdown, lv_color_hex(0x777777), LV_PART_MAIN);
        }
        else
        {
            lv_obj_clear_state(ui_UserDropdown, LV_STATE_DISABLED);

            // Refresh users (if necessary)
            if (!users || users->getUserCount() == 0)
            {
                logger.info("[UI] Updating user list (network connected)");
                _getUsers();
            }

            String list = users ? users->getUsersDelimit("\n") : "No users";
            lv_dropdown_set_options(ui_UserDropdown, list.c_str());

            lv_obj_set_style_bg_color(ui_UserDropdown, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
            lv_obj_set_style_text_color(ui_UserDropdown, lv_color_hex(0x000000), LV_PART_MAIN);
        }

        ui_GlobalButtons::updateGlobalButtons(ui_UserSelectionScreen);
        menuManager.setCachedScreen(PasteurizerMenu::MENU_USER_SELECTION, ui_UserSelectionScreen);
    }

    /* -------------------------------------------------------
     *  Update Selected User Display Label
     * -----------------------------------------------------*/
    if (ui_UserDisplayLabel)
    {
        int selected = lv_dropdown_get_selected(ui_UserDropdown);

        if (!connected)
        {
            lv_label_set_text(ui_UserDisplayLabel, "No network");
        }
        else if (!users || users->getUserCount() == 0)
        {
            lv_label_set_text(ui_UserDisplayLabel, "No users");
        }
        else
        {
            String username = users->getUsers()[selected].name;
            lv_label_set_text(ui_UserDisplayLabel, username.c_str());
        }
    }

    /* -------------------------------------------------------
     *  Enable/Disable Confirm Button
     * -----------------------------------------------------*/
    if (ui_ConfirmUserButton)
    {
        if (!connected || !users || users->getUserCount() == 0)
            lv_obj_add_state(ui_ConfirmUserButton, LV_STATE_DISABLED);
        else
            lv_obj_clear_state(ui_ConfirmUserButton, LV_STATE_DISABLED);
    }

    /* -------------------------------------------------------
     *  Update global labels (network + user)
     * -----------------------------------------------------*/
    ui_GlobalLabels::updateNetworkStatus(ui_UserSelectionScreen);
    ui_GlobalLabels::updateUserSelectionLabel(ui_UserSelectionScreen);

    logger.info("[UI] User Selection screen updated");
}

// =========================
// Destroy
// =========================
void ui_UserSelection_screen_destroy(void)
{
    if (ui_UserSelectionScreen)
        lv_obj_clean(ui_UserSelectionScreen);

    ui_UserSelectionScreen = nullptr;
    ui_UserDropdown = nullptr;
    ui_UserDisplayLabel = nullptr;
    ui_SelectUserLabel = nullptr;
    ui_ConfirmUserButton = nullptr;
    ui_ConfirmLabel1 = nullptr;
    ui_BSLogo = nullptr;

    uic_NetworkWarningLabel = nullptr;
    uic_UserDisplayLabel = nullptr;
    uic_SelectUserLabel = nullptr;
    uic_ConfirmUserButton = nullptr;
    uic_ConfirmLabel1 = nullptr;
    uic_BSLogo = nullptr;
}
