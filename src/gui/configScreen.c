/*
 * Bafang / APT 8X0C / 560C display firmware
 *
 * Based on OpenSourceEBike firmware with Copyrights (C) to Casainho, 2018, 2019, 2020
 * https://github.com/OpenSourceEBike/Color_LCD
 *
 * Adaptation for MPe (ebikecomputer.com)  Copyright (C) BIKEL.PL Marek Przybylak, Maciej Gibowicz 2020, 2021
 *
 * Released under the GPL License, Version 3
 */

#include "stm32f10x.h"
#include "configScreen.h"
#include "screen.h"
#include "texts.h"
#include "buttons.h"
#include "data.h"
#include "lcd.h"
#include "rtc.h"
#include "ugui_config.h"

#define CONFIG_SCREEN_MAX_OBJECTS		configScreen_fieldCount

#define CONF_SCREEN_FONT				CONFIGURATIONS_TEXT_FONT
#define CONF_SCREEN_TXT_ALIGN			ALIGN_TOP_CENTER

#define CONF_SCREEN_X_START				2
#define CONF_SCREEN_Y_START				5
#define CONF_SCREEN_X_END				(SCREEN_WIDTH - CONF_SCREEN_X_START)
#define CONF_SCREEN_Y_END				(SCREEN_HEIGHT - CONF_SCREEN_Y_START)
#define CONF_SCREEN_TXT_HIGH			20
#define CONF_SCREEN_ROW_OFFSET			(CONF_SCREEN_TXT_HIGH + 6)

#ifdef USE_WITH_8X0C
#define CONF_SCREEN_E_LIST_N_ROWS   	1 //number of rows for editable list (>1 must insert \n in text)
#endif
#ifdef USE_WITH_560C
#define CONF_SCREEN_E_LIST_N_ROWS   	2 //number of rows for editable list (>1 must insert \n in text)
#endif
#define CONF_SCREEN_E_LIST_ROW_OFFSET 	CONF_SCREEN_TXT_HIGH*CONF_SCREEN_E_LIST_N_ROWS + 6

#define CONF_SCREEN_MAIN_MENU_Y_END		(SCREEN_HEIGHT * 0.73)
#define CONF_SCREEN_TB_Y_START			(CONF_SCREEN_MAIN_MENU_Y_END + 20)

#define CURRENT_TEXT_MAX_LENGTH		23

UG_WINDOW configScreen_window;
static UG_OBJECT configScreen_objBuff[CONFIG_SCREEN_MAX_OBJECTS];
extern uint16_t buttonsInverted_edit,mainScreen_clockVisibility_edit, mainScreen_temp1Visibility_edit, mainScreen_temp2Visibility_edit;
extern uint16_t selectedParam_edit[];
static char currentText[CURRENT_TEXT_MAX_LENGTH] = "Prad: 0.0 A";

// *********************************** FIELDS ******************************************
static UG_SELECTION configMenuField[] = {FIELD_SELECTION(MPe_texts_mainMenu[Text_mainMenu_batteryCurrent], configScreen_batteryList),
										FIELD_SELECTION(MPe_texts_mainMenu[Text_mainMenu_advancedConfigGasThrottle], configScreen_gasThrottleList),
										FIELD_SELECTION(MPe_texts_mainMenu[Text_mainMenu_speedReadingConfig], configScreen_speedList),
										FIELD_SELECTION(MPe_texts_mainMenu[Text_mainMenu_brakeSensor], configScreen_brakeSensorList),
										FIELD_SELECTION(MPe_texts_mainMenu[Text_mainMenu_readingTemperatureConfig], configScreen_readingTempList),
										FIELD_SELECTION(MPe_texts_mainMenu[Text_mainMenu_PIDConfig], configScreen_PIDreglist),
										FIELD_SELECTION(MPe_texts_mainMenu[Text_mainMenu_PASmodeConfig], configScreen_pasModeList),
										FIELD_SELECTION(MPe_texts_mainMenu[Text_mainMenu_presets], configScreen_presetList),
										FIELD_SELECTION(MPe_texts_mainMenu[Text_mainMenu_selectedParam], configScreen_selectedParamList),
										FIELD_SELECTION(MPe_texts_mainMenu[Text_mainMenu_displayConfig], configScreen_displayList)
};

