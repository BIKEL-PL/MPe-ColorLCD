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

#ifndef MPE_TEXTS_H_
#define MPE_TEXTS_H_

#include "stm32f10x.h"

#define MPE_MAX_TEXT_LENGTH		40

typedef enum {
	languages_polish,
	languages_english,

	languages_count
} Text_languages_t;

enum Texts_mainMenu
{
	Text_mainMenu_batteryCurrent,
	Text_mainMenu_advancedConfigGasThrottle,
	Text_mainMenu_speedReadingConfig,
	Text_mainMenu_brakeSensor,
	Text_mainMenu_readingTemperatureConfig,
	Text_mainMenu_PIDConfig,
	Text_mainMenu_PASmodeConfig,
	Text_mainMenu_presets,
	Text_mainMenu_selectedParam,
	Text_mainMenu_displayConfig,

	Text_mainMenu_numberOfTexts
};
enum Texts_menuBatteryCurrent
{
	Text_menuBatteryCurrent_batteryCapacityAh,
	Text_menuBatteryCurrent_batteryCapacityWh,
	Text_menuBatteryCurrent_LowVoltageCutoff,
	Text_menuBatteryCurrent_fullBatteryVoltage,
	Text_menuBatteryCurrent_currentMv_a,
	Text_menuBatteryCurrent_currentDirection,
	Text_menuBatteryCurrent_voltageDivider,
	Text_menuBatteryCurrent_currentSensorOK,
	Text_menuBatteryCurrent_currentProtection,

	Text_menuBatteryCurrent_numberOfTexts
};
enum Texts_menuAdvConfigGasThrottle
{
	Text_menuAdvConfigGasThrottle_throttleOutMin,
	Text_menuAdvConfigGasThrottle_throttleOutMax,
	Text_menuAdvConfigGasThrottle_throttleInMin,
	Text_menuAdvConfigGasThrottle_throttleInMax,
	Text_menuAdvConfigGasThrottle_throttleReset,
	Text_menuAdvConfigGasThrottle_throttleSafeVoltage,
	Text_menuAdvConfigGasThrottle_ThrottleMode_1,
	Text_menuAdvConfigGasThrottle_ThrottleMode_2,
	Text_menuAdvConfigGasThrottle_ThrottleMode_3,
	Text_menuAdvConfigGasThrottle_ThrottleMode_4,
	Text_menuAdvConfigGasThrottle_ThrottleMode_5,
	Text_menuAdvConfigGasThrottle_ThrottlePowerLimit_1,
	Text_menuAdvConfigGasThrottle_ThrottlePowerLimit_2,
	Text_menuAdvConfigGasThrottle_ThrottlePowerLimit_3,
	Text_menuAdvConfigGasThrottle_ThrottlePowerLimit_4,
	Text_menuAdvConfigGasThrottle_ThrottlePowerLimit_5,
	Text_menuAdvConfigGasThrottle_ThrottleRampUp_1,
	Text_menuAdvConfigGasThrottle_ThrottleRampUp_2,
	Text_menuAdvConfigGasThrottle_ThrottleRampUp_3,
	Text_menuAdvConfigGasThrottle_ThrottleRampUp_4,
	Text_menuAdvConfigGasThrottle_ThrottleRampUp_5,

	Text_menuAdvConfigGasThrottle_numberOfTexts
};
enum Texts_menuSpeedReadingConfig
{
	Text_menuSpeedReadingConfig_metricImperial,
	Text_menuSpeedReadingConfig_perimeter,
	Text_menuSpeedReadingConfig_MotorMagnets,
	Text_menuSpeedReadingConfig_GearRatio,

	Text_menuSpeedReadingConfig_numberOfTexts
};

enum Texts_menuBrakeSensor
{
	Text_menuBrakeSensor_brakeHiLo,


	Text_menuBrakeSensor_numberOfTexts
};
enum Texts_menuReadingTempConfig
{
	Text_menuReadingTempConfig_temperatureUnit,
	Text_menuReadingTempConfig_temp1Type,
	Text_menuReadingTempConfig_temp2Type,
	Text_menuReadingTempConfig_temp1Overheat,
	Text_menuReadingTempConfig_temp2Overheat,

	Text_menuReadingTempConfig_numberOfTexts
};

enum Texts_menuPIDConfig
{
	Text_menuPIDConfig_POWERKP,
	Text_menuPIDConfig_POWERKI,
	Text_menuPIDConfig_POWERKD,
	Text_menuPIDConfig_PLOW,
	Text_menuPIDConfig_ILOW,
	Text_menuPIDConfig_DLOW,
	Text_menuPIDConfig_LOW_Threshold,
	Text_menuPIDConfig_SpeedFactorMin,
	Text_menuPIDConfig_PIDPWMMax,
	Text_menuPIDConfig_SpeedFactorRampUp,
	Text_menuPIDConfig_CruiseControlPowerMin,
	Text_menuPIDConfig_CruiseControlPowerMax,
	Text_menuPIDConfig_CruiseControlPowerRampUp,
	Text_menuPIDConfig_CruiseControlMaxSpeed,


