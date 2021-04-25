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

#ifndef CONFIGSCREEN_H_
#define CONFIGSCREEN_H_

#include "ugui.h"

typedef enum {
	configScreen_menuList,
	configScreen_batteryList,
	configScreen_gasThrottleList,
	configScreen_speedList,
	configScreen_brakeSensorList,
	configScreen_readingTempList,
	configScreen_PIDreglist,
	configScreen_pasModeList,
	configScreen_presetList,
	configScreen_selectedParamList,
	configScreen_displayList,

	configScreen_holdTB,
	configScreen_resetTB,
	configScreen_currentTB,

	configScreen_fieldCount
} configScreenField_t;

extern UG_WINDOW configScreen_window;

void configScreen_init(void);
void configScreen_update(configScreenField_t field);

#endif /* CONFIGSCREEN_H_ */
