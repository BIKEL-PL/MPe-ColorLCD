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

#ifdef USE_WITH_8X0C
#include "stm32f10x.h"
#include "mainScreen.h"
#include "mainScreenCommon.h"
#include "screen.h"
#include "data.h"
#include "assistMode.h"
#include "battery.h"
#include "eeprom.h"
#include <running-pace.h>
#include "img/cruisecontrol.h"
#include "img/brake.h"
#include "img/engine.h"
#include "img/warning.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAIN_SCREEN_MAX_OBJECTS			mainScreen_Fieldcount

#define SPEED_INT_XS_1_POS	10
#define SPEED_INT_XE_1_POS	170
#define SPEED_INT_XS_2_POS	10
#define SPEED_INT_XE_2_POS	210
#define SPEED_PREC_XS_1_POS	180
#define SPEED_PREC_XE_1_POS	310
#define SPEED_PREC_XS_2_POS	220
#define SPEED_PREC_XE_2_POS	310

static enum imgFlag{imgFlagBrake = (1 << 0), imgFlagCruisecontrol = (1 << 1), imgFlagWarning = (1 << 2)};
typedef enum {speedMode_speed, speedMode_pace} speedMode_t;

UG_WINDOW mainScreen_window;
static speedMode_t mainScreen_speedMode = speedMode_speed;

static UG_OBJECT mainScreen_objBuff[MAIN_SCREEN_MAX_OBJECTS];

static char speedIntegerText[MPE_MAX_PARAM_LENGTH] = "0";
static char speedPrecisionText[MPE_MAX_PARAM_LENGTH] = ".0";
static char speedUnitText[MPE_MAX_PARAM_LENGTH] = "km/h";
static uint8_t speedIntCount;

static char paceIntegerText[MPE_MAX_PARAM_LENGTH] = "9.";
static char pacePrecisionText[MPE_MAX_PARAM_LENGTH] = "99";
static char paceUnitText[MPE_MAX_PARAM_LENGTH] = "min/km";

// VALUE TEXTBOX
static UG_TEXTBOX dailyDistanceTB = FIELD_READONLY_STRING(MPe_data_param[Param_DailyDistance], &MEDIUM_NUMBERS_TEXT_FONT, ALIGN_TOP_CENTER);
static UG_TEXTBOX distanceToGoTB = FIELD_READONLY_STRING(MPe_data_param[Param_DistanceToGo], &MEDIUM_NUMBERS_TEXT_FONT, ALIGN_TOP_CENTER);
static UG_TEXTBOX currentPowerTB= FIELD_READONLY_STRING(MPe_data_param[Param_CurrentPower], &BIG_NUMBERS_TEXT_FONT, ALIGN_TOP_CENTER);
static UG_TEXTBOX speedIntegerTB = FIELD_READONLY_STRING(speedIntegerText, &HUGE_NUMBERS_TEXT_FONT, ALIGN_TOP_RIGHT);
static UG_TEXTBOX speedPrecisionTB = FIELD_READONLY_STRING(speedPrecisionText, &BIG_NUMBERS_TEXT_FONT, ALIGN_TOP_LEFT);
static UG_TEXTBOX speedOver100TB = FIELD_READONLY_STRING(speedIntegerText, &HUGE_NUMBERS_TEXT_FONT, ALIGN_TOP_CENTER);
static UG_TEXTBOX paceIntegerTB = FIELD_READONLY_STRING(paceIntegerText, &BIG_NUMBERS_TEXT_FONT, ALIGN_TOP_RIGHT);
static UG_TEXTBOX pacePrecisionTB = FIELD_READONLY_STRING(pacePrecisionText, &HUGE_NUMBERS_TEXT_FONT, ALIGN_TOP_LEFT);

// LABEL TEXTBOX
static UG_TEXTBOX tripTB = FIELD_READONLY_STRING("TRIP", &SMALL_TEXT_FONT, ALIGN_TOP_CENTER);
static UG_TEXTBOX rangeTB = FIELD_READONLY_STRING("RANGE", &SMALL_TEXT_FONT, ALIGN_TOP_CENTER);
static UG_TEXTBOX speedUnitTB = FIELD_READONLY_STRING(speedUnitText, &MEDIUM_TEXT_FONT, ALIGN_TOP_LEFT);
static UG_TEXTBOX wTB = FIELD_READONLY_STRING("W", &MEDIUM_TEXT_FONT, ALIGN_TOP_CENTER);
static UG_TEXTBOX temp1TB = FIELD_READONLY_STRING("TEMP 1", &SMALL_TEXT_FONT, ALIGN_TOP_CENTER);
static UG_TEXTBOX temp2TB = FIELD_READONLY_STRING("TEMP 2", &SMALL_TEXT_FONT, ALIGN_TOP_CENTER);
static UG_TEXTBOX paceUnitTB = FIELD_READONLY_STRING(paceUnitText, &MEDIUM_TEXT_FONT, ALIGN_TOP_LEFT);

