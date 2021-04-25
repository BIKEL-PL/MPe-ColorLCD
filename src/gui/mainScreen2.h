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

#ifndef MAINSCREEN2_H_
#define MAINSCREEN2_H_

#include "ugui.h"

typedef enum {
	// Value fields
	batteryChargePercentageField2,
	batteryVoltageField2,
	clockField2,
	valueTB1, valueTB2,
	valueTB3, valueTB4,
	valueTB5, valueTB6,
	valueTB7, valueTB8,

	// Label fields
	percentField2,
	voltageField2,
	logoField2,
	labelTB1, labelTB2,
	labelTB3, labelTB4,
	labelTB5, labelTB6,
	labelTB7, labelTB8,

	// Other fields
	batteryField2,
	staticDrawings2,
	staticLogoDrawings,
	dynamicDrawings2,

	mainScreen2_Fieldcount
} mainScreen2Field_t;

extern UG_WINDOW mainScreen2_window;

void mainScreen2_init(void);
void mainScreen2_update(mainScreen2Field_t field);
void mainScreen2_updateData(uint8_t paramNr);
void mainScreen2_synchro(void);

#endif /* MAINSCREEN2_H_ */