static UG_EDITABLE batteryField[] = {FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuBatteryCurrent[Text_menuBatteryCurrent_batteryCapacityAh], &MPe_data_configParamValue[ConfigParam_batteryCapacityAh], 0, 65000, 1, 10, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuBatteryCurrent[Text_menuBatteryCurrent_batteryCapacityWh], &MPe_data_configParamValue[ConfigParam_batteryCapacityWh], 0, 65000, 1, 10, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuBatteryCurrent[Text_menuBatteryCurrent_LowVoltageCutoff], &MPe_data_configParamValue[ConfigParam_lowVoltageCutoff], 1, 999, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuBatteryCurrent[Text_menuBatteryCurrent_fullBatteryVoltage], &MPe_data_configParamValue[ConfigParam_fullBatteryVoltage], 1, 999, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuBatteryCurrent[Text_menuBatteryCurrent_currentMv_a], &MPe_data_configParamValue[ConfigParam_currentMv_a], 1, 200, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuBatteryCurrent[Text_menuBatteryCurrent_currentDirection], &MPe_data_configParamValue[ConfigParam_currentDirection], 0, 1, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuBatteryCurrent[Text_menuBatteryCurrent_voltageDivider], &MPe_data_configParamValue[ConfigParam_voltageDivider], 0, 65000, 1, 10, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuBatteryCurrent[Text_menuBatteryCurrent_currentSensorOK], &MPe_data_configParamValue[ConfigParam_currentSensorOK], 0, 1, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuBatteryCurrent[Text_menuBatteryCurrent_currentProtection], &MPe_data_configParamValue[ConfigParam_currentProtection], 0, 30, 1, 10, 100)};

static UG_EDITABLE gasThrottleField[] = {FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_throttleOutMin], &MPe_data_configParamValue[ConfigParam_TOTmin], 0, 450, 5, 10, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_throttleOutMax], &MPe_data_configParamValue[ConfigParam_TOTmax], 0, 450, 5, 10, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_throttleInMin], &MPe_data_configParamValue[ConfigParam_TINmin], 0, 450, 5, 10, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_throttleInMax], &MPe_data_configParamValue[ConfigParam_TINmax], 0, 450, 5, 10, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_throttleReset], &MPe_data_configParamValue[ConfigParam_throttleReset], 0, 1, 1, 10, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_throttleSafeVoltage], &MPe_data_configParamValue[ConfigParam_throttleSafeVoltage], 300, 500, 10, 100, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_ThrottlePowerLimit_1], &MPe_data_configParamValue[ConfigParam_ThrottlePowerLimit_1], 0, 1000, 1, 100, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_ThrottlePowerLimit_2], &MPe_data_configParamValue[ConfigParam_ThrottlePowerLimit_2], 0, 1000, 1, 100, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_ThrottlePowerLimit_3], &MPe_data_configParamValue[ConfigParam_ThrottlePowerLimit_3], 0, 1000, 1, 100, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_ThrottlePowerLimit_4], &MPe_data_configParamValue[ConfigParam_ThrottlePowerLimit_4], 0, 1000, 1, 100, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_ThrottlePowerLimit_5], &MPe_data_configParamValue[ConfigParam_ThrottlePowerLimit_5], 0, 1000, 1, 100, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_ThrottleMode_1], &MPe_data_configParamValue[ConfigParam_ThrottleMode_1], 0, 1, 1, 10, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_ThrottleMode_2], &MPe_data_configParamValue[ConfigParam_ThrottleMode_2], 0, 1, 1, 10, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_ThrottleMode_3], &MPe_data_configParamValue[ConfigParam_ThrottleMode_3], 0, 1, 1, 10, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_ThrottleMode_4], &MPe_data_configParamValue[ConfigParam_ThrottleMode_4], 0, 1, 1, 10, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_ThrottleMode_5], &MPe_data_configParamValue[ConfigParam_ThrottleMode_5], 0, 1, 1, 10, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_ThrottleRampUp_1], &MPe_data_configParamValue[ConfigParam_ThrottleRampUp_1], 0, 10000, 50, 100, 1000),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_ThrottleRampUp_2], &MPe_data_configParamValue[ConfigParam_ThrottleRampUp_2], 0, 10000, 50, 100, 1000),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_ThrottleRampUp_3], &MPe_data_configParamValue[ConfigParam_ThrottleRampUp_3], 0, 10000, 50, 100, 1000),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_ThrottleRampUp_4], &MPe_data_configParamValue[ConfigParam_ThrottleRampUp_4], 0, 10000, 50, 100, 1000),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_ThrottleRampUp_5], &MPe_data_configParamValue[ConfigParam_ThrottleRampUp_5], 0, 10000, 50, 100, 1000)};

static UG_EDITABLE speedField[] = {FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuSpeedReadingConfig[Text_menuSpeedReadingConfig_metricImperial], &MPe_data_configParamValue[ConfigParam_metricImperial], 0, 1, 1, 10, 100),
								   FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuSpeedReadingConfig[Text_menuSpeedReadingConfig_perimeter], &MPe_data_configParamValue[ConfigParam_perimeter], 1, 9999, 1, 10, 100),
								   FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuSpeedReadingConfig[Text_menuSpeedReadingConfig_MotorMagnets], &MPe_data_configParamValue[ConfigParam_MotorMagnets], 1, 999, 1, 10, 100),
								   FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuSpeedReadingConfig[Text_menuSpeedReadingConfig_GearRatio], &MPe_data_configParamValue[ConfigParam_GearRatio], 1, 999, 1, 10, 100)};

