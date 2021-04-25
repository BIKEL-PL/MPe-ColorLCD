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

#include "stm32f10x.h"
#include "statScreen1.h"
#include "statScreen2.h"
#include "texts.h"
#include "data.h"
#include <stdio.h>
#include "ugui_config.h"

#define STAT1_SCREEN_MAX_OBJECTS		stat1Menu_Fieldcount

#define STAT_SCREEN_FONT			CONFIGURATIONS_TEXT_FONT


#define STAT_SCREEN_TXT_ALIGN		ALIGN_TOP_RIGHT
#define STAT_SCREEN_DTA_ALIGN		ALIGN_TOP_CENTER
#define STAT_SCREEN_UNIT_ALIGN		ALIGN_TOP_LEFT

#define STAT_SCREEN_X_OFFSET		2
#define STAT_SCREEN_X_TXT_START		(STAT_SCREEN_X_OFFSET)
#define STAT_SCREEN_X_VAL_START		(SCREEN_WIDTH * 0.44 + STAT_SCREEN_X_OFFSET)
#define STAT_SCREEN_X_UNT_START		(SCREEN_WIDTH * 0.78 + STAT_SCREEN_X_OFFSET)

#define STAT_SCREEN_X_TXT_END		(STAT_SCREEN_X_VAL_START - STAT_SCREEN_X_OFFSET)
#define STAT_SCREEN_X_VAL_END		(STAT_SCREEN_X_UNT_START - STAT_SCREEN_X_OFFSET)
#define STAT_SCREEN_X_UNT_END		(SCREEN_WIDTH - STAT_SCREEN_X_OFFSET)

#ifdef USE_WITH_8X0C
#define STAT_SCREEN_Y_INMOTION_OFFSET	SCREEN_HEIGHT * 0.01
#endif

#ifdef USE_WITH_560C
#define STAT_SCREEN_Y_INMOTION_OFFSET	SCREEN_HEIGHT * 0.05
#endif

#define STAT_SCREEN_Y_DISTANCE_OFFSET	SCREEN_HEIGHT * 0.15
#define STAT_SCREEN_Y_SPEED_OFFSET		SCREEN_HEIGHT * 0.42
#define STAT_SCREEN_Y_BATTERY_OFFSET	SCREEN_HEIGHT * 0.63

UG_WINDOW statScreen1_window;

static UG_OBJECT statScreen1_objBuff[STAT1_SCREEN_MAX_OBJECTS];
static char speedUnitText[MPE_MAX_PARAM_LENGTH] = "km/h";
static char distanceUnitText[MPE_MAX_PARAM_LENGTH] = "km";

// Texts



static UG_TEXTBOX inMotionTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_inMotion], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);

static UG_TEXTBOX distanceTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_distance], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX totalTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_total], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX dailyTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_daily], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX toGoTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_toGo], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);

static UG_TEXTBOX speedTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_speed], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX averageTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_average], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX maxTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_max], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);

static UG_TEXTBOX batteryTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_battery], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX voltageTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_voltage], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX currentTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_current], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX maxCurrentTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_maxCurrent], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX powerStatTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_power], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX maxPowerStatTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_maxPower], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);

// Data
static UG_TEXTBOX timeInMotionVB = FIELD_READONLY_STRING(MPe_data_param[Param_TimeInMotion], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);

static UG_TEXTBOX totalDistanceVB = FIELD_READONLY_STRING(MPe_data_param[Param_TotalDistance], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);
static UG_TEXTBOX dailyDistanceStatVB = FIELD_READONLY_STRING(MPe_data_param[Param_DailyDistance], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);
static UG_TEXTBOX distanceToGoStatVB = FIELD_READONLY_STRING(MPe_data_param[Param_DistanceToGo], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);

static UG_TEXTBOX averageSpeedVB = FIELD_READONLY_STRING(MPe_data_param[Param_AverageSpeed], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);
static UG_TEXTBOX maxSpeedVB = FIELD_READONLY_STRING(MPe_data_param[Param_MaxSpeed], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);

static UG_TEXTBOX batteryVoltageStatVB = FIELD_READONLY_STRING(MPe_data_param[Param_BatteryVoltage], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);
static UG_TEXTBOX currentVB = FIELD_READONLY_STRING(MPe_data_param[Param_Current], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);
static UG_TEXTBOX maxCurrentVB = FIELD_READONLY_STRING(MPe_data_param[Param_MaxCurrent], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);
static UG_TEXTBOX currentPowerStatVB = FIELD_READONLY_STRING(MPe_data_param[Param_CurrentPower], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);
static UG_TEXTBOX maxPowerStatVB = FIELD_READONLY_STRING(MPe_data_param[Param_MaxPower], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);

// Units
static UG_TEXTBOX timeInMotionU = FIELD_READONLY_STRING("min", &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);

static UG_TEXTBOX totalDistanceU = FIELD_READONLY_STRING(distanceUnitText, &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);
static UG_TEXTBOX dailyDistanceStatU = FIELD_READONLY_STRING(distanceUnitText, &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);
static UG_TEXTBOX distanceToGoStatU = FIELD_READONLY_STRING(distanceUnitText, &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);

static UG_TEXTBOX averageSpeedU = FIELD_READONLY_STRING(speedUnitText, &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);
static UG_TEXTBOX maxSpeedU = FIELD_READONLY_STRING(speedUnitText, &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);

static UG_TEXTBOX batteryVoltageStatU = FIELD_READONLY_STRING("V", &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);
static UG_TEXTBOX currentU = FIELD_READONLY_STRING("A", &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);
static UG_TEXTBOX maxCurrentU = FIELD_READONLY_STRING("A", &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);
static UG_TEXTBOX currentPowerStatU = FIELD_READONLY_STRING("W", &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);
static UG_TEXTBOX maxPowerStatU = FIELD_READONLY_STRING("W", &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);

