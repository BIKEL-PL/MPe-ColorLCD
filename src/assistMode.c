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

#include "assistMode.h"
#include "timers.h"
#include "screen.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MIN_ASSIST_MODE					(0)
#define MAX_ASSIST_MODE					(5)

static BOOL legalMode = TRUE;

char MPe_assistMode_editValue[MPE_MAX_PARAM_LENGTH] = "0";
static volatile uint32_t MPe_assistMode_lastEditingTime;

extern void mainScreen_setLegalAssistMode(void);
extern void mainScreen_setIllegalAssistMode(void);

void assistMode_init(void)
{
}
void assistMode_updateValue(void)
{
	snprintf(MPe_assistMode_editValue, sizeof(MPe_assistMode_editValue), "%d", MPe_data_configParamValue[ConfigParam_assistMode]);
	screen_updateData(Param_AssistMode);
	MPe_assistMode_lastEditingTime = get_time_base_counter_1ms();
}
void assistMode_incrementValue(void)
{
	if (MPe_data_configParamValue[ConfigParam_assistMode] < MAX_ASSIST_MODE)
	{
		MPe_data_configParamValue[ConfigParam_assistMode]++;
		assistMode_updateValue();
	}
}
void assistMode_decrementValue(void)
{
	if (MPe_data_configParamValue[ConfigParam_assistMode] > MIN_ASSIST_MODE)
	{
		MPe_data_configParamValue[ConfigParam_assistMode]--;
		assistMode_updateValue();
	}
}

void assistMode_setLegalMode(BOOL mode)
{
	if (legalMode != mode)
	{
		legalMode = mode;

		// change the colors of the AssistMode on the display
		if (legalMode) mainScreen_setLegalAssistMode();
		else mainScreen_setIllegalAssistMode();

		if (MPe_data_configParamValue[ConfigParam_assistMode] < MIN_ASSIST_MODE)
		{
			MPe_data_configParamValue[ConfigParam_assistMode] = MIN_ASSIST_MODE;
			assistMode_updateValue();
		}
		if (MPe_data_configParamValue[ConfigParam_assistMode] > MAX_ASSIST_MODE)
		{
			MPe_data_configParamValue[ConfigParam_assistMode] = MAX_ASSIST_MODE;
			assistMode_updateValue();
		}
	}
}

void assistMode_changeLegalMode(void)
{
	if(legalMode)
	MPe_data_configParamValue[ConfigParam_LegalLimitOnOff] = 0;
	else
	MPe_data_configParamValue[ConfigParam_LegalLimitOnOff] = 1;
}

uint32_t assistMode_getLastEditingTime(void)
{
	return MPe_assistMode_lastEditingTime;
}
