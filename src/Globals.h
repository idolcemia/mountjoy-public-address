#ifndef GLOBALS_H
#define GLOBALS_H

#include <RumpshiftLogger.h>
#include <Users.h>
#include <NetworkManager.h>
#include <WiFi/WiFiNetworkManager.h>
#include "lvgl.h"
#include "config.h"

// -- Menus --
#include <MenuManager.h>
#include "ui/screens/ui_WiFiConnect.h"
#include "ui/screens/ui_DebugLogScreen.h"
#include "ui/screens/ui_UserSelectionScreen.h"
#include "ui/screens/ui_MenuSelectionScreen.h"

// Network credentials
extern const char *ssid;
extern const char *password;
extern const char *server;
extern const int port;

// Global objects
extern RumpshiftLogger logger;
extern Users *users;
extern Users::User currentUser;
extern NetworkManager *network;
extern WiFiClientWrapper *wifiClient;
extern MenuManager menuManager;

// Struct to hold LVGL labels that are used across screens
struct GlobalLabels
{
    lv_obj_t *networkStatusLabel = nullptr;
    lv_obj_t *menuDisplayLabel = nullptr;
    // add more global labels here as needed
};
extern GlobalLabels gLabels;

void initGlobals();
void initMenus();
void initUsers();
void initWiFiClient();
void _getUsers();
void _setCurrentUser(String name);

#endif
