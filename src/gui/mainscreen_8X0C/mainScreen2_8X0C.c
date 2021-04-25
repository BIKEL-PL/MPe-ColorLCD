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
#include "mainScreen2.h"
#include "mainScreenCommon.h"
#include "screen.h"
#include "data.h"
#include "texts.h"
#include "battery.h"
#include "eeprom.h"
#include <stdio.h>
#include <stdlib.h>

#define MAIN_SCREEN2_MAX_OBJECTS			mainScreen2_Fieldcount
#define MAIN_SCREEN2_FIELDS					8

UG_WINDOW mainScreen2_window;

static UG_OBJECT mainScreen2_objBuff[MAIN_SCREEN2_MAX_OBJECTS];

static uint8_t selectedParam[MAIN_SCREEN2_FIELDS] = {
		DEFAULT_VALUE_SELECTED_PARAM_1, DEFAULT_VALUE_SELECTED_PARAM_2,
		DEFAULT_VALUE_SELECTED_PARAM_3, DEFAULT_VALUE_SELECTED_PARAM_4,
		DEFAULT_VALUE_SELECTED_PARAM_5, DEFAULT_VALUE_SELECTED_PARAM_6,
		DEFAULT_VALUE_SELECTED_PARAM_7, DEFAULT_VALUE_SELECTED_PARAM_8,
};
uint16_t selectedParam_edit[MAIN_SCREEN2_FIELDS];
static const uint8_t selectedParamEeprom[MAIN_SCREEN2_FIELDS] = {
		eepromParam_selectedParam1, eepromParam_selectedParam2,
		eepromParam_selectedParam3, eepromParam_selectedParam4,
		eepromParam_selectedParam5, eepromParam_selectedParam6,
		eepromParam_selectedParam7, eepromParam_selectedParam8,
};
static const uint8_t selectedValueField[MAIN_SCREEN2_FIELDS] = {
		valueTB1, valueTB2,
		valueTB3, valueTB4,
		valueTB5, valueTB6,
		valueTB7, valueTB8,
};
static const uint8_t selectedLabelField[MAIN_SCREEN2_FIELDS] = {
		labelTB1, labelTB2,
		labelTB3, labelTB4,
		labelTB5, labelTB6,
		labelTB7, labelTB8,
};

// VALUE TEXTBOX
static UG_TEXTBOX valueTB[MAIN_SCREEN2_FIELDS] = {
		FIELD_READONLY_STRING(MPe_data_param[DEFAULT_VALUE_SELECTED_PARAM_1], &MEDIUM_NUMBERS_TEXT_FONT, ALIGN_TOP_CENTER),
		FIELD_READONLY_STRING(MPe_data_param[DEFAULT_VALUE_SELECTED_PARAM_2], &MEDIUM_NUMBERS_TEXT_FONT, ALIGN_TOP_CENTER),
		FIELD_READONLY_STRING(MPe_data_param[DEFAULT_VALUE_SELECTED_PARAM_3], &MEDIUM_NUMBERS_TEXT_FONT, ALIGN_TOP_CENTER),
		FIELD_READONLY_STRING(MPe_data_param[DEFAULT_VALUE_SELECTED_PARAM_4], &MEDIUM_NUMBERS_TEXT_FONT, ALIGN_TOP_CENTER),
		FIELD_READONLY_STRING(MPe_data_param[DEFAULT_VALUE_SELECTED_PARAM_5], &MEDIUM_NUMBERS_TEXT_FONT, ALIGN_TOP_CENTER),
		FIELD_READONLY_STRING(MPe_data_param[DEFAULT_VALUE_SELECTED_PARAM_6], &MEDIUM_NUMBERS_TEXT_FONT, ALIGN_TOP_CENTER),
		FIELD_READONLY_STRING(MPe_data_param[DEFAULT_VALUE_SELECTED_PARAM_7], &MEDIUM_NUMBERS_TEXT_FONT, ALIGN_TOP_CENTER),
		FIELD_READONLY_STRING(MPe_data_param[DEFAULT_VALUE_SELECTED_PARAM_8], &MEDIUM_NUMBERS_TEXT_FONT, ALIGN_TOP_CENTER),
};
// LABEL TEXTBOX
static UG_TEXTBOX labelTB[MAIN_SCREEN2_FIELDS] = {
		FIELD_READONLY_STRING(MPe_texts_mainScreen2[DEFAULT_VALUE_SELECTED_PARAM_1], &SMALL_TEXT_FONT, ALIGN_TOP_CENTER),
		FIELD_READONLY_STRING(MPe_texts_mainScreen2[DEFAULT_VALUE_SELECTED_PARAM_2], &SMALL_TEXT_FONT, ALIGN_TOP_CENTER),
		FIELD_READONLY_STRING(MPe_texts_mainScreen2[DEFAULT_VALUE_SELECTED_PARAM_3], &SMALL_TEXT_FONT, ALIGN_TOP_CENTER),
		FIELD_READONLY_STRING(MPe_texts_mainScreen2[DEFAULT_VALUE_SELECTED_PARAM_4], &SMALL_TEXT_FONT, ALIGN_TOP_CENTER),
		FIELD_READONLY_STRING(MPe_texts_mainScreen2[DEFAULT_VALUE_SELECTED_PARAM_5], &SMALL_TEXT_FONT, ALIGN_TOP_CENTER),
		FIELD_READONLY_STRING(MPe_texts_mainScreen2[DEFAULT_VALUE_SELECTED_PARAM_6], &SMALL_TEXT_FONT, ALIGN_TOP_CENTER),
		FIELD_READONLY_STRING(MPe_texts_mainScreen2[DEFAULT_VALUE_SELECTED_PARAM_7], &SMALL_TEXT_FONT, ALIGN_TOP_CENTER),
		FIELD_READONLY_STRING(MPe_texts_mainScreen2[DEFAULT_VALUE_SELECTED_PARAM_8], &SMALL_TEXT_FONT, ALIGN_TOP_CENTER),
};

