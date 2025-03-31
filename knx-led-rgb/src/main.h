#pragma once

#include <Arduino.h>
#include <knx.h>
#include "esp-knx-webserver.h"
#include "esp-knx-led.h"

#if defined(ESP32)
    #pragma message "Building main for ESP32"
    #include <WiFi.h>
    #include <esp_wifi.h>
    #include <ESPmDNS.h>
#elif defined(ESP8266)
    #pragma message "Building main for ESP8266"
    #include <ESP8266WiFi.h>
    #include <ESP8266mDNS.h>
#else
    #error "Wrong hardware. Not ESP8266 or ESP32"
#endif

bool knxConfigOk = false;
bool knxDisabled = false;
bool initSent = false;
bool itsDay = true;
int paraBrightnessDay = 0;
int paraBrightnessNight = 0;
int defaultBrightness = 0;

KnxLed rgbLight = KnxLed();
KnxWebserver knxWebServ = KnxWebserver();

void setup();
void loop();