// IMG
static UG_IMAGE imageEngine, imageBrake, imageCruisecontrol;

void mainScreen_renderAssistModeFrame(UG_WINDOW* wnd, UG_OBJECT* obj);
void mainScreen_renderStaticDrawings(UG_WINDOW* wnd, UG_OBJECT* obj);
void mainScreen_setTbColour(UG_TEXTBOX* txb, mainScreenField_t field, UG_COLOR newColour);
void mainScreen_renderSpeedCleaning(UG_WINDOW* wnd, UG_OBJECT* obj);

void mainScreen_init(void)
{
	// Eeprom read
	mainScreen_clockVisibility = mainScreen_clockVisibility_edit = (uint16_t)eeprom_readParam(eepromParam_clockVisibility);
	mainScreen_temp1Visibility = mainScreen_temp1Visibility_edit = (uint16_t)eeprom_readParam(eepromParam_temp1Visibility);
	mainScreen_temp2Visibility = mainScreen_temp2Visibility_edit = (uint16_t)eeprom_readParam(eepromParam_temp2Visibility);

	// Create Window
	UG_WindowCreate( &mainScreen_window, mainScreen_objBuff, MAIN_SCREEN_MAX_OBJECTS, mainScreenCommon_callback );
	UG_WindowSetBackColor( &mainScreen_window, C_BLACK);
	UG_WindowSetForeColor( &mainScreen_window, C_WHITE);
	UG_WindowSetStyle(&mainScreen_window, WND_STYLE_2D);

	// Create Textboxes
	UG_TextboxCreate( &mainScreen_window, &batteryChargePercentageTB, batteryChargePercentageField, 74, 5, 104, 25 );
	UG_TextboxCreate( &mainScreen_window, &percentTB, percentField, 109, 5, 140, 25 );

	UG_TextboxCreate( &mainScreen_window, &batteryVoltageTB, batteryVoltageField, 150, 5, 184, 25 );
	UG_TextboxCreate( &mainScreen_window, &voltageTB, voltageField, 188, 5, 198, 25 );

	UG_TextboxCreate( &mainScreen_window, &clockTB, clockField, 247, 5, 318, 25 );
	if (!mainScreen_clockVisibility) UG_TextboxHide( &mainScreen_window, clockField );

	UG_TextboxCreate( &mainScreen_window, &dailyDistanceTB, dailyDistanceField, 5, 43, 120, 83 );
	UG_TextboxCreate( &mainScreen_window, &tripTB, tripField, 5, 88, 120, 103 );
	UG_TextboxSetForeColor(&mainScreen_window, tripField, MAIN_SCREEN_LABEL_FONT_COLOR);

	UG_TextboxCreate( &mainScreen_window, &distanceToGoTB, distanceToGoField, 200, 43, 315, 83 );
	UG_TextboxCreate( &mainScreen_window, &rangeTB, rangeField, 200, 88, 315, 103 );
	UG_TextboxSetForeColor(&mainScreen_window, rangeField, MAIN_SCREEN_LABEL_FONT_COLOR);

	UG_TextboxCreate( &mainScreen_window, &speedOver100TB, speedOver100Field, 10, 114, 310, 264 );
	UG_TextboxHide( &mainScreen_window, speedOver100Field );
	UG_TextboxCreate( &mainScreen_window, &speedIntegerTB, speedIntegerField, SPEED_INT_XS_1_POS, 114, SPEED_INT_XE_1_POS, 264 );
	UG_TextboxCreate( &mainScreen_window, &speedPrecisionTB, speedPrecisionField, SPEED_PREC_XS_1_POS, 189, SPEED_PREC_XE_1_POS, 264 );
	UG_TextboxCreate( &mainScreen_window, &speedUnitTB, speedUnitField, 220, 128, 310, 158 );
	UG_TextboxSetForeColor(&mainScreen_window, speedUnitField, MAIN_SCREEN_LABEL_FONT_COLOR);

	UG_TextboxCreate( &mainScreen_window, &paceIntegerTB, paceIntegerField, 10, 189, 105, 264 );
	UG_TextboxHide( &mainScreen_window, paceIntegerField );
	UG_TextboxCreate( &mainScreen_window, &pacePrecisionTB, pacePrecisionField, 115, 114, 310, 264 );
	UG_TextboxHide( &mainScreen_window, pacePrecisionField );
	UG_TextboxCreate( &mainScreen_window, &paceUnitTB, paceUnitField, 12, 128, 107, 158 );
	UG_TextboxSetForeColor(&mainScreen_window, paceUnitField, MAIN_SCREEN_LABEL_FONT_COLOR);
	UG_TextboxHide( &mainScreen_window, paceUnitField );

	UG_TextboxCreate( &mainScreen_window, &currentPowerTB, currentPowerField, 75, 283, 245, 345 );
	UG_TextboxCreate( &mainScreen_window, &wTB, wField, 251, 297, 281, 327 );
	UG_TextboxSetForeColor(&mainScreen_window, wField, MAIN_SCREEN_LABEL_FONT_COLOR);

	UG_TextboxCreate( &mainScreen_window, &temperature1TB, temperature1Field, 5, 396, 120, 436 );
	UG_TextboxCreate( &mainScreen_window, &temp1TB, temp1Field, 5, 442, 120, 457 );
	UG_TextboxSetForeColor(&mainScreen_window, temp1Field, MAIN_SCREEN_LABEL_FONT_COLOR);
	if (!mainScreen_temp1Visibility)
	{
		UG_TextboxHide( &mainScreen_window, temperature1Field );
		UG_TextboxHide( &mainScreen_window, temp1Field );
	}

	UG_TextboxCreate( &mainScreen_window, &temperature2TB, temperature2Field, 200, 396, 315, 436 );
	UG_TextboxCreate( &mainScreen_window, &temp2TB, temp2Field, 200, 442, 315, 457 );
	UG_TextboxSetForeColor(&mainScreen_window, temp2Field, MAIN_SCREEN_LABEL_FONT_COLOR);
	if (!mainScreen_temp2Visibility)
	{
		UG_TextboxHide( &mainScreen_window, temperature2Field );
		UG_TextboxHide( &mainScreen_window, temp2Field );
	}


	UG_TextboxCreate( &mainScreen_window, &assistModeTB, assistModeField, 140, 373, 180, 435 );

	UG_TextboxCreate( &mainScreen_window, &logoTB, logoField, 130, 448, 190, 478 );
	UG_TextboxSetForeColor(&mainScreen_window, logoField, C_BLACK);
	UG_TextboxSetBackColor(&mainScreen_window, logoField, C_WHITE);

	// Create the images
	UG_ImageCreate ( &mainScreen_window, &imageBrake, brakeImgField, 128, 47, 193, 112 );
	UG_ImageSetBMP( &mainScreen_window, brakeImgField, &brakeBmp );
	UG_ImageHide( &mainScreen_window, brakeImgField );

	UG_ImageCreate ( &mainScreen_window, &imageCruisecontrol, cruisecontrolImgField, 128, 47, 193, 112 );
	UG_ImageSetBMP( &mainScreen_window, cruisecontrolImgField, &cruisecontrolBmp );
	UG_ImageHide( &mainScreen_window, cruisecontrolImgField );

	UG_ImageCreate ( &mainScreen_window, &imageEngine, warningImgField, 128, 47, 193, 112 );
	UG_ImageSetBMP( &mainScreen_window, warningImgField, &warningBmp );
	UG_ImageHide( &mainScreen_window, warningImgField );

	// Create Other fields
	UG_FieldCreate(&mainScreen_window, renderBattery, batteryField, 0, 0, 0, 0);
	UG_FieldCreate(&mainScreen_window, mainScreen_renderAssistModeFrame, assistModeFrameField, 0, 0, 0, 0);
	UG_FieldCreate(&mainScreen_window, mainScreen_renderStaticDrawings, staticDrawings, 0, 0, 0, 0);
	UG_FieldCreate(&mainScreen_window, mainScreenCommon_renderDynamicDrawings, dynamicDrawings, 0, 0, 0, 0);
	UG_FieldCreate(&mainScreen_window, mainScreen_renderSpeedCleaning, speedCleaning, 0, 0, 0, 0);

	// Update coloring - must be at the end!
	mainScreen_updateAssistMode();
}