	Text_menuPIDConfig_numberOfTexts

};

enum Texts_menuPasModeConfig
{
	Text_menuPasModeConfig_AutoLegal,
	Text_menuPasModeConfig_LegalLimitOnOff,
	Text_menuPasModeConfig_LegalLimitSpeed,
	Text_menuPasModeConfig_LegalLimitPower,
	Text_menuPasModeConfig_PASMagnets,
	Text_menuPasModeConfig_PowerLimitPAS_1,
	Text_menuPasModeConfig_PowerLimitPAS_2,
	Text_menuPasModeConfig_PowerLimitPAS_3,
	Text_menuPasModeConfig_PowerLimitPAS_4,
	Text_menuPasModeConfig_PowerLimitPAS_5,
	Text_menuPasModeConfig_SpeedLimitPAS_1,
	Text_menuPasModeConfig_SpeedLimitPAS_2,
	Text_menuPasModeConfig_SpeedLimitPAS_3,
	Text_menuPasModeConfig_SpeedLimitPAS_4,
	Text_menuPasModeConfig_SpeedLimitPAS_5,
	Text_menuPasModeConfig_MinSpeedPAS_1,
	Text_menuPasModeConfig_MinSpeedPAS_2,
	Text_menuPasModeConfig_MinSpeedPAS_3,
	Text_menuPasModeConfig_MinSpeedPAS_4,
	Text_menuPasModeConfig_MinSpeedPAS_5,
	Text_menuPasModeConfig_CadenceMinPAS_1,
	Text_menuPasModeConfig_CadenceMinPAS_2,
	Text_menuPasModeConfig_CadenceMinPAS_3,
	Text_menuPasModeConfig_CadenceMinPAS_4,
	Text_menuPasModeConfig_CadenceMinPAS_5,
	Text_menuPasModeConfig_CadenceMaxPAS_1,
	Text_menuPasModeConfig_CadenceMaxPAS_2,
	Text_menuPasModeConfig_CadenceMaxPAS_3,
	Text_menuPasModeConfig_CadenceMaxPAS_4,
	Text_menuPasModeConfig_CadenceMaxPAS_5,
	Text_menuPasModeConfig_RampUpPAS_1,
	Text_menuPasModeConfig_RampUpPAS_2,
	Text_menuPasModeConfig_RampUpPAS_3,
	Text_menuPasModeConfig_RampUpPAS_4,
	Text_menuPasModeConfig_RampUpPAS_5,
	Text_menuPasModeConfig_BoostPowerPAS_1,
	Text_menuPasModeConfig_BoostPowerPAS_2,
	Text_menuPasModeConfig_BoostPowerPAS_3,
	Text_menuPasModeConfig_BoostPowerPAS_4,
	Text_menuPasModeConfig_BoostPowerPAS_5,
	Text_menuPasModeConfig_BoostTimePAS_1,
	Text_menuPasModeConfig_BoostTimePAS_2,
	Text_menuPasModeConfig_BoostTimePAS_3,
	Text_menuPasModeConfig_BoostTimePAS_4,
	Text_menuPasModeConfig_BoostTimePAS_5,
	Text_menuPasModeConfig_BoostSpeedPAS_1,
	Text_menuPasModeConfig_BoostSpeedPAS_2,
	Text_menuPasModeConfig_BoostSpeedPAS_3,
	Text_menuPasModeConfig_BoostSpeedPAS_4,
	Text_menuPasModeConfig_BoostSpeedPAS_5,
	Text_menuPasModeConfig_BoostRampUpPAS,
	Text_menuPasModeConfig_CadenceRefreshTime,
	Text_menuPasModeConfig_EnableTorqueSensor,
	Text_menuPasModeConfig_WeightOnPedalStartup,
	Text_menuPasModeConfig_TorqueSensorADCMin,
	Text_menuPasModeConfig_TorqueSensorADCMax,
	Text_menuPasModeConfig_TorqueSensorkgFMax,
	Text_menuPasModeConfig_numberOfTexts
};
enum Texts_menuPresetsConfig
{
	Text_menuPresetsConfig_distancePreset,
	Text_menuPresetsConfig_battChargeNoPreset,

	Text_menuPresetsConfig_numberOfTexts
};
enum Texts_menuSelectedParam
{
	Text_menuSelectedParam_1,
	Text_menuSelectedParam_2,
	Text_menuSelectedParam_3,
	Text_menuSelectedParam_4,
	Text_menuSelectedParam_5,
	Text_menuSelectedParam_6,
	Text_menuSelectedParam_7,
	Text_menuSelectedParam_8,

	Text_menuSelectedParam_numberOfTexts
};
enum Texts_menuDisplayConfig
{
	Text_menuDisplayConfig_clockHour,
	Text_menuDisplayConfig_clockMinutes,
	Text_menuDisplayConfig_backlightDayBrightness,
	Text_menuDisplayConfig_backlightNightBrightness,
	Text_menuDisplayConfig_autoPowerOff,
	Text_menuDisplayConfig_clockVisibility,
	Text_menuDisplayConfig_temp1Visibility,
	Text_menuDisplayConfig_temp2Visibility,
	Text_menuDisplayConfig_language,
	Text_menuDisplayConfig_buttonsInverted,

