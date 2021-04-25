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

#ifdef USE_WITH_560C
#include "stm32f10x.h"
#include "mainScreenCommon.h"
#include "mainScreen.h"
#include "mainScreen2.h"
#include "screen.h"
#include "data.h"
#include "assistMode.h"
#include "battery.h"
#include "eeprom.h"
#include "lcd.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>


static char clockText[MPE_MAX_PARAM_LENGTH] = "0:00";

uint16_t mainScreen_clockVisibility, mainScreen_temp1Visibility, mainScreen_temp2Visibility;
uint16_t mainScreen_clockVisibility_edit, mainScreen_temp1Visibility_edit, mainScreen_temp2Visibility_edit;

// VALUE TEXTBOX
UG_TEXTBOX batteryChargePercentageTB = FIELD_READONLY_STRING(MPe_data_param[Param_BatteryChargePercentage], &CONFIGURATIONS_TEXT_FONT, ALIGN_TOP_RIGHT);
UG_TEXTBOX batteryVoltageTB = FIELD_READONLY_STRING(MPe_data_param[Param_BatteryVoltage], &CONFIGURATIONS_TEXT_FONT, ALIGN_TOP_RIGHT);
UG_TEXTBOX clockTB = FIELD_READONLY_STRING(clockText, &CONFIGURATIONS_TEXT_FONT, ALIGN_TOP_CENTER);
UG_TEXTBOX assistModeTB = FIELD_READONLY_STRING(MPe_assistMode_editValue, &MEDIUM_NUMBERS_TEXT_FONT, ALIGN_TOP_CENTER);
UG_TEXTBOX temperature1TB = FIELD_READONLY_STRING(MPe_data_param[Param_Temperature1], &MEDIUM_NUMBERS_TEXT_FONT, ALIGN_TOP_CENTER);
UG_TEXTBOX temperature2TB = FIELD_READONLY_STRING(MPe_data_param[Param_Temperature2], &MEDIUM_NUMBERS_TEXT_FONT, ALIGN_TOP_CENTER);

// LABEL TEXTBOX
UG_TEXTBOX percentTB = FIELD_READONLY_STRING("%   /", &CONFIGURATIONS_TEXT_FONT, ALIGN_TOP_RIGHT);
UG_TEXTBOX voltageTB = FIELD_READONLY_STRING("V", &CONFIGURATIONS_TEXT_FONT, ALIGN_TOP_RIGHT);
UG_TEXTBOX logoTB = FIELD_READONLY_STRING("MPe", &MEDIUM_TEXT_FONT, ALIGN_BOTTOM_CENTER);

void mainScreenCommon_init(void)
{
	// Eeprom read
	mainScreen_clockVisibility = mainScreen_clockVisibility_edit = (uint16_t)eeprom_readParam(eepromParam_clockVisibility);
	mainScreen_temp1Visibility = mainScreen_temp1Visibility_edit = (uint16_t)eeprom_readParam(eepromParam_temp1Visibility);
	mainScreen_temp2Visibility = mainScreen_temp2Visibility_edit = (uint16_t)eeprom_readParam(eepromParam_temp2Visibility);
}

