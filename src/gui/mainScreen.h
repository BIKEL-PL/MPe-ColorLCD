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

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include "ugui.h"

typedef enum {
	// Value fields
	batteryChargePercentageField,
	batteryVoltageField,
	clockField,
	dailyDistanceField,
	distanceToGoField,
	currentPowerField,
	temperature1Field,
	temperature2Field,
	assistModeField,
	speedIntegerField,
	speedPrecisionField,
	speedOver100Field,
	paceIntegerField,
	pacePrecisionField,

	// Label fields
	percentField,
	voltageField,
	tripField,
	rangeField,
	speedUnitField,
	paceUnitField,
	wField,
	temp1Field,
	temp2Field,
	logoField,

	// IMG fields
	brakeImgField,
	cruisecontrolImgField,
	warningImgField,

	// Other fields
	batteryField,
	assistModeFrameField,
	staticDrawings,
	dynamicDrawings,
	speedCleaning,

	mainScreen_Fieldcount
} mainScreenField_t;

extern UG_WINDOW mainScreen_window;

void mainScreen_init(void);
void mainScreen_update(mainScreenField_t field);
void mainScreen_updateSpeed(void);
void mainScreen_updatePace(void);
void mainScreen_updateAssistMode(void);
void mainScreen_updateImg(mainScreenField_t imgId, bool show);
void mainScreen_updateClock(uint8_t hour, uint8_t min);
void mainScreen_updateSiImpUnit(void);
void mainScreen_changeSpeedMode(void);
void mainScreen_synchro(void);

#endif /* MAINSCREEN_H_ */
