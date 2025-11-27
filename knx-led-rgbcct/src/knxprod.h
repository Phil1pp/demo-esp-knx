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
#define MAIN_ApplicationNumber 0x05
#define MAIN_ApplicationVersion 0x03
#define MAIN_OrderNumber "EspRGBcctLight"
#define MAIN_ParameterSize 40
#define MAIN_MaxKoNumber 19


#define APP_Hostname		0x0000
// Offset: 0, Size: 256 Bit (32 Byte), Text: ESP Hostname
#define ParamAPP_Hostname knx.paramData(APP_Hostname)
#define APP_DimSpeed		0x0020
// Offset: 32, Size: 8 Bit (1 Byte), Text: Dimmgeschwindigkeit
#define ParamAPP_DimSpeed ((uint32_t)((knx.paramByte(APP_DimSpeed))))
#define APP_LightBrightnessDay		0x0021
// Offset: 33, Size: 8 Bit (1 Byte), Text: Einschalthelligkeit bei Tag
#define ParamAPP_LightBrightnessDay ((uint32_t)((knx.paramByte(APP_LightBrightnessDay))))
#define APP_LightBrightnessNight		0x0022
// Offset: 34, Size: 8 Bit (1 Byte), Text: Einschalthelligkeit bei Nacht
#define ParamAPP_LightBrightnessNight ((uint32_t)((knx.paramByte(APP_LightBrightnessNight))))
#define APP_LightDefaultColorTemp		0x0023
// Offset: 35, Size: 16 Bit (2 Byte), Text: Einschaltfarbtemperatur
#define ParamAPP_LightDefaultColorTemp ((uint32_t)((knx.paramWord(APP_LightDefaultColorTemp))))
#define APP_LightDefaultColor		0x0025
// Offset: 37, Size: 24 Bit (3 Byte), Text: Einschaltfarbe
#define ParamAPP_LightDefaultColor knx.paramData(APP_LightDefaultColor)
//!< Number: 1, Text: Zentral, Function: Tag=1 / Nacht=0
#define APP_KoGeneralDayNight 1
#define KoAPP_GeneralDayNight knx.getGroupObject(APP_KoGeneralDayNight)
//!< Number: 2, Text: Beleuchtung, Function: Schalten Ein/Aus
#define APP_KoLightOnOff 2
#define KoAPP_LightOnOff knx.getGroupObject(APP_KoLightOnOff)
//!< Number: 3, Text: Beleuchtung, Function: Helligkeit absolut (V)
#define APP_KoLightBrightnessAbs 3
#define KoAPP_LightBrightnessAbs knx.getGroupObject(APP_KoLightBrightnessAbs)
//!< Number: 4, Text: Beleuchtung, Function: Helligkeit relativ (V)
#define APP_KoLightBrightnessRel 4
#define KoAPP_LightBrightnessRel knx.getGroupObject(APP_KoLightBrightnessRel)
//!< Number: 5, Text: Beleuchtung, Function: Sättigung absolut (S)
#define APP_KoLightSaturationAbs 5
#define KoAPP_LightSaturationAbs knx.getGroupObject(APP_KoLightSaturationAbs)
//!< Number: 6, Text: Beleuchtung, Function: Sättigung relativ (S)
#define APP_KoLightSaturationRel 6
#define KoAPP_LightSaturationRel knx.getGroupObject(APP_KoLightSaturationRel)
//!< Number: 7, Text: Beleuchtung, Function: Farbton absolut (H)
#define APP_KoLightHueAbs 7
#define KoAPP_LightHueAbs knx.getGroupObject(APP_KoLightHueAbs)
//!< Number: 8, Text: Beleuchtung, Function: Farbton relativ (H)
#define APP_KoLightHueRel 8
#define KoAPP_LightHueRel knx.getGroupObject(APP_KoLightHueRel)
//!< Number: 9, Text: Beleuchtung, Function: Farbtemperatur absolut
#define APP_KoLightColorTempAbs 9
#define KoAPP_LightColorTempAbs knx.getGroupObject(APP_KoLightColorTempAbs)
//!< Number: 10, Text: Beleuchtung, Function: Farbtemperatur relativ
#define APP_KoLightColorTempRel 10
#define KoAPP_LightColorTempRel knx.getGroupObject(APP_KoLightColorTempRel)
//!< Number: 11, Text: Beleuchtung, Function: Farbe RGB absolut
#define APP_KoLightColorRgb 11
#define KoAPP_LightColorRgb knx.getGroupObject(APP_KoLightColorRgb)
//!< Number: 12, Text: Beleuchtung, Function: Farbe HSV absolut
#define APP_KoLightColorHsv 12
#define KoAPP_LightColorHsv knx.getGroupObject(APP_KoLightColorHsv)
//!< Number: 13, Text: Beleuchtung, Function: Status
#define APP_KoLightStatus 13
#define KoAPP_LightStatus knx.getGroupObject(APP_KoLightStatus)
//!< Number: 14, Text: Beleuchtung, Function: Status Helligkeit (V)
#define APP_KoLightStatusBrightness 14
#define KoAPP_LightStatusBrightness knx.getGroupObject(APP_KoLightStatusBrightness)
//!< Number: 15, Text: Beleuchtung, Function: Status Sättigung (S)
#define APP_KoLightStatusSaturation 15
#define KoAPP_LightStatusSaturation knx.getGroupObject(APP_KoLightStatusSaturation)
//!< Number: 16, Text: Beleuchtung, Function: Status Farbton (H)
#define APP_KoLightStatusHue 16
#define KoAPP_LightStatusHue knx.getGroupObject(APP_KoLightStatusHue)
//!< Number: 17, Text: Beleuchtung, Function: Status Farbtemperatur
#define APP_KoLightStatusColorTemp 17
#define KoAPP_LightStatusColorTemp knx.getGroupObject(APP_KoLightStatusColorTemp)
//!< Number: 18, Text: Beleuchtung, Function: Status Farbe RGB
#define APP_KoLightStatusColorRgb 18
#define KoAPP_LightStatusColorRgb knx.getGroupObject(APP_KoLightStatusColorRgb)
//!< Number: 19, Text: Beleuchtung, Function: Status Farbe HSV
#define APP_KoLightStatusColorHsv 19
#define KoAPP_LightStatusColorHsv knx.getGroupObject(APP_KoLightStatusColorHsv)