void mainScreen_update(mainScreenField_t field)
{
	for (uint8_t i = 0; i < MAIN_SCREEN_MAX_OBJECTS; i++)
	{
		if (mainScreen_objBuff[i].id == field)
		{
			mainScreen_objBuff[i].state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;
			return;
		}
	}
}

void mainScreen_renderAssistModeFrame(UG_WINDOW* wnd, UG_OBJECT* obj)
{
	if (obj->state & OBJ_STATE_UPDATE)
	{
		UG_S16 xs = 134, xe = 186, ys = 362, ye = 440, i, size = 5, r = 4;

		if (obj->state & OBJ_STATE_VISIBLE)
		{
			// Full redraw necessary?
			if (obj->state & OBJ_STATE_REDRAW)
			{
				UG_COLOR c = C_WHITE;
				for (i = 0; i < size; i++) UG_DrawRoundFrame(xs + i, ys + i, xe - i, ye - i, r, c);

				obj->state &= ~OBJ_STATE_REDRAW;
			}
		}
		else
		{
			UG_COLOR c = C_BLACK;
			for (i = 0; i < size; i++) UG_DrawRoundFrame(xs + i, ys + i, xe - i, ye - i, r, c);
		}
		obj->state &= ~OBJ_STATE_UPDATE;
	}
}
void mainScreen_renderStaticDrawings(UG_WINDOW* wnd, UG_OBJECT* obj)
{
	if (obj->state & OBJ_STATE_UPDATE)
	{
		if (obj->state & OBJ_STATE_VISIBLE)
		{
			// Full redraw necessary?
			if (obj->state & OBJ_STATE_REDRAW)
			{
				UG_S16 xs, ys, xe, ye, i, size = 3, r = 4;
				UG_COLOR c = MAIN_SCREEN_LABEL_FONT_COLOR;

				// horizontal battery line
				xs = 6; xe = 315;
				ys = ye = 30;
				for (i = 0; i < size; i++) UG_DrawLine(xs, ys + i, xe, ye + i, c);

				// vertical battery line
				if (mainScreen_clockVisibility)
				{
					xs = xe = 244;
					ys = 4; ye = 26;
					for (i = 0; i < size; i++) UG_DrawLine(xs + i, ys, xe + i, ye, c);
				}

				// power frame
				xs = 69; xe = 251;
				ys = 272; ye = 350;
				size = 5;
				for (i = 0; i < size; i++) UG_DrawRoundFrame(xs + i, ys + i, xe - i, ye - i, r, c);

				// logo image
				size = 9; c = C_WHITE;
				xs = xe = 191;
				ys = 451; ye = 478;
				for (i = 0; i < size; i++) UG_DrawLine(xs + i, ys + (3*i), xe + i, ye, c);
				xs = xe = 129;
				for (i = 0; i < size; i++) UG_DrawLine(xs - i, ys + (3*i), xe - i, ye, c);
				xs = 120; xe = 200;
				ys = ye = 479;
				UG_DrawLine(xs, ys, xe, ye, c);

				obj->state &= ~OBJ_STATE_REDRAW;
			}
		}
		obj->state &= ~OBJ_STATE_UPDATE;
	}
}