static UG_EDITABLE brakeSensorField[] = {FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuBrakeSensor[Text_menuBrakeSensor_brakeHiLo], &MPe_data_configParamValue[ConfigParam_brakeHiLo], 0, 1, 1, 10, 100)};

static UG_EDITABLE readingTempField[] = {FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuReadingTempConfig[Text_menuReadingTempConfig_temperatureUnit], &MPe_data_configParamValue[ConfigParam_temperatureUnit], 0, 1, 1, 10, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuReadingTempConfig[Text_menuReadingTempConfig_temp1Type], &MPe_data_configParamValue[ConfigParam_temp1Type], 0, 4, 1, 10, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuReadingTempConfig[Text_menuReadingTempConfig_temp2Type], &MPe_data_configParamValue[ConfigParam_temp2Type], 0, 4, 1, 10, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuReadingTempConfig[Text_menuReadingTempConfig_temp1Overheat], &MPe_data_configParamValue[ConfigParam_temp1Overheat], 1, 500, 1, 10, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuReadingTempConfig[Text_menuReadingTempConfig_temp2Overheat], &MPe_data_configParamValue[ConfigParam_temp2Overheat], 1, 500, 1, 10, 100)};

static UG_EDITABLE PIDregField[] =     {FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPIDConfig[Text_menuPIDConfig_POWERKP], &MPe_data_configParamValue[ConfigParam_POWERKP], 0, 1000, 10, 50, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPIDConfig[Text_menuPIDConfig_POWERKI], &MPe_data_configParamValue[ConfigParam_POWERKI], 0, 1000, 10, 50, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPIDConfig[Text_menuPIDConfig_POWERKD], &MPe_data_configParamValue[ConfigParam_POWERKD], 0, 1000, 10, 50, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPIDConfig[Text_menuPIDConfig_PLOW], &MPe_data_configParamValue[ConfigParam_PLOW], 0, 1000, 10, 50, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPIDConfig[Text_menuPIDConfig_ILOW], &MPe_data_configParamValue[ConfigParam_ILOW], 0, 1000, 10, 50, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPIDConfig[Text_menuPIDConfig_DLOW], &MPe_data_configParamValue[ConfigParam_DLOW], 0, 1000, 10, 50, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPIDConfig[Text_menuPIDConfig_LOW_Threshold], &MPe_data_configParamValue[ConfigParam_PID_Low_Threshold], 0, 1000, 10, 50, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPIDConfig[Text_menuPIDConfig_SpeedFactorMin], &MPe_data_configParamValue[ConfigParam_SpeedFactorMin], 1, 100, 1, 10, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPIDConfig[Text_menuPIDConfig_PIDPWMMax], &MPe_data_configParamValue[ConfigParam_PIDPWMMax], 1, 255, 5, 50, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPIDConfig[Text_menuPIDConfig_SpeedFactorRampUp], &MPe_data_configParamValue[ConfigParam_SpeedFactorRampUp], 0, 3000, 10, 50, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPIDConfig[Text_menuPIDConfig_CruiseControlPowerMin], &MPe_data_configParamValue[ConfigParam_CruiseControlPowerMin], 0, 2000, 10, 50, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPIDConfig[Text_menuPIDConfig_CruiseControlPowerMax], &MPe_data_configParamValue[ConfigParam_CruiseControlPowerMax], 0, 2000, 10, 50, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPIDConfig[Text_menuPIDConfig_CruiseControlPowerRampUp], &MPe_data_configParamValue[ConfigParam_CruiseControlPowerRampUp], 0, 3000, 10, 50, 100),
										FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPIDConfig[Text_menuPIDConfig_CruiseControlMaxSpeed], &MPe_data_configParamValue[ConfigParam_CruiseControlMaxSpeed], 0, 40, 5, 10, 100)};