void statScreen1_callback( UG_MESSAGE* msg )
{
	if (msg->type == MSG_TYPE_OBJECT)
	{
		if (msg->id == OBJ_TYPE_BUTTON)
	    {
			if (msg->event == UP_CLICK || msg->event == DOWN_CLICK)
		    {
				screen_changeWindow(&statScreen2_window);
		    }
			else if (msg->event == ONOFFUP_LONG_CLICK)
		    {
				data_resetBatteryIndicatorRequest();
		    }
			else if (msg->event == ONOFFDOWN_LONG_CLICK)
		    {
				data_resetCurrValuesRequest();
		    }
	    }
	}
}

void statScreen1_init(void)
{
	uint16_t yOffset = STAT_SCREEN_Y_INMOTION_OFFSET;
	const uint8_t textHeight = 20;
	const uint8_t rowOffset = textHeight + 2;

	// Create Window
	UG_WindowCreate( &statScreen1_window, statScreen1_objBuff, STAT1_SCREEN_MAX_OBJECTS, statScreen1_callback );
	UG_WindowSetBackColor( &statScreen1_window, C_BLACK);
	UG_WindowSetForeColor( &statScreen1_window, C_WHITE);
	UG_WindowSetStyle(&statScreen1_window, WND_STYLE_2D);

	// ***************************************** In motion *********************************** //
	UG_TextboxCreate( &statScreen1_window, &inMotionTB, stat1Menu_inMotionField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHeight );
	UG_TextboxSetForeColor(&statScreen1_window, stat1Menu_inMotionField, C_YELLOW);
	UG_TextboxCreate( &statScreen1_window, &timeInMotionVB, stat1Menu_inMotionFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &timeInMotionU, stat1Menu_inMotionFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHeight );

	// ***************************************** Distance *********************************** //
	yOffset = STAT_SCREEN_Y_DISTANCE_OFFSET;

	UG_TextboxCreate( &statScreen1_window, &distanceTB, stat1Menu_distanceField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHeight );
	UG_TextboxSetForeColor(&statScreen1_window, stat1Menu_distanceField, C_YELLOW);
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen1_window, &totalTB, stat1Menu_totalField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &totalDistanceVB, stat1Menu_totalFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &totalDistanceU, stat1Menu_totalFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHeight );
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen1_window, &dailyTB, stat1Menu_dailyField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &dailyDistanceStatVB, stat1Menu_dailyFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &dailyDistanceStatU, stat1Menu_dailyFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHeight );
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen1_window, &toGoTB, stat1Menu_toGoField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &distanceToGoStatVB, stat1Menu_toGoFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &distanceToGoStatU, stat1Menu_toGoFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHeight );

	// ***************************************** Speed ************************************** //
	yOffset = STAT_SCREEN_Y_SPEED_OFFSET;

	UG_TextboxCreate( &statScreen1_window, &speedTB, stat1Menu_speedField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHeight );
	UG_TextboxSetForeColor(&statScreen1_window, stat1Menu_speedField, C_YELLOW);
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen1_window, &averageTB, stat1Menu_averageField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &averageSpeedVB, stat1Menu_averageFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &averageSpeedU, stat1Menu_averageFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHeight );
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen1_window, &maxTB, stat1Menu_maxField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &maxSpeedVB, stat1Menu_maxFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &maxSpeedU, stat1Menu_maxFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHeight );

	// ***************************************** Battery ************************************ //
	yOffset = STAT_SCREEN_Y_BATTERY_OFFSET;

	UG_TextboxCreate( &statScreen1_window, &batteryTB, stat1Menu_batteryField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHeight );
	UG_TextboxSetForeColor(&statScreen1_window, stat1Menu_batteryField, C_YELLOW);
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen1_window, &voltageTB, stat1Menu_voltageField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &batteryVoltageStatVB, stat1Menu_voltageFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &batteryVoltageStatU, stat1Menu_voltageFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHeight );
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen1_window, &currentTB, stat1Menu_currentField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &currentVB, stat1Menu_currentFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &currentU, stat1Menu_currentFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHeight );
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen1_window, &maxCurrentTB, stat1Menu_maxCurrentField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &maxCurrentVB, stat1Menu_maxCurrentFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &maxCurrentU, stat1Menu_maxCurrentFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHeight );
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen1_window, &powerStatTB, stat1Menu_powerField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &currentPowerStatVB, stat1Menu_powerFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &currentPowerStatU, stat1Menu_powerFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHeight );
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen1_window, &maxPowerStatTB, stat1Menu_maxPowerField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &maxPowerStatVB, stat1Menu_maxPowerFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHeight );
	UG_TextboxCreate( &statScreen1_window, &maxPowerStatU, stat1Menu_maxPowerFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHeight );
}

void statScreen1_update(statScreen1Field_t field)
{
	for (uint8_t i = 0; i < STAT1_SCREEN_MAX_OBJECTS; i++)
	{
		if (statScreen1_objBuff[i].id == field)
		{
			statScreen1_objBuff[i].state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;
			return;
		}
	}
}
void statScreen1_updateSiImpUnit(void)
{
	snprintf(speedUnitText, MPE_MAX_PARAM_LENGTH, MPe_data_configParamValue[ConfigParam_metricImperial] ? "mph" : "km/h");
	snprintf(distanceUnitText, MPE_MAX_PARAM_LENGTH, MPe_data_configParamValue[ConfigParam_metricImperial] ? "mi" : "km");
}
