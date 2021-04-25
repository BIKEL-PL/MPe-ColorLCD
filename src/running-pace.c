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

#include <running-pace.h>
#include "data.h"
#include <stdlib.h>
#include <stdbool.h>

#define MAX_RUNNING_PACE_SPEED_SAMPLES	10

static float running_pace;
static float running_speed[MAX_RUNNING_PACE_SPEED_SAMPLES];

extern void mainScreen_updatePace(void);

void running_pace_init(void)
{
	running_pace = 0.0f;
}

float running_getAverageSpeed(uint8_t samples)
{
	float sum = 0;
	if (samples > MAX_RUNNING_PACE_SPEED_SAMPLES) samples = MAX_RUNNING_PACE_SPEED_SAMPLES;
	for (uint8_t i = 0; i < samples; i++)
	{
		sum += running_speed[i];
	}
	return sum / samples;
}

void running_pace_updateSpeed(void)
{
	static uint8_t index = 0;
	static bool isBufferFull = false;
	static float oldPace = 0.0f;
	float avSpeed = 0;

	running_speed[index] = atof(MPe_data_param[Param_Speed]);
	avSpeed = running_getAverageSpeed(isBufferFull ? MAX_RUNNING_PACE_SPEED_SAMPLES : (index + 1));

	if (avSpeed) running_pace = 1 / (avSpeed / 60);
	else running_pace = 100.0;

	if (index < MAX_RUNNING_PACE_SPEED_SAMPLES - 1) index++;
	else
	{
		index = 0;
		isBufferFull = true;
	}

	if (running_pace != oldPace) mainScreen_updatePace();
	oldPace = running_pace;
}

float running_pace_getPace(void)
{
	return running_pace;
}