static UG_EDITABLE pasModeField[] = {FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_AutoLegal], &MPe_data_configParamValue[ConfigParam_AutoLegal], 0, 1, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_LegalLimitOnOff], &MPe_data_configParamValue[ConfigParam_LegalLimitOnOff], 0, 1, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_LegalLimitSpeed], &MPe_data_configParamValue[ConfigParam_LegalLimitSpeed], 0, 100, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_LegalLimitPower], &MPe_data_configParamValue[ConfigParam_LegalLimitPower], 0, 1000, 10, 50, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_PASMagnets], &MPe_data_configParamValue[ConfigParam_PASMagnets], 1, 50, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_PowerLimitPAS_1], &MPe_data_configParamValue[ConfigParam_PowerLimitPAS_1], 1, 3000, 1, 50, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_PowerLimitPAS_2], &MPe_data_configParamValue[ConfigParam_PowerLimitPAS_2], 1, 3000, 1, 50, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_PowerLimitPAS_3], &MPe_data_configParamValue[ConfigParam_PowerLimitPAS_3], 1, 3000, 1, 50, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_PowerLimitPAS_4], &MPe_data_configParamValue[ConfigParam_PowerLimitPAS_4], 1, 3000, 1, 50, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_PowerLimitPAS_5], &MPe_data_configParamValue[ConfigParam_PowerLimitPAS_5], 1, 3000, 1, 50, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_SpeedLimitPAS_1], &MPe_data_configParamValue[ConfigParam_SpeedLimitPAS_1], 10, 100, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_SpeedLimitPAS_2], &MPe_data_configParamValue[ConfigParam_SpeedLimitPAS_2], 10, 100, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_SpeedLimitPAS_3], &MPe_data_configParamValue[ConfigParam_SpeedLimitPAS_3], 10, 100, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_SpeedLimitPAS_4], &MPe_data_configParamValue[ConfigParam_SpeedLimitPAS_4], 10, 100, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_SpeedLimitPAS_5], &MPe_data_configParamValue[ConfigParam_SpeedLimitPAS_5], 10, 100, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_MinSpeedPAS_1], &MPe_data_configParamValue[ConfigParam_MinSpeedPAS_1], 0, 100, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_MinSpeedPAS_2], &MPe_data_configParamValue[ConfigParam_MinSpeedPAS_2], 0, 100, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_MinSpeedPAS_3], &MPe_data_configParamValue[ConfigParam_MinSpeedPAS_3], 0, 100, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_MinSpeedPAS_4], &MPe_data_configParamValue[ConfigParam_MinSpeedPAS_4], 0, 100, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_MinSpeedPAS_5], &MPe_data_configParamValue[ConfigParam_MinSpeedPAS_5], 0, 100, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_CadenceMinPAS_1], &MPe_data_configParamValue[ConfigParam_CadenceMinPAS_1], 0, 200, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_CadenceMinPAS_2], &MPe_data_configParamValue[ConfigParam_CadenceMinPAS_2], 0, 200, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_CadenceMinPAS_3], &MPe_data_configParamValue[ConfigParam_CadenceMinPAS_3], 0, 200, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_CadenceMinPAS_4], &MPe_data_configParamValue[ConfigParam_CadenceMinPAS_4], 0, 200, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_CadenceMinPAS_5], &MPe_data_configParamValue[ConfigParam_CadenceMinPAS_5], 0, 200, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_CadenceMaxPAS_1], &MPe_data_configParamValue[ConfigParam_CadenceMaxPAS_1], 1, 200, 10, 20, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_CadenceMaxPAS_2], &MPe_data_configParamValue[ConfigParam_CadenceMaxPAS_2], 1, 200, 10, 20, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_CadenceMaxPAS_3], &MPe_data_configParamValue[ConfigParam_CadenceMaxPAS_3], 1, 200, 10, 20, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_CadenceMaxPAS_4], &MPe_data_configParamValue[ConfigParam_CadenceMaxPAS_4], 1, 200, 10, 20, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_CadenceMaxPAS_5], &MPe_data_configParamValue[ConfigParam_CadenceMaxPAS_5], 1, 200, 10, 20, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_RampUpPAS_1], &MPe_data_configParamValue[ConfigParam_RampUpPAS_1], 50, 2000, 50, 100, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_RampUpPAS_2], &MPe_data_configParamValue[ConfigParam_RampUpPAS_2], 50, 2000, 50, 100, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_RampUpPAS_3], &MPe_data_configParamValue[ConfigParam_RampUpPAS_3], 50, 2000, 50, 100, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_RampUpPAS_4], &MPe_data_configParamValue[ConfigParam_RampUpPAS_4], 50, 2000, 50, 100, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_RampUpPAS_5], &MPe_data_configParamValue[ConfigParam_RampUpPAS_5], 50, 2000, 50, 100, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_BoostPowerPAS_1], &MPe_data_configParamValue[ConfigParam_BoostPowerPAS_1], 0, 10000, 50, 100, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_BoostPowerPAS_2], &MPe_data_configParamValue[ConfigParam_BoostPowerPAS_2], 0, 10000, 50, 100, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_BoostPowerPAS_3], &MPe_data_configParamValue[ConfigParam_BoostPowerPAS_3], 0, 10000, 50, 100, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_BoostPowerPAS_4], &MPe_data_configParamValue[ConfigParam_BoostPowerPAS_4], 0, 10000, 50, 100, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_BoostPowerPAS_5], &MPe_data_configParamValue[ConfigParam_BoostPowerPAS_5], 0, 10000, 50, 100, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_BoostTimePAS_1], &MPe_data_configParamValue[ConfigParam_BoostTimePAS_1], 0, 10000, 50, 100, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_BoostTimePAS_2], &MPe_data_configParamValue[ConfigParam_BoostTimePAS_2], 0, 10000, 50, 100, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_BoostTimePAS_3], &MPe_data_configParamValue[ConfigParam_BoostTimePAS_3], 0, 10000, 50, 100, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_BoostTimePAS_4], &MPe_data_configParamValue[ConfigParam_BoostTimePAS_4], 0, 10000, 50, 100, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_BoostTimePAS_5], &MPe_data_configParamValue[ConfigParam_BoostTimePAS_5], 0, 10000, 50, 100, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_BoostSpeedPAS_1], &MPe_data_configParamValue[ConfigParam_BoostSpeedPAS_1], 0, 100, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_BoostSpeedPAS_2], &MPe_data_configParamValue[ConfigParam_BoostSpeedPAS_2], 0, 100, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_BoostSpeedPAS_3], &MPe_data_configParamValue[ConfigParam_BoostSpeedPAS_3], 0, 100, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_BoostSpeedPAS_4], &MPe_data_configParamValue[ConfigParam_BoostSpeedPAS_4], 0, 100, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_BoostSpeedPAS_5], &MPe_data_configParamValue[ConfigParam_BoostSpeedPAS_5], 0, 100, 1, 10, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_BoostRampUpPAS], &MPe_data_configParamValue[ConfigParam_BoostRampUpPAS], 50, 5000, 50, 100, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_CadenceRefreshTime], &MPe_data_configParamValue[ConfigParam_CadenceRefreshTime], 150, 750, 10, 50, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_EnableTorqueSensor], &MPe_data_configParamValue[ConfigParam_EnableTorqueSensor], 0, 1, 1, 20, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_WeightOnPedalStartup], &MPe_data_configParamValue[ConfigParam_WeightOnPedalStartup], 50, 1000, 10, 20, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_TorqueSensorADCMin], &MPe_data_configParamValue[ConfigParam_TorqueSensorADCMin], 0, 1023, 10, 20, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_TorqueSensorADCMax], &MPe_data_configParamValue[ConfigParam_TorqueSensorADCMax], 0, 1023, 10, 20, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_TorqueSensorkgFMax], &MPe_data_configParamValue[ConfigParam_TorqueSensorkgFMax], 0, 1000, 10, 20, 100)};

