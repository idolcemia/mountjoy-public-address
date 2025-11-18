#include "Globals.h"

// Network credentials
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASS;
const char *server = LAN_IP;
const int port = 8000;

// Global objects
RumpshiftLogger logger(BAUD_RATE, DEBUG_LEVEL, true);
Users *users = nullptr;
NetworkManager *network =
    new WiFiNetworkManager(
        ssid,
        password,
        WiFiImpl::ARDUINO_WIFI,
        &logger);
WiFiClientWrapper wifiClient;
Menus menus;

// Global labels
GlobalLabels gLabels;
