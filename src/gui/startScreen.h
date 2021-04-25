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

#ifndef STARTSCREEN_H_
#define STARTSCREEN_H_

#include "ugui.h"

typedef enum {
	logoMPeField,
	wwwField,
	sloganField,
	versionField,

	startImageField,

	startScreen_Fieldcount
} startScreenField_t;

extern UG_WINDOW startScreen_window;

void startScreen_init(void);
void startScreen_update(startScreenField_t field);

#endif /* STARTSCREEN_H_ */