void mainScreen2_renderStaticDrawings(UG_WINDOW* wnd, UG_OBJECT* obj);
void mainScreen2_renderStaticLogoDrawings(UG_WINDOW* wnd, UG_OBJECT* obj);

void mainScreen2_init(void)
{
	UG_S16 v_xs, v_ys, v_xe, v_ye, l_xs, l_ys, l_xe, l_ye;
	uint32_t value;

	// Eeprom init
	for (uint8_t i = 0; i < MAIN_SCREEN2_FIELDS; i++)
	{
		value = eeprom_readParam(selectedParamEeprom[i]);
		if (value < Param_NumberOfParam) selectedParam_edit[i] = selectedParam[i] = (uint8_t)value;
	}

	// Create Window
	UG_WindowCreate( &mainScreen2_window, mainScreen2_objBuff, MAIN_SCREEN2_MAX_OBJECTS, mainScreenCommon_callback );
	UG_WindowSetBackColor( &mainScreen2_window, C_BLACK);
	UG_WindowSetForeColor( &mainScreen2_window, C_WHITE);
	UG_WindowSetStyle(&mainScreen2_window, WND_STYLE_2D);

	// Create Textboxes
	UG_TextboxCreate( &mainScreen2_window, &batteryChargePercentageTB, batteryChargePercentageField2, 74, 5, 104, 25 );
	UG_TextboxCreate( &mainScreen2_window, &percentTB, percentField2, 109, 5, 140, 25 );

	UG_TextboxCreate( &mainScreen2_window, &batteryVoltageTB, batteryVoltageField2, 150, 5, 184, 25 );
	UG_TextboxCreate( &mainScreen2_window, &voltageTB, voltageField2, 188, 5, 198, 25 );

	UG_TextboxCreate( &mainScreen2_window, &clockTB, clockField2, 247, 5, 318, 25 );
	if (!mainScreen_clockVisibility) UG_TextboxHide( &mainScreen2_window, clockField2 );

	l_ys = 54; l_ye = 69; v_ys = 86; v_ye = 126;
	for (uint8_t i = 0, j = 0; i < MAIN_SCREEN2_FIELDS; i++)
	{
		if (!j)
		{
			// left side
			l_xs = v_xs = 9; l_xe = v_xe = 155;
		}
		else
		{
			// right side
			l_xs = v_xs = 166; l_xe = v_xe = 310;
		}

		UG_TextboxCreate( &mainScreen2_window, &labelTB[i], selectedLabelField[i], l_xs, l_ys, l_xe, l_ye );
		UG_TextboxSetText( &mainScreen2_window, selectedLabelField[i], MPe_texts_mainScreen2[selectedParam[i]]);
		UG_TextboxSetForeColor(&mainScreen2_window, selectedLabelField[i], MAIN_SCREEN2_LABEL_FONT_COLOR);
		UG_TextboxCreate( &mainScreen2_window, &valueTB[i], selectedValueField[i], v_xs, v_ys, v_xe, v_ye );
		UG_TextboxSetText( &mainScreen2_window, selectedValueField[i], MPe_data_param[selectedParam[i]]);

		if (j == 0) j = 1;
		else
		{
			j = 0;
			l_ys += 111; l_ye += 111; v_ys += 111; v_ye += 111;
		}
	}

	UG_TextboxCreate( &mainScreen2_window, &logoTB, logoField2, 130, 448, 190, 478 );
	UG_TextboxSetForeColor(&mainScreen2_window, logoField2, C_BLACK);
	UG_TextboxSetBackColor(&mainScreen2_window, logoField2, C_WHITE);

	// Create Other fields
	UG_FieldCreate(&mainScreen2_window, renderBattery, batteryField2, 0, 0, 0, 0);
	UG_FieldCreate(&mainScreen2_window, mainScreen2_renderStaticDrawings, staticDrawings2, 0, 0, 0, 0);
	UG_FieldCreate(&mainScreen2_window, mainScreen2_renderStaticLogoDrawings, staticLogoDrawings, 0, 0, 0, 0);
	UG_FieldCreate(&mainScreen2_window, mainScreenCommon_renderDynamicDrawings, dynamicDrawings2, 0, 0, 0, 0);
}

