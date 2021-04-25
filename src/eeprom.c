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

#include "stdio.h"
#include <string.h>
#include "eeprom.h"
#include "eeprom_hw.h"
#include "version.h"

static eeprom_data_t m_eeprom_data;
static bool eeprom_writeFlag = false;

const eeprom_data_t m_eeprom_data_defaults = {
	.versionHigh = VERSION_HIGH,
	.versionLow = VERSION_LOW,
	.lcdPowerOffTimeMinutes = DEFAULT_VALUE_LCD_POWER_OFF_TIME,
	.lcdBacklightDayBrightness = DEFAULT_VALUE_LCD_BACKLIGHT_DAY_BRIGHTNESS,
	.lcdBacklightNightBrightness = DEFAULT_VALUE_LCD_BACKLIGHT_NIGHT_BRIGHTNESS,
	.clockVisibilyty = DEFAULT_VALUE_CLOCK_VISIBILITY,
	.temp1Visibilyty = DEFAULT_VALUE_TEMP1_VISIBILITY,
	.temp2Visibilyty = DEFAULT_VALUE_TEMP2_VISIBILITY,
	.language = DEFAULT_VALUE_LANGUAGE,
	.selectedParam1 = DEFAULT_VALUE_SELECTED_PARAM_1,
	.selectedParam2 = DEFAULT_VALUE_SELECTED_PARAM_2,
	.selectedParam3 = DEFAULT_VALUE_SELECTED_PARAM_3,
	.selectedParam4 = DEFAULT_VALUE_SELECTED_PARAM_4,
	.selectedParam5 = DEFAULT_VALUE_SELECTED_PARAM_5,
	.selectedParam6 = DEFAULT_VALUE_SELECTED_PARAM_6,
	.selectedParam7 = DEFAULT_VALUE_SELECTED_PARAM_7,
	.selectedParam8 = DEFAULT_VALUE_SELECTED_PARAM_8,
	.buttonsInverted = DEFAULT_VALUE_BUTTONS_INVERTED,
};

void eeprom_init() {
	eeprom_hw_init();

	// read the values from EEPROM to array
	memset(&m_eeprom_data, 0, sizeof(m_eeprom_data));

	// if eeprom is blank use defaults
	// if eeprom version is less than the min required version, wipe and use defaults
	// if eeprom version is greater than the current app version, user must have downgraded - wipe and use defaults
	if (!flash_read_words(&m_eeprom_data, sizeof(m_eeprom_data) / sizeof(uint32_t)))
	{
		// If we are using default data it doesn't get written to flash until someone calls write
		memcpy(&m_eeprom_data, &m_eeprom_data_defaults, sizeof(m_eeprom_data_defaults));
	}
	else
	{
		if (m_eeprom_data.versionHigh != m_eeprom_data_defaults.versionHigh
				|| m_eeprom_data.versionLow != m_eeprom_data_defaults.versionLow)
		{
			// f we have a new software number, we restore default settings
			memcpy(&m_eeprom_data, &m_eeprom_data_defaults, sizeof(m_eeprom_data_defaults));
		}
	}
}

uint32_t eeprom_readParam(eepromParam_t paramNr)
{
	uint32_t result = 0;

	switch (paramNr)
	{
	case eepromParam_lcdPowerOffTimeMinutes: result = m_eeprom_data.lcdPowerOffTimeMinutes;
	break;
	case eepromParam_lcdBacklightDayBrightness: result = m_eeprom_data.lcdBacklightDayBrightness;
	break;
	case eepromParam_lcdBacklightNightBrightness: result = m_eeprom_data.lcdBacklightNightBrightness;
	break;
	case eepromParam_clockVisibility: result = m_eeprom_data.clockVisibilyty;
	break;
	case eepromParam_temp1Visibility: result = m_eeprom_data.temp1Visibilyty;
	break;
	case eepromParam_temp2Visibility: result = m_eeprom_data.temp2Visibilyty;
	break;
	case eepromParam_language: result = m_eeprom_data.language;
	break;
	case eepromParam_selectedParam1: result = m_eeprom_data.selectedParam1;
	break;
	case eepromParam_selectedParam2: result = m_eeprom_data.selectedParam2;
	break;
	case eepromParam_selectedParam3: result = m_eeprom_data.selectedParam3;
	break;
	case eepromParam_selectedParam4: result = m_eeprom_data.selectedParam4;
	break;
	case eepromParam_selectedParam5: result = m_eeprom_data.selectedParam5;
	break;
	case eepromParam_selectedParam6: result = m_eeprom_data.selectedParam6;
	break;
	case eepromParam_selectedParam7: result = m_eeprom_data.selectedParam7;
	break;
	case eepromParam_selectedParam8: result = m_eeprom_data.selectedParam8;
	break;
	case eepromParam_buttonsInverted: result = m_eeprom_data.buttonsInverted;
	break;
	default: result = 0;
	}

	return result;
}
void eeprom_saveParam(eepromParam_t paramNr, uint32_t value)
{
	eeprom_writeFlag = true;

	switch (paramNr)
	{
	case eepromParam_lcdPowerOffTimeMinutes: m_eeprom_data.lcdPowerOffTimeMinutes = value;
	break;
	case eepromParam_lcdBacklightDayBrightness: m_eeprom_data.lcdBacklightDayBrightness = value;
	break;
	case eepromParam_lcdBacklightNightBrightness: m_eeprom_data.lcdBacklightNightBrightness = value;
	break;
	case eepromParam_clockVisibility: m_eeprom_data.clockVisibilyty = value;
	break;
	case eepromParam_temp1Visibility: m_eeprom_data.temp1Visibilyty = value;
	break;
	case eepromParam_temp2Visibility: m_eeprom_data.temp2Visibilyty = value;
	break;
	case eepromParam_language: m_eeprom_data.language = value;
	break;
	case eepromParam_selectedParam1: m_eeprom_data.selectedParam1 = value;
	break;
	case eepromParam_selectedParam2: m_eeprom_data.selectedParam2 = value;
	break;
	case eepromParam_selectedParam3: m_eeprom_data.selectedParam3 = value;
	break;
	case eepromParam_selectedParam4: m_eeprom_data.selectedParam4 = value;
	break;
	case eepromParam_selectedParam5: m_eeprom_data.selectedParam5 = value;
	break;
	case eepromParam_selectedParam6: m_eeprom_data.selectedParam6 = value;
	break;
	case eepromParam_selectedParam7: m_eeprom_data.selectedParam7 = value;
	break;
	case eepromParam_selectedParam8: m_eeprom_data.selectedParam8 = value;
	break;
	case eepromParam_buttonsInverted: m_eeprom_data.buttonsInverted = value;
	break;
	default:;
	}
}
void eeprom_write_variables(void)
{
	flash_write_words(&m_eeprom_data, sizeof(m_eeprom_data) / sizeof(uint32_t));
}
void eeprom_synchro(void)
{
	if (eeprom_writeFlag == true)
	{
		eeprom_write_variables();
		eeprom_writeFlag = false;
	}
}

void eeprom_init_defaults(void)
{
	// first force KEY value to 0
	eeprom_write(ADDRESS_KEY, 0);

	// eeprom_init() will read the default values now
	eeprom_init();
}
