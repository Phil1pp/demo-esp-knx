#include "main.h"
#include "settings.h"
#include "credentials.h"

// callback from knx value change
void knxCallback(GroupObject &go)
{
    // callbacks are now handled in the class, not per instance,
    // this means, we have to check, which GroupObject is calling back
    switch (go.asap())
    {
    case GO_ZENTRAL_TAG_NACHT:
    {
        itsDay = go.value(DPT_Switch);
        defaultBrightness = itsDay ? paraBrightnessDay : paraBrightnessNight;
        rgbLight.configDefaultBrightness(defaultBrightness);
        break;
    }
    case GO_ZENTRAL_SZENE:
    {
        // TODO: find better solution for scene assignment
        // should be through ETS parameters, not hardcoded
        switch ((int)go.value(DPT_SceneNumber))
        {
        case 0:
        {
            rgbLight.switchLight(0);
            break;
        }
        case 1: // TV
            rgbLight.setTemperature(3000);
            rgbLight.setBrightness(179); // 70%
            break;
        case 2: // Kochen
            rgbLight.setTemperature(4000);
            rgbLight.setBrightness(128); // 50%
            break;
        case 3: // Essen
        {
            rgbLight.setTemperature(3000);
            rgbLight.setBrightness(128); // 50%
            break;
        }
        }
        break;
    }
    case GO_BELEUCHTUNG_SCHALTEN_EIN_AUS:
    {
        rgbLight.switchLight(go.value(DPT_Switch));
        break;
    }
    case GO_BELEUCHTUNG_HELLIGKEIT_ABSOLUT:
    {
        rgbLight.setBrightness(go.value(DPT_Percent_U8));
        break;
    }
    case GO_BELEUCHTUNG_HELLIGKEIT_RELATIV:
    {
        dpt3_t dimCmd;
        dimCmd.fromDPT3(go.value(DPT_Control_Dimming));
        rgbLight.setRelDimmCmd(dimCmd);
        break;
    }
    case GO_BELEUCHTUNG_FARBE_RGB_ABSOLUT:
    {
        rgb_t rgb;
        rgb.fromDPT232600(go.value(DPT_Colour_RGB));
        rgbLight.setRgb(rgb);
        break;
    }
    case GO_BELEUCHTUNG_FARBE_HSV_ABSOLUT:
    {
        hsv_t hsv;
        hsv.fromDPT232600(go.value(DPT_Colour_RGB));
        rgbLight.setHsv(hsv);
        break;
    }
    case GO_BELEUCHTUNG_FARBTON_RELATIV:
    {
        dpt3_t dimCmd;
        dimCmd.fromDPT3(go.value(DPT_Control_Dimming));
        rgbLight.setRelHueCmd(dimCmd);
        break;
    }
    case GO_BELEUCHTUNG_SAETTIGUNG_RELATIV:
    {
        dpt3_t dimCmd;
        dimCmd.fromDPT3(go.value(DPT_Control_Dimming));
        rgbLight.setRelSaturationCmd(dimCmd);
        break;
    }
    case GO_BELEUCHTUNG_FARBTON_ABSOLUT:
    {      
        hsv_t hsv = rgbLight.getHsv();
        hsv.h = go.value(DPT_Percent_U8);
        rgbLight.setHsv(hsv);
        break;
    }
    case GO_BELEUCHTUNG_SAETTIGUNG_ABSOLUT:
    {      
        hsv_t hsv = rgbLight.getHsv();
        hsv.s = go.value(DPT_Percent_U8);
        rgbLight.setHsv(hsv);
        break;
    }
    }
}

void statusCallback(bool state)
{
    knx.getGroupObject(GO_BELEUCHTUNG_STATUS).value(state, DPT_Switch);
}

void responseBrightnessCallback(uint8_t value)
{
    knx.getGroupObject(GO_BELEUCHTUNG_STATUS_HELLIGKEIT).value(value, DPT_Percent_U8);
}

void responseColorHsvCallback(hsv_t value)
{
    knx.getGroupObject(GO_BELEUCHTUNG_STATUS_FARBE_HSV).value(value.toDPT232600(), DPT_Colour_RGB);
    knx.getGroupObject(GO_BELEUCHTUNG_STATUS_FARBTON).value(value.h, DPT_Percent_U8);
    knx.getGroupObject(GO_BELEUCHTUNG_STATUS_SAETTIGUNG).value(value.s, DPT_Percent_U8);
}

void responseColorRgbCallback(rgb_t value)
{
    knx.getGroupObject(GO_BELEUCHTUNG_STATUS_FARBE_RGB).value(value.toDPT232600(), DPT_Colour_RGB);
}