void mainScreen2_update(mainScreen2Field_t field)
{
	for (uint8_t i = 0; i < MAIN_SCREEN2_MAX_OBJECTS; i++)
	{
		if (mainScreen2_objBuff[i].id == field)
		{
			mainScreen2_objBuff[i].state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;
			return;
		}
	}
}

void mainScreen2_updateData(uint8_t paramNr)
{
	for (uint8_t i = 0; i < MAIN_SCREEN2_FIELDS; i++)
	{
		if (paramNr == selectedParam[i])
		{
			mainScreen2_update(selectedValueField[i]);
			if (i == 6 || i == 7)
			{
				// refresh logo
				mainScreen2_update(staticLogoDrawings);
				mainScreen2_update(logoField2);
			}
			break;
		}
	}
}

void mainScreen2_renderStaticDrawings(UG_WINDOW* wnd, UG_OBJECT* obj)
{
	if (obj->state & OBJ_STATE_UPDATE)
	{
		if (obj->state & OBJ_STATE_VISIBLE)
		{
			// Full redraw necessary?
			if (obj->state & OBJ_STATE_REDRAW)
			{
				UG_S16 xs, ys, xe, ye, i, size = 3;
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

				// vertical half-split line
				xs = xe = 159;
				ys = 37; ye = 444;
				for (i = 0; i < size; i++) UG_DrawLine(xs + i, ys, xe + i, ye, c);

				// horizontal left-split lines
				xs = 9; xe = 155;
				ys = ye = 143;
				for (i = 0; i < size; i++) UG_DrawLine(xs, ys + i, xe, ye + i, c);

				ys = ye = 255;
				for (i = 0; i < size; i++) UG_DrawLine(xs, ys + i, xe, ye + i, c);

				ys = ye = 367;
				for (i = 0; i < size; i++) UG_DrawLine(xs, ys + i, xe, ye + i, c);

				// horizontal right-split lines
				xs = 166; xe = 310;
				ys = ye = 143;
				for (i = 0; i < size; i++) UG_DrawLine(xs, ys + i, xe, ye + i, c);

				ys = ye = 255;
				for (i = 0; i < size; i++) UG_DrawLine(xs, ys + i, xe, ye + i, c);

				ys = ye = 367;
				for (i = 0; i < size; i++) UG_DrawLine(xs, ys + i, xe, ye + i, c);

				obj->state &= ~OBJ_STATE_REDRAW;
			}
		}
		obj->state &= ~OBJ_STATE_UPDATE;
	}
}

void mainScreen2_renderStaticLogoDrawings(UG_WINDOW* wnd, UG_OBJECT* obj)
{
	if (obj->state & OBJ_STATE_UPDATE)
	{
		if (obj->state & OBJ_STATE_VISIBLE)
		{
			// Full redraw necessary?
			if (obj->state & OBJ_STATE_REDRAW)
			{
				UG_S16 xs, ys, xe, ye, i, size = 3;
				UG_COLOR c = MAIN_SCREEN_LABEL_FONT_COLOR;

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

void mainScreen2_synchro(void)
{
	for (uint8_t i = 0; i < MAIN_SCREEN2_FIELDS; i++)
	{
		if (selectedParam[i] != selectedParam_edit[i])
		{
			if (selectedParam_edit[i] < Param_NumberOfParam)
			{
				selectedParam[i] = selectedParam_edit[i];
				UG_TextboxSetText( &mainScreen2_window, selectedLabelField[i], MPe_texts_mainScreen2[selectedParam[i]]);
				UG_TextboxSetText( &mainScreen2_window, selectedValueField[i], MPe_data_param[selectedParam[i]]);
				eeprom_saveParam(selectedParamEeprom[i], selectedParam[i]);
			}
			else selectedParam_edit[i] = selectedParam[i];
		}
	}
}
#endif
