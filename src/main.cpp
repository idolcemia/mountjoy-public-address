/*
 * Mountjoy Kettle fill / Temp gauge using the following components:
 * Arduino Giga R1 M7
 * Giga Display Shield (800 x 480)
 * LvGL 9.x
 * 20251125 tested good.
 * Enhanced with WiFi debug, retries, RSSI, MAC, and encryption info
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

#include "config.h"
#include "Globals.h"
#include "ui/screens/labels/ui_GlobalLabels.h"

#include "FillControl.h"
#include "PressureControl.h"

                                 

Arduino_H7_Video Display(800, 480, GigaDisplayShield);
Arduino_GigaDisplayTouch TouchDetector;

WiFiClientWrapper testClient;

unsigned long lastSensorUpdate = 0;
const unsigned long SENSOR_UPDATE_INTERVAL = 1000;




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

    initGlobals();
    // testClient.begin(&logger);
    Display.begin();
    TouchDetector.begin();
    ui_init();

    
}

void loop()
{
    lv_timer_handler();
    // ui_GlobalLabels::updateNetworkStatus();
    // ui_GlobalLabels::networkChecked();

    // Only update once LVGL is stable
    // Maybe move this to particular screens
    // if (millis() > 3000) // wait 3s after boot
    // {
    //     Diagnostic::updateDefault();
    // }

    // Update presdsure and flow periodically
    unsigned long currentMillis = millis();
    if (currentMillis - lastSensorUpdate >= SENSOR_UPDATE_INTERVAL)
    {
        lastSensorUpdate = currentMillis;


        logger.info("Fill Volume: " + String(fillControl.getFillVolume()) + " L ");

    switch (fillControl._state)
        {
        default:
        case FillControlState::FILL_RESET:
            fillControl.reset();
            break;
        case FillControlState::FILL_RUN:
            fillControl.getFillVolume(); // Get current volume, according to sensor.
            if (fillControl.getFillVolume() >= fillControl._fillAmount) {
                fillControl.reset();

            }   
            break;
        case FillControlState::FILL_PAUSE:
            fillControl.stop();
            break;
        case FillControlState::FILL_DONE:
            fillControl.reset();
            break;
        }

        switch (pressureControl._state)
        {
        default:
        case PressureControlState::PC_RESET:
            pressureControl.reset();
            break;
        case PressureControlState::PC_RUN:

            if (pressureControl.overPressure()) {
                pressureControl.reset();
            }   
            break;
        case PressureControlState::PC_PAUSE:
            pressureControl.stop();
            break;
        case PressureControlState::PC_DONE:
            pressureControl.reset();
            break;
        }

        pressureControl.updateUI();
        fillControl.updateUI();
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
