#pragma once

#include <MenuManager.h>
#include "ui/screens/ui_WiFiConnect.h"
#include "ui/screens/ui_DebugLogScreen.h"
#include "ui/screens/ui_UserSelectionScreen.h"
#include "ui/screens/ui_MenuSelectionScreen.h"

/**
 * @class Menus
 * @brief Wrapper class to initialize all menus using MenuManager.
 *
 * Example usage:
 *   Menus::instance.menus // access the MenuManager
 */
class Menus
{
public:
    MenuManager menus;

    Menus()
    {
        setupMenus();
    }

    // Singleton access
    static Menus &getInstance()
    {
        static Menus instance;
        return instance;
    }

private:
    void setupMenus()
    {
        // Add all your menus here
        menus.addMenu("Menu Selection", ui_MenuSelection_screen_init, ui_MenuSelection_screen_destroy);
        menus.addMenu("WiFi Connect", ui_WiFiConnect_screen_init, ui_WiFiConnect_screen_destroy);
        menus.addMenu("Debug Log", ui_DebugLog_screen_init, ui_DebugLog_screen_destroy);
        menus.addMenu("User Selection", ui_UserSelection_screen_init, ui_UserSelection_screen_destroy);

        // Add more menus as needed:
        // menus.addMenu("User Selection", ui_UserSelectionScreen, ui_UserSelection_screen_init);
        // menus.addMenu("Menu Selection", ui_MenuSelectionScreen, ui_MenuSelection_screen_init);
    }
};
