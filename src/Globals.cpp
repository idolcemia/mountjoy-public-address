#include "Globals.h"

#include "FillControl.h" // replaced FlowMeter

// Network credentials
const char *ssid = "mtjyspkl";
const char *password = "bebubbly";
const char *server = "3.136.200.179"; // Replace with actual server IP
const int port = 8000;

// Dallas temperature sensor pin assignment.
#define ONE_WIRE_BUS 12
#define SENSOR_NOT_FOUND -127

// Global objects
RumpshiftLogger logger(BAUD_RATE, DEBUG_LEVEL, true);
Users *users = nullptr;
Users::User currentUser;
NetworkManager *network =
    new WiFiNetworkManager(
        ssid,
        password,
        WiFiImpl::ARDUINO_WIFI,
        &logger);
WiFiClientWrapper *wifiClient = nullptr;
MenuManager menuManager;
PasteurizerRelays pasteurizerRelays(4, 5, 6, 7); // on the arduino shield, 4, 7, 8, 12
TemperatureSensor chamberTemperatureSensor(A0);


FillControl fillControl(A5, 5, 4.5); // Sensor pin, valve pin, fill level target in liters (according to sensor)
// A fill level of 8.5 liters yields 44 liters total yield.

PressureControl pressureControl(2, 3, 4, 6 );


// Global labels
GlobalLabels gLabels;

void initGlobals()
{
    initMenus();
    initWiFiClient();
    initUsers();

    pasteurizerRelays.begin();
    chamberTemperatureSensor.begin();

    fillControl.begin();
    pressureControl.begin();
}

void initMenus()
{
    // --- Initialization ---
    logger.info("[Globals] Running init menus.");
    menuManager.setLogger(&logger);

    // Menu Selection
    menuManager.addMenu(PasteurizerMenu::MENU_MENU_SELECTION, ui_MenuSelection_screen_init, ui_MenuSelection_screen_destroy);
    menuManager.setUpdater(PasteurizerMenu::MENU_MENU_SELECTION, ui_MenuSelectionScreenUpdate);

    // WiFi
    menuManager.addMenu(PasteurizerMenu::MENU_WIFI_CONNECT, ui_WiFiConnect_screen_init, ui_WiFiConnect_screen_destroy);
    menuManager.setUpdater(PasteurizerMenu::MENU_WIFI_CONNECT, ui_WiFiConnectScreenUpdate);

    // Debug Log
    menuManager.addMenu(PasteurizerMenu::MENU_DEBUG_LOG, ui_DebugLog_screen_init, ui_DebugLog_screen_destroy);
    menuManager.setUpdater(PasteurizerMenu::MENU_DEBUG_LOG, ui_DebugLogScreenUpdate);

    // User Selection
    menuManager.addMenu(PasteurizerMenu::MENU_USER_SELECTION, ui_UserSelection_screen_init, ui_UserSelection_screen_destroy);
    menuManager.setUpdater(PasteurizerMenu::MENU_USER_SELECTION, ui_UserSelectionScreenUpdate);

    // Temp Control
    menuManager.addMenu(PasteurizerMenu::MENU_TEMP_CONTROL, ui_TempControl_screen_init, ui_TempControl_screen_destroy);
    menuManager.setUpdater(PasteurizerMenu::MENU_TEMP_CONTROL, ui_TempControlScreenUpdate);
}

void initWiFiClient()
{
    wifiClient = new WiFiClientWrapper();
    wifiClient->begin(&logger);
}

void initUsers()
{
    users = new Users(*wifiClient, &logger);
}

void _getUsers()
{
    const char *host = "3.136.200.179";
    const int port = 8888;
    const char *url =
        "/notion-api/integrations/notion/consoleIntegration/users?include=name,id&excludeNames=Mountjoy+Sparkling,Ritual+Coffee+Roasters,n2y+integration,Console,Product+Management";

    HttpResponse resp = users->fetch(host, port, url);
    logger.info("[RESPONSE - BODY] " + resp.body());
    users->parse(resp.body());
    users->printUsers();
}

void _setCurrentUser(String name)
{
    if (users->getUserByName(name, currentUser))
    {
        logger.info("Current user set to: '" + currentUser.getName() + "'.");
    }
    else
    {
        logger.error("Unable to set current user set to: '" + name + "'.");
    }
}
