/*
 * Mountjoy Public Address / Broadcast System
 */

#include <Arduino.h>
#include <ArduinoJson.h>

#include <lv_conf.h>
#include <lvgl.h>

#include "Arduino_H7_Video.h"
#include "Arduino_GigaDisplayTouch.h"
#include "ui/ui.h"

// --- Internal libs ---
// #include <NetworkManager.h>
// #include <WiFi/WiFiNetworkManager.h>
#include <RumpshiftLogger.h>
#include <WiFi/WiFiClientWrapper.h>
#include <HttpResponse.h>
#include <Users.h>
#include <Diagnostic.h>

#include "../include/config.h"
#include "Globals.h"
#include "ui/screens/labels/ui_GlobalLabels.h"

#include "AudioMaster.h"

enum Station // Device addressing for different stations I2C communication.
// Values should be set according to actual hardware design.
{
    STATION_BREWING = 0,
    STATION_EXTRACTING = 9,
    STATION_PASTEURIZING = 2,
    STATION_CLEANING = 3
};

Arduino_H7_Video Display(800, 480, GigaDisplayShield);
Arduino_GigaDisplayTouch TouchDetector;

WiFiClientWrapper testClient;

// Loop control variables
unsigned long lastAudioUpdate = 0;
const unsigned long AUDIO_UPDATE_INTERVAL = 100;
bool gAudioMasterReady = false;

static void logStartupStage(const char *stage)
{
    logger.info(String("[STARTUP] ") + stage);
    Serial.println(String("[STARTUP] ") + stage);
}

void _log();

void setup()
{
    Serial.begin(115200);
    delay(50);
    Serial.println("[STARTUP] setup entered");

    logger.begin();
    logger.info("Starting log....");
    logStartupStage("logger.begin complete");

    network->setRemote(server, port);
    logStartupStage("network->setRemote complete");
    // network->begin();
    // network->printStatus();

    // wifiClient.begin(&logger);
    // users = new Users(wifiClient, &logger);

    initGlobals();
    logStartupStage("initGlobals complete");
    // testClient.begin(&logger);

    logger.info(String("Beginning display initialization..."));
    Display.begin();
    logStartupStage("Display.begin complete");
    TouchDetector.begin();
    logStartupStage("TouchDetector.begin complete");
    ui_init();
    logStartupStage("ui_init complete");

    logStartupStage("audioMaster.begin start");
    if (!audioMaster.begin())
    {
        logger.error(String("AudioMaster initialization failed: ") + audioMaster.getLastError());
        Serial.println(String("[STARTUP] AudioMaster error: ") + audioMaster.getLastError());
        Serial.println("[STARTUP] audioMaster.begin failed");
        gAudioMasterReady = false;
    }
    else
    {
        gAudioMasterReady = true;
        logStartupStage("audioMaster.begin complete");
    }
}

void loop()
{
    lv_timer_handler();

    unsigned long currentMillis = millis();

    if (!gAudioMasterReady)
    {
        static bool loggedNotReady = false;
        if (!loggedNotReady)
        {
            logger.error("[AUDIO] update skipped: audio master not initialized.");
            Serial.println("[AUDIO] update skipped: audio master not initialized.");
            loggedNotReady = true;
        }
    }
    else
    {
        if (currentMillis - lastAudioUpdate >= AUDIO_UPDATE_INTERVAL)
        {
            lastAudioUpdate = currentMillis;

            // Process incoming and outgoing communication
            audioMaster.update();
        }
    }

    delay(5); // Small delay for stability
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

    // HttpResponse resp = wifiClient.http().post(host, port, url, output);
    // logger.info("[POST RESPONSE - FULL] " + resp.full());
}