void mainScreen_setTbColour(UG_TEXTBOX* txb, mainScreenField_t field, UG_COLOR newColour)
{
	for (uint8_t i = 0; i < MAIN_SCREEN_MAX_OBJECTS; i++)
	{
		if (mainScreen_objBuff[i].id == field)
		{
			if (txb) txb->fc = newColour;
			mainScreen_objBuff[i].state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;
			return;
		}
	}
}
void mainScreen_setLegalAssistMode(void)
{
	UG_FieldShow(&mainScreen_window, assistModeFrameField);
}
void mainScreen_setIllegalAssistMode(void)
{
	UG_FieldHide(&mainScreen_window, assistModeFrameField);
}
void mainScreen_updateAssistMode(void)
{
	uint16_t value = MPe_data_configParamValue[ConfigParam_assistMode];
	UG_COLOR c = C_WHITE;
	switch (value)
	{
	case 0: c = C_ORANGE;
		break;
	case 1: c = C_YELLOW;
		break;
	case 2: c = C_LIME_GREEN;
		break;
	case 3: c = C_DEEP_SKY_BLUE;
		break;
	case 4: c = C_MAGENTA;
		break;
	case 5: c = C_TOMATO;
		break;
	default: c = C_WHITE;
		break;
	}
	mainScreen_setTbColour(&assistModeTB, assistModeField, c);	// update AssistModeTB color
}
void mainScreen_updateImg(mainScreenField_t imgId, bool show)
{
	static uint8_t displayedImgFlag = 0;
	uint8_t flag = 0;

	switch (imgId)
	{
	case brakeImgField: flag = imgFlagBrake;
	break;
	case cruisecontrolImgField: flag = imgFlagCruisecontrol;
	break;
	case warningImgField: flag = imgFlagWarning;
	break;
	default: flag = 0;
	}

	if (show) displayedImgFlag |= flag;
	else displayedImgFlag &= ~flag;

	if (displayedImgFlag & imgFlagBrake)
	{
		UG_ImageHide( &mainScreen_window, warningImgField );
		UG_ImageHide( &mainScreen_window, cruisecontrolImgField );
		UG_ImageShow( &mainScreen_window, brakeImgField );
	}
	else
	{
		UG_ImageHide( &mainScreen_window, brakeImgField );
		if (displayedImgFlag & imgFlagWarning)
		{
			UG_ImageHide( &mainScreen_window, cruisecontrolImgField );
			UG_ImageShow( &mainScreen_window, warningImgField );
		}
		else
		{
			UG_ImageHide( &mainScreen_window, warningImgField );
			if (displayedImgFlag & imgFlagCruisecontrol)
			{
				UG_ImageShow( &mainScreen_window, cruisecontrolImgField );
			}
			else UG_ImageHide( &mainScreen_window, cruisecontrolImgField );
		}
	}
}
void mainScreen_renderSpeedCleaning(UG_WINDOW* wnd, UG_OBJECT* obj)
{
	if (obj->state & OBJ_STATE_UPDATE)
	{
		if (obj->state & OBJ_STATE_VISIBLE)
		{
			UG_S16 xs1 = 0, ys1 = 0, xe1 = 0, ye1 = 0,
					xs2 = 0, ys2 = 0, xe2 = 0, ye2 = 0,
					xs3 = 0, ys3 = 0, xe3 = 0, ye3 = 0;

			if (!UG_TextboxGetPosition(wnd, speedIntegerField, &xs1, &xe1, &ys1, &ye1))
			{
				if (!UG_TextboxGetPosition(wnd, speedPrecisionField, &xs2, &xe2, &ys2, &ye2))
				{
					if (!UG_TextboxGetPosition(wnd, speedUnitField, &xs3, &xe3, &ys3, &ye3))
					{
						// here we cat rectangle between two fields
						UG_FillFrame(xe1, ys1, xs2, ye1, C_BLACK);
						// here we cat rectangle above second field
						UG_FillFrame(xs2, ys1, xs3, ys2, C_BLACK);
					}
				}
			}
		}
		obj->state &= ~OBJ_STATE_REDRAW;
		obj->state &= ~OBJ_STATE_UPDATE;
	}
}

