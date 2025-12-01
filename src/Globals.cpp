#include "Globals.h"

// Network credentials
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASS;
const char *server = LAN_IP;
const int port = 8000;

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

// Global labels
GlobalLabels gLabels;

void initGlobals()
{
    initMenus();
    initWiFiClient();
    initUsers();
}

void initMenus()
{
    menuManager.setLogger(&logger);
    menuManager.addMenu("Menu Selection", ui_MenuSelection_screen_init, ui_MenuSelection_screen_destroy);
    menuManager.addMenu("WiFi Connect", ui_WiFiConnect_screen_init, ui_WiFiConnect_screen_destroy);
    menuManager.addMenu("Debug Log", ui_DebugLog_screen_init, ui_DebugLog_screen_destroy);
    menuManager.addMenu("User Selection", ui_UserSelection_screen_init, ui_UserSelection_screen_destroy);
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