static UG_EDITABLE presetsField[] = {FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPresetsConfig[Text_menuPresetsConfig_distancePreset], &data_presetDistance_edit, 0, 65000, 10, 100, 1000),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuPresetsConfig[Text_menuPresetsConfig_battChargeNoPreset], &data_presetBattChargeNo_edit, 0, 65000, 1, 10, 100)};

static UG_EDITABLE selectedParamField[] = {FIELD_EDITABLE(MPe_texts_menuSelectedParam[Text_menuSelectedParam_1], &selectedParam_edit[0], 0, Param_NumberOfParam - 1),
									FIELD_EDITABLE(MPe_texts_menuSelectedParam[Text_menuSelectedParam_2], &selectedParam_edit[1], 0, Param_NumberOfParam - 1),
									FIELD_EDITABLE(MPe_texts_menuSelectedParam[Text_menuSelectedParam_3], &selectedParam_edit[2], 0, Param_NumberOfParam - 1),
									FIELD_EDITABLE(MPe_texts_menuSelectedParam[Text_menuSelectedParam_4], &selectedParam_edit[3], 0, Param_NumberOfParam - 1),
									FIELD_EDITABLE(MPe_texts_menuSelectedParam[Text_menuSelectedParam_5], &selectedParam_edit[4], 0, Param_NumberOfParam - 1),
									FIELD_EDITABLE(MPe_texts_menuSelectedParam[Text_menuSelectedParam_6], &selectedParam_edit[5], 0, Param_NumberOfParam - 1),
									FIELD_EDITABLE(MPe_texts_menuSelectedParam[Text_menuSelectedParam_7], &selectedParam_edit[6], 0, Param_NumberOfParam - 1),
									FIELD_EDITABLE(MPe_texts_menuSelectedParam[Text_menuSelectedParam_8], &selectedParam_edit[7], 0, Param_NumberOfParam - 1)};

