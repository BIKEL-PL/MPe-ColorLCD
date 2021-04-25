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
#include "stm32f10x_gpio.h"
#include "stdio.h"
#include "pins.h"
#include "lcd.h"
#include "ugui_driver/ugui_bafang_lcd.h"
#include "ugui.h"
#include "eeprom.h"
#include "main.h"
#include "data.h"
#include <stdlib.h>

typedef enum {
	lcd_backlight_day, lcd_backlight_night
} lcd_backlight_t;

static uint16_t lcd_backlightDayBrightness, lcd_backlightNightBrightness, lcd_powerOffTimeMinutes;
uint16_t lcd_backlightDayBrightness_edit, lcd_backlightNightBrightness_edit, lcd_powerOffTimeMinutes_edit;
static lcd_backlight_t lcd_backlight;

static uint32_t lcd_userIdleTimer = 0;
static BOOL lcd_userActivity = FALSE;

void lcd_set_backlight_intensity(uint8_t ui8_intensity);

/* Place your initialization/startup code here (e.g. MyInst_Start()) */
void lcd_init(void)
{
#ifdef USE_WITH_8X0C
	bafang_8X0C_lcd_init();
#endif

#ifdef USE_WITH_560C
	bafang_560C_lcd_init();
#endif
	UG_FillScreen(C_BLACK);

	lcd_backlightDayBrightness_edit = lcd_backlightDayBrightness = (uint16_t)eeprom_readParam(eepromParam_lcdBacklightDayBrightness);
	lcd_backlightNightBrightness_edit = lcd_backlightNightBrightness = (uint16_t)eeprom_readParam(eepromParam_lcdBacklightNightBrightness);
	lcd_powerOffTimeMinutes_edit = lcd_powerOffTimeMinutes = (uint16_t)eeprom_readParam(eepromParam_lcdPowerOffTimeMinutes);

	lcd_backlight = lcd_backlight_day;
	lcd_set_backlight_intensity(lcd_backlightDayBrightness);
}

void lcd_set_backlight_intensity(uint8_t ui8_intensity)
{
	ui8_intensity /= 5;

	// force to be min of 5% and max of 100%
	if (ui8_intensity < 1)
	{
		ui8_intensity = 1;
	}
	else if (ui8_intensity > 20)
	{
		ui8_intensity = 20;
	}
#ifdef USE_WITH_8X0C
	TIM_SetCompare2(TIM3, ((uint16_t) ui8_intensity) * 2000);
#endif

#ifdef USE_WITH_560C
	TIM_SetCompare4(TIM1, ((uint16_t) ui8_intensity) * 2000);
#endif
}
void lcd_toggleDayNightBacklight(void)
{
	if (lcd_backlight == lcd_backlight_day)
	{
		lcd_backlight = lcd_backlight_night;
		lcd_set_backlight_intensity(lcd_backlightNightBrightness);
	}
	else
	{
		lcd_backlight = lcd_backlight_day;
		lcd_set_backlight_intensity(lcd_backlightDayBrightness);
	}

	data_lightToggleRequest();
}

void lcd_power_off(void)
{
	// save the variables on EEPROM
	//TODO:eeprom_write_variables();

	// put screen all black and disable backlight
	UG_FillScreen(0);
	lcd_set_backlight_intensity(0);

	// now disable the power to all the system
	system_power(0);

	// block here
	while (1);
}

void lcd_synchro(void)
{
	if (lcd_backlightDayBrightness_edit != lcd_backlightDayBrightness)
	{
		lcd_backlightDayBrightness = lcd_backlightDayBrightness_edit;
		if (lcd_backlight == lcd_backlight_day) lcd_set_backlight_intensity(lcd_backlightDayBrightness);
		eeprom_saveParam(eepromParam_lcdBacklightDayBrightness, lcd_backlightDayBrightness);
	}
	if (lcd_backlightNightBrightness_edit != lcd_backlightNightBrightness)
	{
		lcd_backlightNightBrightness = lcd_backlightNightBrightness_edit;
		if (lcd_backlight == lcd_backlight_night) lcd_set_backlight_intensity(lcd_backlightNightBrightness);
		eeprom_saveParam(eepromParam_lcdBacklightNightBrightness, lcd_backlightNightBrightness);
	}
	if (lcd_powerOffTimeMinutes_edit != lcd_powerOffTimeMinutes)
	{
		lcd_powerOffTimeMinutes = lcd_powerOffTimeMinutes_edit;
		eeprom_saveParam(eepromParam_lcdPowerOffTimeMinutes, lcd_powerOffTimeMinutes);
	}
}
void lcd_automaticPowerOffManagement(void)
{
	if (lcd_userActivity == TRUE)
	{
		lcd_userIdleTimer = 0;
		lcd_userActivity = FALSE;
	}
	else if (atoi(MPe_data_param[Param_Speed]) > 0)
	{
		lcd_userIdleTimer = 0;
	}
	else
	{
		lcd_userIdleTimer += AUTO_POWER_OFF_INTERVAL;
		if ((lcd_userIdleTimer > (lcd_powerOffTimeMinutes * 60000)) && (lcd_userIdleTimer > 60000))
		{
			lcd_power_off();
		}
	}
}
void lcd_setUserActivity(void)
{
	lcd_userActivity = TRUE;
}
