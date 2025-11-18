#pragma once

#include <MenuManager.h>
#include "ui/screens/ui_WiFiConnect.h"
#include "ui/screens/ui_DebugLogScreen.h"
#include "ui/screens/ui_UserSelectionScreen.h"

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
        menus.addMenu("WiFi Connect", ui_WiFiConnectScreen, ui_WiFiConnect_screen_init);
        menus.addMenu("Debug Log", ui_DebugLogScreen, ui_DebugLog_screen_init);
        menus.addMenu("User Selection", ui_UserSelectionScreen, ui_UserSelection_screen_init);

        // Add more menus as needed:
        // menus.addMenu("User Selection", ui_UserSelectionScreen, ui_UserSelection_screen_init);
        // menus.addMenu("Menu Selection", ui_MenuSelectionScreen, ui_MenuSelection_screen_init);
    }
};