void mainScreen_updatePace(void)
{
	static float lastPace = 0.0f;
	float pace = running_pace_getPace();
	int integer = (int)pace;
	int prec = (int)roundf(pace * 100) - (integer * 100);
	int seconds = (prec * 60) / 100;

	if (seconds >= 60)
	{
		seconds = 0;
		integer++;
	}

	if (pace != lastPace)
	{
		for (uint8_t i = 0; i < MPE_MAX_PARAM_LENGTH; i++)
		{
			paceIntegerText[i] = pacePrecisionText[i] = 0;
		}

		if (integer > 9)
		{
			strcpy(paceIntegerText, "9.");
			strcpy(pacePrecisionText, "99");
		}
		else
		{
			snprintf(paceIntegerText, MPE_MAX_PARAM_LENGTH, "%d.", integer);
			snprintf(pacePrecisionText, MPE_MAX_PARAM_LENGTH, "%02d", seconds);
		}

		if (mainScreen_speedMode == speedMode_pace)
		{
			mainScreen_update(paceIntegerField);
			mainScreen_update(pacePrecisionField);
		}

		lastPace = pace;
	}
}
void mainScreen_updateSpeed(void)
{
	uint8_t i = 0, intCount = 0, precCount = 0, separatorFound = 0;
	static uint8_t intCountLast = 3;

	for (i = 0; i < MPE_MAX_PARAM_LENGTH; i++)
	{
		speedIntegerText[i] = speedPrecisionText[i] = 0;
	}

	for (i = 0; i < MPE_MAX_PARAM_LENGTH; i++)
	{
		if (MPe_data_param[Param_Speed][i])
		{
			if (MPe_data_param[Param_Speed][i] != '.')
			{
				if (separatorFound)
				{
					if (precCount < 2) speedPrecisionText[precCount++] = MPe_data_param[Param_Speed][i];
				}
				else speedIntegerText[intCount++] = MPe_data_param[Param_Speed][i];
			}
			else
			{
				if (separatorFound) break;
				separatorFound = 1;
				speedPrecisionText[precCount++] = MPe_data_param[Param_Speed][i];
			}
		}
	}
	speedIntCount = intCount;

	if (mainScreen_speedMode == speedMode_speed)
	{
		if (intCount > 2)
		{
			if (intCountLast <= 2)
			{
				// zastapilo przejscie z V < 100 na V > 100
				UG_TextboxShow(&mainScreen_window, speedOver100Field);
				UG_TextboxHide(&mainScreen_window, speedIntegerField);
				UG_TextboxHide(&mainScreen_window, speedPrecisionField);
				UG_TextboxHide(&mainScreen_window, speedUnitField);
			}
			mainScreen_update(speedOver100Field);
		}
		else
		{
			if (intCountLast > 2)
			{
				// zastapilo przejscie z V > 100 na V < 100
				UG_TextboxHide(&mainScreen_window, speedOver100Field);
				UG_TextboxShow(&mainScreen_window, speedIntegerField);
				UG_TextboxShow(&mainScreen_window, speedPrecisionField);
				UG_TextboxShow(&mainScreen_window, speedUnitField);
			}
			mainScreen_update(speedIntegerField);
			mainScreen_update(speedPrecisionField);
		}

		// update X position
		if (intCountLast != intCount)
		{
			if (intCount == 1)
			{
				mainScreen_update(speedCleaning);	// need to clear field
				UG_TextboxSetXPosition(&mainScreen_window, speedIntegerField, SPEED_INT_XS_1_POS, SPEED_INT_XE_1_POS);
				UG_TextboxSetXPosition(&mainScreen_window, speedPrecisionField, SPEED_PREC_XS_1_POS, SPEED_PREC_XE_1_POS);
			}
			else
			{
				if (intCount == 2) mainScreen_update(speedCleaning);	// need to clear field
				UG_TextboxSetXPosition(&mainScreen_window, speedIntegerField, SPEED_INT_XS_2_POS, SPEED_INT_XE_2_POS);
				UG_TextboxSetXPosition(&mainScreen_window, speedPrecisionField, SPEED_PREC_XS_2_POS, SPEED_PREC_XE_2_POS);
			}
		}
		intCountLast = intCount;
	}
}
void mainScreen_updateSiImpUnit(void)
{
	snprintf(speedUnitText, MPE_MAX_PARAM_LENGTH, MPe_data_configParamValue[ConfigParam_metricImperial] ? "mph" : "km/h");
}
void mainScreen_changeSpeedMode(void)
{
	if (mainScreen_speedMode == speedMode_speed)
	{
		mainScreen_speedMode = speedMode_pace;
		UG_TextboxHide( &mainScreen_window, speedOver100Field );
		UG_TextboxHide( &mainScreen_window, speedIntegerField );
		UG_TextboxHide( &mainScreen_window, speedPrecisionField );
		UG_TextboxHide( &mainScreen_window, speedUnitField );
		UG_FieldHide( &mainScreen_window, speedCleaning );

		UG_TextboxShow( &mainScreen_window, paceIntegerField );
		UG_TextboxShow( &mainScreen_window, pacePrecisionField );
		UG_TextboxShow( &mainScreen_window, paceUnitField );
	}
	else
	{
		mainScreen_speedMode = speedMode_speed;
		UG_TextboxHide( &mainScreen_window, paceIntegerField );
		UG_TextboxHide( &mainScreen_window, pacePrecisionField );
		UG_TextboxHide( &mainScreen_window, paceUnitField );

		UG_TextboxShow( &mainScreen_window, speedIntegerField );
		UG_TextboxShow( &mainScreen_window, speedPrecisionField );
		UG_TextboxShow( &mainScreen_window, speedUnitField );
		UG_FieldShow( &mainScreen_window, speedCleaning );
		mainScreen_updateSpeed();
	}
}
#endif