static UG_EDITABLE displayField[] = {//FIELD_EDITABLE(MPe_texts_menuDisplayConfig[Text_menuDisplayConfig_clockHour], &rtc_editTime_hour, 0, 23),
									//FIELD_EDITABLE(MPe_texts_menuDisplayConfig[Text_menuDisplayConfig_clockMinutes], &rtc_editTime_min, 0, 59),
									//FIELD_EDITABLE(MPe_texts_menuDisplayConfig[Text_menuDisplayConfig_backlightDayBrightness], &lcd_backlightDayBrightness_edit, 0, 100),
									//FIELD_EDITABLE(MPe_texts_menuDisplayConfig[Text_menuDisplayConfig_backlightNightBrightness], &lcd_backlightNightBrightness_edit, 0, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuDisplayConfig[Text_menuDisplayConfig_backlightDayBrightness], &lcd_backlightDayBrightness_edit, 0, 100, 10, 20, 100),
									FIELD_EDITABLE_CUSTOM_STEPS(MPe_texts_menuDisplayConfig[Text_menuDisplayConfig_backlightNightBrightness], &lcd_backlightNightBrightness_edit, 0, 100, 10, 20, 100),
									FIELD_EDITABLE(MPe_texts_menuDisplayConfig[Text_menuDisplayConfig_autoPowerOff], &lcd_powerOffTimeMinutes_edit, 1, 999),
									//FIELD_EDITABLE(MPe_texts_menuDisplayConfig[Text_menuDisplayConfig_clockVisibility], &mainScreen_clockVisibility_edit, 0, 1),
									FIELD_EDITABLE(MPe_texts_menuDisplayConfig[Text_menuDisplayConfig_temp1Visibility], &mainScreen_temp1Visibility_edit, 0, 1),
									FIELD_EDITABLE(MPe_texts_menuDisplayConfig[Text_menuDisplayConfig_temp2Visibility], &mainScreen_temp2Visibility_edit, 0, 1),
									FIELD_EDITABLE(MPe_texts_menuDisplayConfig[Text_menuDisplayConfig_buttonsInverted], &buttonsInverted_edit, 0, 1),
									FIELD_EDITABLE(MPe_texts_menuDisplayConfig[Text_menuDisplayConfig_language], &text_language_edit, 0, 1)};

// *********************************** LISTS ******************************************
static UG_SLIST configMenu = FIELD_SLIST(&CONF_SCREEN_FONT, CONF_SCREEN_TXT_ALIGN, CONF_SCREEN_ROW_OFFSET,
										TBL_SIZE(configMenuField), C_YELLOW, &configMenuField);

static UG_ELIST batteryList = FIELD_ELIST(&CONF_SCREEN_FONT, CONF_SCREEN_E_LIST_ROW_OFFSET,
										TBL_SIZE(batteryField), C_YELLOW, &batteryField, CONF_SCREEN_E_LIST_N_ROWS);

static UG_ELIST gasThrottleList = FIELD_ELIST(&CONF_SCREEN_FONT, CONF_SCREEN_E_LIST_ROW_OFFSET,
										TBL_SIZE(gasThrottleField), C_YELLOW, &gasThrottleField, CONF_SCREEN_E_LIST_N_ROWS);

static UG_ELIST speedList = FIELD_ELIST(&CONF_SCREEN_FONT, CONF_SCREEN_E_LIST_ROW_OFFSET,
										TBL_SIZE(speedField), C_YELLOW, &speedField, CONF_SCREEN_E_LIST_N_ROWS);

static UG_ELIST brakeSensorList = FIELD_ELIST(&CONF_SCREEN_FONT, CONF_SCREEN_E_LIST_ROW_OFFSET,
										TBL_SIZE(brakeSensorField), C_YELLOW, &brakeSensorField, CONF_SCREEN_E_LIST_N_ROWS);

static UG_ELIST readingTempList = FIELD_ELIST(&CONF_SCREEN_FONT, CONF_SCREEN_E_LIST_ROW_OFFSET,
										TBL_SIZE(readingTempField), C_YELLOW, &readingTempField, CONF_SCREEN_E_LIST_N_ROWS);

static UG_ELIST PIDreglist = FIELD_ELIST(&CONF_SCREEN_FONT, CONF_SCREEN_E_LIST_ROW_OFFSET,
										TBL_SIZE(PIDregField), C_YELLOW, &PIDregField, CONF_SCREEN_E_LIST_N_ROWS);

static UG_ELIST pasModeList = FIELD_ELIST(&CONF_SCREEN_FONT, CONF_SCREEN_E_LIST_ROW_OFFSET,
										TBL_SIZE(pasModeField), C_YELLOW, &pasModeField, CONF_SCREEN_E_LIST_N_ROWS);

static UG_ELIST presetsList = FIELD_ELIST(&CONF_SCREEN_FONT, CONF_SCREEN_E_LIST_ROW_OFFSET,
										TBL_SIZE(presetsField), C_YELLOW, &presetsField, CONF_SCREEN_E_LIST_N_ROWS);

static UG_ELIST selectedParamList = FIELD_ELIST(&CONF_SCREEN_FONT, CONF_SCREEN_E_LIST_ROW_OFFSET,
										TBL_SIZE(selectedParamField), C_YELLOW, &selectedParamField, CONF_SCREEN_E_LIST_N_ROWS);

static UG_ELIST displayList = FIELD_ELIST(&CONF_SCREEN_FONT, CONF_SCREEN_E_LIST_ROW_OFFSET,
										TBL_SIZE(displayField), C_YELLOW, &displayField, CONF_SCREEN_E_LIST_N_ROWS);

