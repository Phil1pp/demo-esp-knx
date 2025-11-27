#include "main.h"

// callback from knx value change
void knxCallback(GroupObject &go)
{
    // callbacks are now handled in the class, not per instance,
    // this means, we have to check, which GroupObject is calling back
    switch (go.asap())
    {
    case APP_KoGeneralDayNight:
    {
        bool itsDay = go.value(DPT_Switch);
        uint8_t defaultBrightness = itsDay ? ParamAPP_BrightnessDay : ParamAPP_BrightnessNight;
        Light.configDefaultBrightness(defaultBrightness);
        break;
    }
    case APP_KoLightOnOff:
    {
        Light.switchLight(go.value(DPT_Switch));
        break;
    }
    case APP_KoLightBrightness:
    {
        Light.setBrightness(go.value(DPT_Percent_U8));
        break;
    }
    case APP_KoLightDim:
    {
        dpt3_t dimCmd;
        dimCmd.fromDPT3(go.value(DPT_Control_Dimming));
        Light.setRelDimCmd(dimCmd);
        break;
    }
    case APP_KoLightColorTemp:
    {
        Light.setTemperature(go.value(DPT_Value_2_Ucount));
        break;
    }
    case APP_KoLightColorTempRelative:
    {
        dpt3_t dimCmd;
        dimCmd.fromDPT3(go.value(DPT_Control_Dimming));
        Light.setRelTemperatureCmd(dimCmd);
        break;
    }
    }
}

void statusCallback(bool state)
{
    KoAPP_LightStatus.value(state, DPT_Switch);
}

void responseBrightnessCallback(uint8_t value)
{
    KoAPP_LightStatusBrightness.value(value, DPT_Percent_U8);
}

void responseTemperatureCallback(uint16_t value)
{
    KoAPP_LightStatusColorTemp.value(value, DPT_Value_2_Ucount);
}

void setup()
{
#ifdef ESP8266
    Serial.begin(74880);
#else
    Serial.begin(115200);
#endif

    // set correct hardware type for flash compatibility check
    setFirmwareVersion(MAIN_OpenKnxId, MAIN_ApplicationNumber, MAIN_ApplicationVersion);

    // Red status led on at start
    pinMode(STAT_LED_GN, OUTPUT);
    pinMode(STAT_LED_RD, OUTPUT);
    digitalWrite(STAT_LED_GN, STAT_LED_OFF);
    digitalWrite(STAT_LED_RD, STAT_LED_ON);

    // read adress table, association table, groupobject table and parameters from eeprom
    knx.readMemory();

    knxConfigOk = knx.configured() && checkKnxApp() && knx.bau().groupObjectTable().entryCount() == MAIN_MaxKoNumber; //MAIN_ParameterSize

    // Setup device after device was configured by ETS
    if (knxConfigOk)
    {
        GroupObject::classCallback(knxCallback); // callbacks are now handled per class, not per instance

        // read hostname from ETS config
        HOSTNAME = (char *)knx.paramData(APP_Hostname);

        // read parameters from ETS config
        Light.configDimSpeed(ParamAPP_DimSpeed);
        Light.configDefaultBrightness(ParamAPP_BrightnessDay);
        Light.configDefaultTemperature(ParamAPP_DefaultColorTemp);

        // register callbacks from LED controllers
        Light.registerStatusCallback(statusCallback);
        Light.registerBrightnessCallback(responseBrightnessCallback);
        Light.registerTemperatureCallback(responseTemperatureCallback);
    }
    Light.initTunableWhiteLight(LED_PIN_CW, LED_PIN_WW, CCT_MODE);

    Serial.println(HOSTNAME);
    Serial.println(getKnxPhysAddr());

    // Red status led off and green led on after setup
    digitalWrite(STAT_LED_GN, STAT_LED_ON);
    digitalWrite(STAT_LED_RD, STAT_LED_OFF);

    #if defined(ESP8266) || defined(ESP32)
    WiFi.persistent(false); // Solve possible wifi init errors
    WiFi.disconnect(true);  // Delete SDK wifi config
    #endif

    WiFi.mode(WIFI_STA);
    WiFi.hostname(HOSTNAME);
    //WiFi.setAutoConnect(true);
    //WiFi.setAutoReconnect(true);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    WiFi.hostname(HOSTNAME); // Set hostname twice
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        // Implement AP mode here after timeout if needed
        // however an open AP can be a security issue
        // settingsAP();
    }

    // Setup and start KNX stack
    setKnxHostname(HOSTNAME);
    knx.start();

    // Setup and start webserver and OTA
    knxWebServ.setHostname(HOSTNAME);
    knxWebServ.setBuildDetails("Compiled: " __DATE__ ", " __TIME__ );
    knxWebServ.setKnxDetail(getKnxPhysAddr(), getKnxAppDetails(), knxConfigOk);
    knxWebServ.registerGetKnxModeCallback(getKnxMode);
    knxWebServ.registerSetKnxModeCallback(setKnxMode);
    knxWebServ.registerRestartDeviceCallback(restartDeviceCallback);
    knxWebServ.startOta();
    knxWebServ.startWeb(WWW_USER, WWW_PASS);

    // Both status LEDs off after WIFI connect
    digitalWrite(STAT_LED_GN, STAT_LED_OFF);
    digitalWrite(STAT_LED_RD, STAT_LED_OFF);
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        knxWebServ.loop();
        if(getKnxActive())
        {
            knx.loop();
        }

        // only run the application code if the device was configured with ETS
        if (!knxConfigOk)
            return;

        if (!initSent)
        {
            Light.sendStatusUpdate();
            initSent = true;
        }

        Light.loop();
    }
    else
    {
        static unsigned int reconnectCounter = 0;
        if(reconnectCounter > 1000)
        {
            WiFi.reconnect();
            reconnectCounter=0;
        }
        reconnectCounter++;
    }
    keepCycleTime(MIN_CYCLE_TIME);
}