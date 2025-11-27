#pragma once

#define paramDelay(time) (uint32_t)( \
            (time & 0xC000) == 0xC000 ? (time & 0x3FFF) * 100 : \
            (time & 0xC000) == 0x0000 ? (time & 0x3FFF) * 1000 : \
            (time & 0xC000) == 0x4000 ? (time & 0x3FFF) * 60000 : \
            (time & 0xC000) == 0x8000 ? ((time & 0x3FFF) > 1000 ? 3600000 : \
                                            (time & 0x3FFF) * 3600000 ) : 0 )
#define PT_OnOff_Aus 0
#define PT_OnOff_Ein 1
//--------------------Allgemein---------------------------
#define MAIN_OpenKnxId 0xB0
#define MAIN_ApplicationNumber 0x04
#define MAIN_ApplicationVersion 0x03
#define MAIN_OrderNumber "EspCctLight"
#define MAIN_ParameterSize 37
#define MAIN_MaxKoNumber 9


#define APP_Hostname		0x0000
// Offset: 0, Size: 256 Bit (32 Byte), Text: ESP Hostname
#define ParamAPP_Hostname knx.paramData(APP_Hostname)
#define APP_DimSpeed		0x0020
// Offset: 32, Size: 8 Bit (1 Byte), Text: Dimmgeschwindigkeit
#define ParamAPP_DimSpeed ((uint32_t)((knx.paramByte(APP_DimSpeed))))
#define APP_BrightnessDay		0x0021
// Offset: 33, Size: 8 Bit (1 Byte), Text: Einschalthelligkeit bei Tag
#define ParamAPP_BrightnessDay ((uint32_t)((knx.paramByte(APP_BrightnessDay))))
#define APP_BrightnessNight		0x0022
// Offset: 34, Size: 8 Bit (1 Byte), Text: Einschalthelligkeit bei Nacht
#define ParamAPP_BrightnessNight ((uint32_t)((knx.paramByte(APP_BrightnessNight))))
#define APP_DefaultColorTemp		0x0023
// Offset: 35, Size: 16 Bit (2 Byte), Text: Einschaltfarbtemperatur
#define ParamAPP_DefaultColorTemp ((uint32_t)((knx.paramWord(APP_DefaultColorTemp))))
//!< Number: 1, Text: Zentral, Function: Tag=1 / Nacht=0
#define APP_KoGeneralDayNight 1
#define KoAPP_GeneralDayNight knx.getGroupObject(APP_KoGeneralDayNight)
//!< Number: 2, Text: Beleuchtung, Function: Schalten Ein/Aus
#define APP_KoLightOnOff 2
#define KoAPP_LightOnOff knx.getGroupObject(APP_KoLightOnOff)
//!< Number: 3, Text: Beleuchtung, Function: Dimmen absolut
#define APP_KoLightBrightness 3
#define KoAPP_LightBrightness knx.getGroupObject(APP_KoLightBrightness)
//!< Number: 4, Text: Beleuchtung, Function: Dimmen relativ
#define APP_KoLightDim 4
#define KoAPP_LightDim knx.getGroupObject(APP_KoLightDim)
//!< Number: 5, Text: Beleuchtung, Function: Farbtemperatur absolut
#define APP_KoLightColorTemp 5
#define KoAPP_LightColorTemp knx.getGroupObject(APP_KoLightColorTemp)
//!< Number: 6, Text: Beleuchtung, Function: Farbtemperatur relativ
#define APP_KoLightColorTempRelative 6
#define KoAPP_LightColorTempRelative knx.getGroupObject(APP_KoLightColorTempRelative)
//!< Number: 7, Text: Beleuchtung, Function: Status
#define APP_KoLightStatus 7
#define KoAPP_LightStatus knx.getGroupObject(APP_KoLightStatus)
//!< Number: 8, Text: Beleuchtung, Function: Status Dimmwert
#define APP_KoLightStatusBrightness 8
#define KoAPP_LightStatusBrightness knx.getGroupObject(APP_KoLightStatusBrightness)
//!< Number: 9, Text: Beleuchtung, Function: Status Farbtemperatur
#define APP_KoLightStatusColorTemp 9
#define KoAPP_LightStatusColorTemp knx.getGroupObject(APP_KoLightStatusColorTemp)

