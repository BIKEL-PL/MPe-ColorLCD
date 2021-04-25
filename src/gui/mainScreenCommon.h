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

#ifndef MAINSCREENCOMMON_H_
#define MAINSCREENCOMMON_H_

#include "ugui.h"

#define MAIN_SCREEN_LABEL_FONT_COLOR	C_SILVER
#define MAIN_SCREEN2_LABEL_FONT_COLOR	C_GAINSBORO

extern UG_TEXTBOX batteryChargePercentageTB, batteryVoltageTB, assistModeTB, temperature1TB, temperature2TB,
					percentTB, voltageTB, clockTB, logoTB;

extern uint16_t mainScreen_clockVisibility, mainScreen_temp1Visibility, mainScreen_temp2Visibility;
extern uint16_t mainScreen_clockVisibility_edit, mainScreen_temp1Visibility_edit, mainScreen_temp2Visibility_edit;

void mainScreenCommon_init(void);
void mainScreenCommon_callback( UG_MESSAGE* msg );
void mainScreenCommon_updateClock(uint8_t hour, uint8_t min);
void mainScreenCommon_synchro(void);
void mainScreenCommon_renderDynamicDrawings(UG_WINDOW* wnd, UG_OBJECT* obj);

#endif /* MAINSCREENCOMMON_H_ */