knxModeOptions_t getKnxMode()
{
    if (knxDisabled)
    {
        return KNX_MODE_OFF;
    }
    else
    {
        return knx.progMode() ? KNX_MODE_PROG : KNX_MODE_NORMAL;
    }
}

void setKnxMode(knxModeOptions_t state)
{
    knxDisabled = state == KNX_MODE_OFF;
    knx.progMode(state == KNX_MODE_PROG);
}

String getPhysAddr()
{
    uint16_t indAddr = knx.individualAddress();
    uint8_t line = (indAddr >> 8) & 0x0F;
    uint8_t area = indAddr >> 12;
    uint8_t member = indAddr & 0xFF;
    return String(line) + "." + String(area) + "." + String(member);
}

void setup()
{
#ifdef ESP32
    Serial.begin(115200);
#else
    Serial.begin(74880);
#endif

    // Green status led on at start
    pinMode(STAT_LED_GN, OUTPUT);
    digitalWrite(STAT_LED_GN, STAT_LED_ON);

    // read adress table, association table, groupobject table and parameters from eeprom
    knx.readMemory();
    knxConfigOk = knx.configured() && knx.getGroupObjectCount() == AMOUNT_OF_GA;

    // Setup device after device was configured by ETS
    if (knxConfigOk)
    {
        GroupObject::classCallback(knxCallback); // callbacks are now handled per class, not per instance

        // read parameters from ETS config
        HOSTNAME = (char *)knx.paramData(0);

        int paraDimmSpeed = (int)knx.paramInt(32);
        rgbLight.configDimmSpeed(paraDimmSpeed);

        paraBrightnessDay = (int)knx.paramInt(36);
        paraBrightnessNight = (int)knx.paramInt(40);
        int paraDefaultTemperature = (int)knx.paramInt(44);
        rgbLight.configDefaultBrightness(paraBrightnessDay);
        rgbLight.configDefaultTemperature(paraDefaultTemperature);
        hsv_t paraDefaultHsv;
        paraDefaultHsv.fromDPT232600(knx.paramInt(48));
        rgbLight.configDefaultHsv(paraDefaultHsv);

        // register callbacks from LED controllers
        rgbLight.registerStatusCallback(statusCallback);
        rgbLight.registerBrightnessCallback(responseBrightnessCallback);
        rgbLight.registerColorHsvCallback(responseColorHsvCallback);
        rgbLight.registerColorRgbCallback(responseColorRgbCallback);
    }
    rgbLight.initRgbLight(LED_PIN_R, LED_PIN_G, LED_PIN_B);

    Serial.println(HOSTNAME);
    Serial.println(getPhysAddr());

    WiFi.persistent(false); // Solve possible wifi init errors
    WiFi.disconnect(true);  // Delete SDK wifi config
    WiFi.mode(WIFI_STA);
    WiFi.hostname(HOSTNAME);
    //WiFi.setAutoConnect(true);
    //WiFi.setAutoReconnect(true);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    WiFi.hostname(HOSTNAME); // Set hostname twice
    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        delay(500);
        // Implement AP mode here after timeout if needed
        // however an open AP can be a security issue
        // settingsAP();
    }

    // Setup and start KNX stack
    // knx.ledPin(PIN_PROGMODE_LED);
    // knx.ledPinActiveOn(PROGMODE_LED_ACT_ON);
    // knx.buttonPin(PIN_PROGMODE_BTN);
    knx.start();

    // Setup and start webserver and OTA
    knxWebServ.setHostname(HOSTNAME);
    knxWebServ.setKnxDetail(getPhysAddr(), knxConfigOk);
    knxWebServ.registerGetKnxModeCallback(getKnxMode);
    knxWebServ.registerSetKnxModeCallback(setKnxMode);
    knxWebServ.startOta();
    knxWebServ.startWeb(WWW_USER, WWW_PASS);

    // Green LED off after WIFI connect
    digitalWrite(STAT_LED_GN, STAT_LED_OFF);
}

void loop()
{
    unsigned long timeCycleStart = micros();
    if (WiFi.status() == WL_CONNECTED)
    {
        knxWebServ.loop();
        if (!knxDisabled)
        {
            knx.loop();
        }

        // only run the application code if the device was configured with ETS
        if (!knxConfigOk)
            return;

        if (!initSent)
        {
            rgbLight.sendStatusUpdate();
            initSent = true;
        }

        rgbLight.loop();
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
    unsigned long cycleTime = micros() - timeCycleStart;
    if (cycleTime < MIN_CYCLE_TIME)
    {
        // delay here until minimum cycle time is reached
        // necessary for constant LED fading times
        delayMicroseconds(MIN_CYCLE_TIME - cycleTime);
    }
}