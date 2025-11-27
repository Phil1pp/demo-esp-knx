#pragma once

#include <Arduino.h>
#include <knx.h>
#include <knxprod.h>
#include "settings.h"
#include "credentials.h"
#include <esp-knx-common.h>
#include <esp-knx-webserver.h>
#include <esp-knx-led.h>

#if defined(ESP32)
    #include <WiFi.h>
    #include <mDNS.h>
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
    #include <ESP8266mDNS.h>
#elif defined(LIBRETINY)
    #include <WiFi.h>
    #include <mDNS.h>
#else
    #error "Wrong hardware. Not ESP32, ESP8266 or LIBRETINY"
#endif

bool knxConfigOk = false;
bool initSent = false;
KnxLed Light = KnxLed();
KnxWebserver knxWebServ = KnxWebserver();
void setup();
void loop();

void knxCallback(GroupObject &go);
void statusCallback(bool state);
void responseBrightnessCallback(uint8_t value);
void responseTemperatureCallback(uint16_t value);