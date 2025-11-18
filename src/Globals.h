#ifndef GLOBALS_H
#define GLOBALS_H

#include <RumpshiftLogger.h>
#include <Users.h>
#include <NetworkManager.h>
#include <WiFi/WiFiNetworkManager.h>
#include "ui/menus.h"
#include "lvgl.h"
#include "config.h"

// Network credentials
extern const char *ssid;
extern const char *password;
extern const char *server;
extern const int port;

// Global objects
extern RumpshiftLogger logger;
extern Users *users;
extern NetworkManager *network;
extern WiFiClientWrapper wifiClient;
extern Menus menus;

// Struct to hold LVGL labels that are used across screens
struct GlobalLabels
{
    lv_obj_t *networkStatusLabel = nullptr;
    lv_obj_t *menuDisplayLabel = nullptr;
    // add more global labels here as needed
};
extern GlobalLabels gLabels;

#endif
