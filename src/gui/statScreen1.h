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

#ifndef STATSCREEN1_H_
#define STATSCREEN1_H_

#include "screen.h"

typedef enum {
	stat1Menu_inMotionField, stat1Menu_inMotionFieldVal, stat1Menu_inMotionFieldUnit,

	stat1Menu_distanceField,
	stat1Menu_totalField, stat1Menu_totalFieldVal, stat1Menu_totalFieldUnit,
	stat1Menu_dailyField, stat1Menu_dailyFieldVal, stat1Menu_dailyFieldUnit,
	stat1Menu_toGoField, stat1Menu_toGoFieldVal, stat1Menu_toGoFieldUnit,

	stat1Menu_speedField,
	stat1Menu_averageField, stat1Menu_averageFieldVal, stat1Menu_averageFieldUnit,
	stat1Menu_maxField, stat1Menu_maxFieldVal, stat1Menu_maxFieldUnit,

	stat1Menu_batteryField,
	stat1Menu_voltageField, stat1Menu_voltageFieldVal, stat1Menu_voltageFieldUnit,
	stat1Menu_currentField, stat1Menu_currentFieldVal, stat1Menu_currentFieldUnit,
	stat1Menu_maxCurrentField, stat1Menu_maxCurrentFieldVal, stat1Menu_maxCurrentFieldUnit,
	stat1Menu_powerField, stat1Menu_powerFieldVal, stat1Menu_powerFieldUnit,
	stat1Menu_maxPowerField, stat1Menu_maxPowerFieldVal, stat1Menu_maxPowerFieldUnit,

	stat1Menu_Fieldcount
} statScreen1Field_t;

extern UG_WINDOW statScreen1_window;

void statScreen1_init(void);
void statScreen1_update(statScreen1Field_t field);
void statScreen1_updateSiImpUnit(void);

#endif /* STATSCREEN1_H_ */
