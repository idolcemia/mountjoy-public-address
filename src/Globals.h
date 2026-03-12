#ifndef GLOBALS_H
#define GLOBALS_H

#include <RumpshiftLogger.h>
#include <Users.h>
#include <NetworkManager.h>
#include <WiFi/WiFiNetworkManager.h>
#include "lvgl.h"
#include "config.h"
#include "PasteurizerRelays.h"
#include "TemperatureSensor.h"
#include "GigaAudio.h"
#include <OneWire.h>
#include <DallasTemperature.h>


#include "FillControl.h"
#include "PressureControl.h"

// -- Menus --
#include <MenuManager.h>
#include "ui/screens/ui_WiFiConnect.h"
#include "ui/screens/ui_DebugLogScreen.h"
#include "ui/screens/ui_UserSelectionScreen.h"
#include "ui/screens/ui_MenuSelectionScreen.h"
#include "ui/screens/ui_TempControl.h"

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
extern PasteurizerRelays pasteurizerRelays;
extern TemperatureSensor chamberTemperatureSensor;
extern TemperatureSensor kettleTemperatureSensor;
extern GigaAudio audio;
extern FillControl fillControl;
extern PressureControl pressureControl;

namespace PasteurizerMenu
{
    constexpr const char *MENU_MENU_SELECTION = "Menu Selection";
    constexpr const char *MENU_WIFI_CONNECT = "WiFi Connect";
    constexpr const char *MENU_DEBUG_LOG = "Debug Log";
    constexpr const char *MENU_USER_SELECTION = "User Selection";
    constexpr const char *MENU_TEMP_CONTROL = "Temp Control";
}

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
