/*
 * Starter project using the following components:
 * Arduino Giga R1 M7
 * Giga Display Shield (800 x 480)
 * LvGL 8.x
 * 20251020 tested good.
 * Enhanced with WiFi debug, retries, RSSI, MAC, and encryption info
 */

#include <Arduino.h>
#include <ArduinoJson.h>

#include "Arduino_H7_Video.h"
#include "Arduino_GigaDisplayTouch.h"
#include "lvgl.h"
#include "ui/ui.h"

// --- Internal libs ---
// #include <NetworkManager.h>
// #include <WiFi/WiFiNetworkManager.h>
#include <RumpshiftLogger.h>
#include <WiFi/WiFiClientWrapper.h>
#include <HttpResponse.h>
#include <Users.h>
#include <Diagnostic.h>

#include "config.h"
#include "Globals.h"

Arduino_H7_Video Display(800, 480, GigaDisplayShield);
Arduino_GigaDisplayTouch TouchDetector;

void _getUsers();
void _log();

void setup()
{
    logger.begin();
    logger.info("Starting log....");

    network->setRemote(server, port);
    // network->begin();
    // network->printStatus();

    // wifiClient.begin(&logger);
    // users = new Users(wifiClient, &logger);
    // _getUsers();

    Display.begin();
    TouchDetector.begin();
    ui_init();
}

void loop()
{
    lv_timer_handler();

    // Only update once LVGL is stable
    if (millis() > 3000) // wait 3s after boot
    {
        Diagnostic::updateDefault();
    }

    delay(16);
}

void _log()
{
    StaticJsonDocument<512> doc;

    // Database ID
    // doc["database_id"] = DB_ID;

    // Title property
    doc["Entry"] = "Log Entry 1";

    // Number property
    doc["Temperature (C)"] = 23.5;

    // Timestamp property (Notion date)
    // char timestampStr[25];
    // unsigned long now = millis() / 1000;
    // snprintf(timestampStr, sizeof(timestampStr), "2025-10-30T21:%02lu:00Z", (now / 60) % 60);

    // doc["Timestamp"] = timestampStr;

    // Serialize JSON
    String output;
    serializeJson(doc, output);

    // POST
    const char *host = "3.136.200.179";
    const int port = 8000;
    const char *url = API_PATH;
    logger.info("[HTTP_LOG] " + output);

    HttpResponse resp = wifiClient.http().post(host, port, url, output);
    logger.info("[POST RESPONSE - FULL] " + resp.full());
}

void _getUsers()
{
    const char *host = "3.136.200.179";
    const int port = 8888;
    const char *url = "/notion-api/integrations/notion/consoleIntegration/users?include=name,id&excludeNames=Mountjoy+Sparkling,Ritual+Coffee+Roasters,n2y+integration,Console,Product+Management";

    // HttpResponse resp = wifiClient.http().get(host, port, url);
    // logger.info("[RESPONSE - BODY] " + resp.body());

    HttpResponse resp = users->fetch(host, port, url);
    logger.info("[RESPONSE - BODY] " + resp.body());
    users->parse(resp.body());
    users->printUsers();
}