static UG_TEXTBOX holdTB = FIELD_READONLY_STRING(MPe_texts_configScreen[Text_configScreen_hold], &CONF_SCREEN_FONT, ALIGN_TOP_CENTER);
static UG_TEXTBOX resetTB = FIELD_READONLY_STRING(MPe_texts_configScreen[Text_configScreen_reset], &CONF_SCREEN_FONT, ALIGN_TOP_CENTER);
static UG_TEXTBOX currentTB = FIELD_READONLY_STRING(currentText, &CONF_SCREEN_FONT, ALIGN_TOP_CENTER);

static configScreenField_t activeFieldId = configScreen_menuList;
extern UG_WINDOW mainScreen_window;

extern void mainScreenCommon_synchro(void);
extern void mainScreen2_synchro(void);

void configScreen_callback( UG_MESSAGE* msg )
{
	if (msg->type == MSG_TYPE_OBJECT)
	{
		if (msg->id == OBJ_TYPE_BUTTON)
	    {
			if (msg->event == UP_CLICK)
		    {
				if (activeFieldId == configScreen_menuList)
				{
					UG_SelectListDecrementPosition(&configScreen_window, configScreen_menuList);
				}
				else
				{
					if (UG_EditListIsActive(&configScreen_window, activeFieldId))
					{
						UG_EditListIncrementValue(&configScreen_window, activeFieldId,
								(msg->click == FAST_REPEAT) ? ELST_FAST_STEP :
								(msg->click == MEDIUM_REPEAT) ? ELST_MEDIUM_STEP : ELST_SLOW_STEP);
					}
					else UG_EditListDecrementPosition(&configScreen_window, activeFieldId);
				}
		    }
			else if (msg->event == DOWN_CLICK)
			{
				if (activeFieldId == configScreen_menuList)
				{
					UG_SelectListIncrementPosition(&configScreen_window, configScreen_menuList);
				}
				else
				{
					if (UG_EditListIsActive(&configScreen_window, activeFieldId))
					{
						UG_EditListDecrementValue(&configScreen_window, activeFieldId,
								(msg->click == FAST_REPEAT) ? ELST_FAST_STEP :
								(msg->click == MEDIUM_REPEAT) ? ELST_MEDIUM_STEP : ELST_SLOW_STEP);
					}
					else UG_EditListIncrementPosition(&configScreen_window, activeFieldId);
				}
			}
			else if (msg->event == ONOFF_CLICK
					&& msg->click == ONE_CLICK)
			{
				if (activeFieldId == configScreen_menuList)
				{
					activeFieldId = UG_SelectListShowSelectedField(&configScreen_window, activeFieldId);
					// hide current
					UG_TextboxHide(&configScreen_window, configScreen_holdTB);
					UG_TextboxHide(&configScreen_window, configScreen_resetTB);
					UG_TextboxHide(&configScreen_window, configScreen_currentTB);
				}
				else
				{
					if (UG_EditListIsActive(&configScreen_window, activeFieldId))
					{
						UG_EditListResetActive(&configScreen_window, activeFieldId);
						if (activeFieldId == configScreen_presetList)
						{
							// presets synchro on exit presets settings
							data_synchro();
						}
					}
					else UG_EditListSetActive(&configScreen_window, activeFieldId);
				}
			}
			else if (msg->event == ONOFF_LONG_CLICK
					&& msg->click == ONE_CLICK)
			{
				if (activeFieldId == configScreen_menuList)
				{
					screen_changeWindow(&mainScreen_window);
				}
				else
				{
					if (activeFieldId == configScreen_displayList)
					{
						// eeprom synchro on exit display settings
						lcd_synchro();
						rtc_synchro();
						texts_synchro();
						buttons_synchro();
						mainScreenCommon_synchro();
					}
					else if (activeFieldId == configScreen_selectedParamList)
					{
						mainScreen2_synchro();
					}

					activeFieldId = UG_SelectListShowMainMenu(&configScreen_window, activeFieldId, configScreen_menuList);
					// show current
					UG_TextboxShow(&configScreen_window, configScreen_holdTB);
					UG_TextboxShow(&configScreen_window, configScreen_resetTB);
					UG_TextboxShow(&configScreen_window, configScreen_currentTB);
				}
			}
			else if (msg->event == ONOFFDOWN_LONG_CLICK)
			{
				data_resetCurrentSensorRequest();
			}
	    }
	}
}