void mainScreenCommon_callback( UG_MESSAGE* msg )
{
	if (msg->type == MSG_TYPE_OBJECT)
	{
		if (msg->id == OBJ_TYPE_BUTTON)
	    {
			uint8_t isCruiseControlActiv = atoi(MPe_data_param[Param_CruiseControlActivity]);

			if (msg->event == UP_CLICK)
		    {
				if (msg->click == ONE_CLICK)
				{
					if (isCruiseControlActiv) data_cruiseControlSwitchOffRequest();
					else assistMode_incrementValue();
				}
		    }
			else if (msg->event == DOWN_CLICK)
			{
				if (msg->click == ONE_CLICK)
				{
					if (isCruiseControlActiv) data_cruiseControlSwitchOffRequest();
					else assistMode_decrementValue();
				}
			}
			else if (msg->event == ONOFFDOWN_LONG_CLICK)
			{
				assistMode_changeLegalMode();
			}
			else if (msg->event == UP_LONG_CLICK)
			{
				if (isCruiseControlActiv)
				{
					data_cruiseControlAccelerationRequest();
				}
				else
				{
					if (screen_getCurrentScreen() == &mainScreen_window)
					{
						screen_changeWindow(&mainScreen2_window);
					}
					else screen_changeWindow(&mainScreen_window);
				}
			}
			else if (msg->event == DOWN_LONG_CLICK)
			{
				if (isCruiseControlActiv)
				{
					data_cruiseControlSlowDownRequest();
				}
				else data_cruiseControlSwitchOnRequest();
			}
			else if (msg->event == ONOFF_CLICK
					&& msg->click == ONE_CLICK)
			{
				if (isCruiseControlActiv) data_cruiseControlSwitchOffRequest();
				else lcd_toggleDayNightBacklight();
			}
#if RUNNING_PACE_MODE
			else if (msg->event == ONOFFUP_LONG_CLICK
					&& msg->click == ONE_CLICK)
			{
				mainScreen_changeSpeedMode();
			}
#endif
	    }
	}
}
void mainScreenCommon_renderDynamicDrawings(UG_WINDOW* wnd, UG_OBJECT* obj)
{
	if (obj->state & OBJ_STATE_UPDATE)
	{
		if (obj->state & OBJ_STATE_VISIBLE)
		{
			UG_S16 xs, ys, xe, ye, i, size = 5;
			UG_COLOR c = assistModeTB.fc;

			// left vertical line
			xs = xe = 0;
			ys = 80; ye = 295;
			for (i = 0; i < size; i++) UG_DrawLine(xs + i, ys + i, xe + i, ye - i, c);

			// right vertical line
			xs = xe = SCREEN_WIDTH-1 - RIGHT_OFFSET;
			ys = 80; ye = 295;
			for (i = 0; i < size; i++) UG_DrawLine(xs - i, ys + i, xe - i, ye - i, c);
		}
		obj->state &= ~OBJ_STATE_REDRAW;
		obj->state &= ~OBJ_STATE_UPDATE;
	}
}
void mainScreenCommon_updateClock(uint8_t hour, uint8_t min)
{
	//snprintf(clockText, MPE_MAX_PARAM_LENGTH, "%d:%02d", hour, min);
	int hours = atoi(MPe_data_param[Param_TimeInMotion])/60;				//dodane aby zastapic zegar czasem w ruchu
	int minutes = atoi(MPe_data_param[Param_TimeInMotion]) - (hours*60);	//dodane aby zastapic zegar czasem w ruchu
	snprintf(clockText, MPE_MAX_PARAM_LENGTH, "%d:%02d", hours,minutes);	//dodane aby zastapic zegar czasem w ruchu
}
void mainScreenCommon_synchro(void)
{
	if (mainScreen_clockVisibility_edit != mainScreen_clockVisibility)
	{
		mainScreen_clockVisibility = mainScreen_clockVisibility_edit;
		if (!mainScreen_clockVisibility)
		{
			UG_TextboxHide( &mainScreen_window, clockField );
			UG_TextboxHide( &mainScreen2_window, clockField2 );
		}
		else
		{
			UG_TextboxShow( &mainScreen_window, clockField );
			UG_TextboxShow( &mainScreen2_window, clockField2 );
		}
		mainScreen_update(staticDrawings);
		eeprom_saveParam(eepromParam_clockVisibility, mainScreen_clockVisibility);
	}
	if (mainScreen_temp1Visibility_edit != mainScreen_temp1Visibility)
	{
		mainScreen_temp1Visibility = mainScreen_temp1Visibility_edit;
		if (!mainScreen_temp1Visibility)
		{
			UG_TextboxHide( &mainScreen_window, temperature1Field );
			UG_TextboxHide( &mainScreen_window, temp1Field );
		}
		else
		{
			UG_TextboxShow( &mainScreen_window, temperature1Field );
			UG_TextboxShow( &mainScreen_window, temp1Field );
		}
		eeprom_saveParam(eepromParam_temp1Visibility, mainScreen_temp1Visibility);
	}
	if (mainScreen_temp2Visibility_edit != mainScreen_temp2Visibility)
	{
		mainScreen_temp2Visibility = mainScreen_temp2Visibility_edit;
		if (!mainScreen_temp2Visibility)
		{
			UG_TextboxHide( &mainScreen_window, temperature2Field );
			UG_TextboxHide( &mainScreen_window, temp2Field );
		}
		else
		{
			UG_TextboxShow( &mainScreen_window, temperature2Field );
			UG_TextboxShow( &mainScreen_window, temp2Field );
		}
		eeprom_saveParam(eepromParam_temp2Visibility, mainScreen_temp2Visibility);
	}
}
#endif