	Text_menuDisplayConfig_numberOfTexts
};
enum Texts_statScreen
{
	Texts_statScreen_inMotion,

	Texts_statScreen_distance,
	Texts_statScreen_total,
	Texts_statScreen_daily,
	Texts_statScreen_toGo,

	Texts_statScreen_speed,
	Texts_statScreen_average,
	Texts_statScreen_max,

	Texts_statScreen_battery,
	Texts_statScreen_voltage,
	Texts_statScreen_current,
	Texts_statScreen_maxCurrent,
	Texts_statScreen_power,
	Texts_statScreen_maxPower,

	Texts_statScreen_batteryCd,
	Texts_statScreen_capacity,
	Texts_statScreen_consumption,
	Texts_statScreen_consumptionWh,
	Texts_statScreen_remained,
	Texts_statScreen_numberOfChargeCycles,
	Texts_statScreen_energyConsumption,

	Texts_statScreen_temperature,
	Texts_statScreen_temp1,
	Texts_statScreen_temp2,

	Texts_statScreen_other,
	Texts_statScreen_cadence,
	Texts_statScreen_throttleIn,
	Texts_statScreen_torqueADC,
	Texts_statScreen_torqueKGF,
	Texts_statScreen_softwareVersion,

	Texts_statScreen_numberOfTexts
};
enum Texts_configScreen
{
	Text_configScreen_hold,
	Text_configScreen_reset,

	Text_configScreen_numberOfTexts
};

enum Texts_mainScreen2
{
	Texts_mainScreen2_Speed,
	Texts_mainScreen2_DistanceToGo,
	Texts_mainScreen2_BatteryChargePercentage,
	Texts_mainScreen2_DailyDistance,
	Texts_mainScreen2_CurrentPower,
	Texts_mainScreen2_Temperature1,
	Texts_mainScreen2_AssistMode,
	Texts_mainScreen2_TotalDistance,
	Texts_mainScreen2_AverageSpeed,
	Texts_mainScreen2_MaxSpeed,
	Texts_mainScreen2_TimeInMotion,
	Texts_mainScreen2_BatteryVoltage,
	Texts_mainScreen2_Current,
	Texts_mainScreen2_MaxCurrent,
	Texts_mainScreen2_MaxPower,
	Texts_mainScreen2_EnergyConsumptionPerKilometer,
	Texts_mainScreen2_BatteryCapacity,
	Texts_mainScreen2_CurrentBatteryConsumption,
	Texts_mainScreen2_Temperature2,
	Texts_mainScreen2_NumberOfChargeCycles,
	Texts_mainScreen2_BrakeActivity,
	Texts_mainScreen2_CruiseControlActivity,
	Texts_mainScreen2_SoftwareVersion,
	Texts_mainScreen2_legalIllegal,
	Texts_mainScreen2_WhUsed,
	Texts_mainScreen2_Warning,
	Texts_mainScreen2_Cadence,
	Texts_mainScreen2_ThrottleIn,
	Texts_mainScreen2_torqueADC,
	Texts_mainScreen2_torqueKGF,

	Texts_mainScreen2_numberOfTexts
};

extern char MPe_texts_mainMenu[Text_mainMenu_numberOfTexts][MPE_MAX_TEXT_LENGTH];
extern char MPe_texts_menuBatteryCurrent[Text_menuBatteryCurrent_numberOfTexts][MPE_MAX_TEXT_LENGTH];
extern char MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_numberOfTexts][MPE_MAX_TEXT_LENGTH];
extern char MPe_texts_menuSpeedReadingConfig[Text_menuSpeedReadingConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH];
extern char MPe_texts_menuPIDConfig[Text_menuPIDConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH];
extern char MPe_texts_menuBrakeSensor[Text_menuBrakeSensor_numberOfTexts][MPE_MAX_TEXT_LENGTH];
extern char MPe_texts_menuReadingTempConfig[Text_menuReadingTempConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH];
extern char MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH];
extern char MPe_texts_menuPresetsConfig[Text_menuPresetsConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH];
extern char MPe_texts_menuSelectedParam[Text_menuSelectedParam_numberOfTexts][MPE_MAX_TEXT_LENGTH];
extern char MPe_texts_menuDisplayConfig[Text_menuDisplayConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH];
extern char MPe_texts_statScreen[Texts_statScreen_numberOfTexts][MPE_MAX_TEXT_LENGTH];
extern char MPe_texts_configScreen[Text_configScreen_numberOfTexts][MPE_MAX_TEXT_LENGTH];
extern char MPe_texts_mainScreen2[Texts_mainScreen2_numberOfTexts][MPE_MAX_TEXT_LENGTH];
extern uint16_t text_language_edit;

void texts_init(void);
void texts_setLanguage(Text_languages_t language);
void texts_synchro(void);

#endif /* MPE_TEXTS_H_ */