void configScreen_init(void)
{
	uint16_t yOffset = 0;

	// Create Window
	UG_WindowCreate( &configScreen_window, configScreen_objBuff, CONFIG_SCREEN_MAX_OBJECTS, configScreen_callback );
	UG_WindowSetBackColor( &configScreen_window, C_BLACK);
	UG_WindowSetForeColor( &configScreen_window, C_WHITE);
	UG_WindowSetStyle(&configScreen_window, WND_STYLE_2D);

	// Create Select List
	UG_SelectListCreate(&configScreen_window, &configMenu, configScreen_menuList, CONF_SCREEN_X_START, CONF_SCREEN_Y_START, CONF_SCREEN_X_END, CONF_SCREEN_MAIN_MENU_Y_END);

	// Create Edit List
	UG_EditListCreate(&configScreen_window, &batteryList, configScreen_batteryList, CONF_SCREEN_X_START, CONF_SCREEN_Y_START, CONF_SCREEN_X_END, CONF_SCREEN_Y_END);
	UG_EditListHide(&configScreen_window, configScreen_batteryList);

	UG_EditListCreate(&configScreen_window, &gasThrottleList, configScreen_gasThrottleList, CONF_SCREEN_X_START, CONF_SCREEN_Y_START, CONF_SCREEN_X_END, CONF_SCREEN_Y_END);
	UG_EditListHide(&configScreen_window, configScreen_gasThrottleList);

	UG_EditListCreate(&configScreen_window, &speedList, configScreen_speedList, CONF_SCREEN_X_START, CONF_SCREEN_Y_START, CONF_SCREEN_X_END, CONF_SCREEN_Y_END);
	UG_EditListHide(&configScreen_window, configScreen_speedList);

	UG_EditListCreate(&configScreen_window, &brakeSensorList, configScreen_brakeSensorList, CONF_SCREEN_X_START, CONF_SCREEN_Y_START, CONF_SCREEN_X_END, CONF_SCREEN_Y_END);
	UG_EditListHide(&configScreen_window, configScreen_brakeSensorList);

	UG_EditListCreate(&configScreen_window, &readingTempList, configScreen_readingTempList, CONF_SCREEN_X_START, CONF_SCREEN_Y_START, CONF_SCREEN_X_END, CONF_SCREEN_Y_END);
	UG_EditListHide(&configScreen_window, configScreen_readingTempList);

	UG_EditListCreate(&configScreen_window, &PIDreglist, configScreen_PIDreglist, CONF_SCREEN_X_START, CONF_SCREEN_Y_START, CONF_SCREEN_X_END, CONF_SCREEN_Y_END);
	UG_EditListHide(&configScreen_window, configScreen_PIDreglist);

	UG_EditListCreate(&configScreen_window, &pasModeList, configScreen_pasModeList, CONF_SCREEN_X_START, CONF_SCREEN_Y_START, CONF_SCREEN_X_END, CONF_SCREEN_Y_END);
	UG_EditListHide(&configScreen_window, configScreen_pasModeList);

	UG_EditListCreate(&configScreen_window, &presetsList, configScreen_presetList, CONF_SCREEN_X_START, CONF_SCREEN_Y_START, CONF_SCREEN_X_END, CONF_SCREEN_Y_END);
	UG_EditListHide(&configScreen_window, configScreen_presetList);

	UG_EditListCreate(&configScreen_window, &selectedParamList, configScreen_selectedParamList, CONF_SCREEN_X_START, CONF_SCREEN_Y_START, CONF_SCREEN_X_END, CONF_SCREEN_Y_END);
	UG_EditListHide(&configScreen_window, configScreen_selectedParamList);

	UG_EditListCreate(&configScreen_window, &displayList, configScreen_displayList, CONF_SCREEN_X_START, CONF_SCREEN_Y_START, CONF_SCREEN_X_END, CONF_SCREEN_Y_END);
	UG_EditListHide(&configScreen_window, configScreen_displayList);

	// Create Textbox
	yOffset = CONF_SCREEN_TB_Y_START;
	UG_TextboxCreate( &configScreen_window, &holdTB, configScreen_holdTB, CONF_SCREEN_X_START, yOffset, CONF_SCREEN_X_END, CONF_SCREEN_Y_END );
	yOffset += CONF_SCREEN_ROW_OFFSET;
	UG_TextboxCreate( &configScreen_window, &resetTB, configScreen_resetTB, CONF_SCREEN_X_START, yOffset, CONF_SCREEN_X_END, CONF_SCREEN_Y_END );
	yOffset += CONF_SCREEN_ROW_OFFSET;
	UG_TextboxCreate( &configScreen_window, &currentTB, configScreen_currentTB, CONF_SCREEN_X_START, yOffset, CONF_SCREEN_X_END, CONF_SCREEN_Y_END );
}

void configScreen_updateCurrent(void)
{
	snprintf(currentText, CURRENT_TEXT_MAX_LENGTH, "%s %s A", MPe_texts_statScreen[Texts_statScreen_current], MPe_data_param[Param_Current]);
}
void configScreen_update(configScreenField_t field)
{
	for (uint8_t i = 0; i < CONFIG_SCREEN_MAX_OBJECTS; i++)
	{
		if (configScreen_objBuff[i].id == field)
		{
			if (field == configScreen_currentTB) configScreen_updateCurrent();
			configScreen_objBuff[i].state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;
			return;
		}
	}
}
