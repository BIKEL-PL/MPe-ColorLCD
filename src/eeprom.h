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

#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "data.h"

typedef enum {
	eepromParam_versionHigh,
	eepromParam_versionLow,
	eepromParam_lcdPowerOffTimeMinutes,
	eepromParam_lcdBacklightDayBrightness,
	eepromParam_lcdBacklightNightBrightness,
	eepromParam_clockVisibility,
	eepromParam_temp1Visibility,
	eepromParam_temp2Visibility,
	eepromParam_language,

	eepromParam_selectedParam1,
	eepromParam_selectedParam2,
	eepromParam_selectedParam3,
	eepromParam_selectedParam4,
	eepromParam_selectedParam5,
	eepromParam_selectedParam6,
	eepromParam_selectedParam7,
	eepromParam_selectedParam8,

	eepromParam_buttonsInverted,

	eepromParam_count
} eepromParam_t;
typedef struct eeprom_data {
	uint32_t versionHigh;
	uint32_t versionLow;
	uint32_t lcdPowerOffTimeMinutes;
	uint32_t lcdBacklightDayBrightness;
	uint32_t lcdBacklightNightBrightness;
	uint32_t clockVisibilyty;
	uint32_t temp1Visibilyty;
	uint32_t temp2Visibilyty;
	uint32_t language;
	uint32_t selectedParam1;
	uint32_t selectedParam2;
	uint32_t selectedParam3;
	uint32_t selectedParam4;
	uint32_t selectedParam5;
	uint32_t selectedParam6;
	uint32_t selectedParam7;
	uint32_t selectedParam8;
	uint32_t buttonsInverted;
} eeprom_data_t;

// *************************************************************************** //
// EEPROM memory variables default values
#define DEFAULT_VALUE_LCD_POWER_OFF_TIME                            15 // 15 minutes, each unit 1 minute
#define DEFAULT_VALUE_LCD_BACKLIGHT_DAY_BRIGHTNESS                  100 // 100 = 100%
#define DEFAULT_VALUE_LCD_BACKLIGHT_NIGHT_BRIGHTNESS                0
#define DEFAULT_VALUE_CLOCK_VISIBILITY								1
#define DEFAULT_VALUE_TEMP1_VISIBILITY								1
#define DEFAULT_VALUE_TEMP2_VISIBILITY								1
#define DEFAULT_VALUE_LANGUAGE										0
#define DEFAULT_VALUE_BUTTONS_INVERTED								0

#define DEFAULT_VALUE_SELECTED_PARAM_1								Param_Speed
#define DEFAULT_VALUE_SELECTED_PARAM_2								Param_BatteryVoltage
#define DEFAULT_VALUE_SELECTED_PARAM_3								Param_CurrentPower
#define DEFAULT_VALUE_SELECTED_PARAM_4								Param_Current
#define DEFAULT_VALUE_SELECTED_PARAM_5								Param_MaxPower
#define DEFAULT_VALUE_SELECTED_PARAM_6								Param_MaxCurrent
#define DEFAULT_VALUE_SELECTED_PARAM_7								Param_Temperature1
#define DEFAULT_VALUE_SELECTED_PARAM_8								Param_EnergyConsumptionPerKilometer
// *************************************************************************** //

void eeprom_init(void);
void eeprom_synchro(void);
uint32_t eeprom_readParam(eepromParam_t paramNr);
void eeprom_saveParam(eepromParam_t paramNr, uint32_t value);
void eeprom_init_defaults(void);

#endif /* _EEPROM_H_ */
