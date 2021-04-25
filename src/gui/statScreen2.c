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
#include "statScreen2.h"
#include "statScreen1.h"
#include "texts.h"
#include "data.h"
#include <stdio.h>

#define STAT2_SCREEN_MAX_OBJECTS		stat2Menu_Fieldcount

#define STAT_SCREEN_FONT			CONFIGURATIONS_TEXT_FONT


#define STAT_SCREEN_TXT_ALIGN		ALIGN_TOP_RIGHT
#define STAT_SCREEN_DTA_ALIGN		ALIGN_TOP_CENTER
#define STAT_SCREEN_UNIT_ALIGN		ALIGN_TOP_LEFT

#define STAT_SCREEN_X_OFFSET		2
#define STAT_SCREEN_X_TXT_START		(STAT_SCREEN_X_OFFSET)
#define STAT_SCREEN_X_VAL_START		(SCREEN_WIDTH * 0.44 + STAT_SCREEN_X_OFFSET)
#ifdef USE_WITH_8X0C
#define STAT_SCREEN_X_UNT_START		(SCREEN_WIDTH * 0.78 + STAT_SCREEN_X_OFFSET)
#endif

#ifdef USE_WITH_560C
#define STAT_SCREEN_X_UNT_START		(SCREEN_WIDTH * 0.74 + STAT_SCREEN_X_OFFSET)
#endif

#define STAT_SCREEN_X_TXT_END		(STAT_SCREEN_X_VAL_START - STAT_SCREEN_X_OFFSET)
#define STAT_SCREEN_X_VAL_END		(STAT_SCREEN_X_UNT_START - STAT_SCREEN_X_OFFSET)
#define STAT_SCREEN_X_UNT_END		(SCREEN_WIDTH - STAT_SCREEN_X_OFFSET)

#define STAT_SCREEN_Y_BATTERYCD_OFFSET		SCREEN_HEIGHT * 0.01

#ifdef USE_WITH_8X0C
#define STAT_SCREEN_Y_TEMPERATURE_OFFSET	SCREEN_HEIGHT * 0.41
#endif

#ifdef USE_WITH_560C
#define STAT_SCREEN_Y_TEMPERATURE_OFFSET	SCREEN_HEIGHT * 0.44
#endif
#define STAT_SCREEN_Y_OTHER_OFFSET			SCREEN_HEIGHT * 0.63

UG_WINDOW statScreen2_window;

static UG_OBJECT statScreen2_objBuff[STAT2_SCREEN_MAX_OBJECTS];
static char tempUnitText[MPE_MAX_PARAM_LENGTH] = "st C";
static char energyConsumptionUnitText[MPE_MAX_PARAM_LENGTH] = "Wh/km";

// Texts
static UG_TEXTBOX batteryContTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_batteryCd], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX capacityTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_capacity], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX consumptionTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_consumption], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX consumptionWhTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_consumptionWh], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX remainedTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_remained], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX numberOfChargeCyclesTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_numberOfChargeCycles], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX energyConsumptionTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_energyConsumption], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);

static UG_TEXTBOX temperatureTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_temperature], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX temp1TB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_temp1], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX temp2TB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_temp2], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);

static UG_TEXTBOX otherTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_other], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX cadenceTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_cadence], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX throttleInTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_throttleIn], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX torqueADCTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_torqueADC], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX torqueKGFTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_torqueKGF], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);
static UG_TEXTBOX softwareVersionTB = FIELD_READONLY_STRING(MPe_texts_statScreen[Texts_statScreen_softwareVersion], &STAT_SCREEN_FONT, STAT_SCREEN_TXT_ALIGN);

// Data
static UG_TEXTBOX batteryCapacityVB = FIELD_READONLY_STRING(MPe_data_param[Param_BatteryCapacity], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);
static UG_TEXTBOX currentBatteryConsumptionVB = FIELD_READONLY_STRING(MPe_data_param[Param_CurrentBatteryConsumption], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);
static UG_TEXTBOX currentBatteryConsumptionWhVB = FIELD_READONLY_STRING(MPe_data_param[Param_WhUsed], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);
static UG_TEXTBOX batteryChargePercentageStatVB = FIELD_READONLY_STRING(MPe_data_param[Param_BatteryChargePercentage], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);
static UG_TEXTBOX numberOfChargeCyclesVB = FIELD_READONLY_STRING(MPe_data_param[Param_NumberOfChargeCycles], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);
static UG_TEXTBOX energyConsumptionVB = FIELD_READONLY_STRING(MPe_data_param[Param_EnergyConsumptionPerKilometer], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);

static UG_TEXTBOX temperature1StatVB = FIELD_READONLY_STRING(MPe_data_param[Param_Temperature1], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);
static UG_TEXTBOX temperature2VB = FIELD_READONLY_STRING(MPe_data_param[Param_Temperature2], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);

static UG_TEXTBOX cadenceVB = FIELD_READONLY_STRING(MPe_data_param[Param_Cadence], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);
static UG_TEXTBOX throttleInVB = FIELD_READONLY_STRING(MPe_data_param[Param_ThrottleIn], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);
static UG_TEXTBOX torqueADCVB = FIELD_READONLY_STRING(MPe_data_param[Param_TorqueSensorADC], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);
static UG_TEXTBOX torqueKGFVB = FIELD_READONLY_STRING(MPe_data_param[Param_WeightOnPedal], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);

static UG_TEXTBOX softwareVersionVB = FIELD_READONLY_STRING(MPe_data_param[Param_SoftwareVersion], &STAT_SCREEN_FONT, STAT_SCREEN_DTA_ALIGN);

// Units
static UG_TEXTBOX batteryCapacityU = FIELD_READONLY_STRING("Ah", &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);
static UG_TEXTBOX currentBatteryConsumptionU = FIELD_READONLY_STRING("Ah", &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);
static UG_TEXTBOX currentBatteryConsumptionWhU = FIELD_READONLY_STRING("Wh", &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);
static UG_TEXTBOX batteryChargePercentageStatU = FIELD_READONLY_STRING("%", &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);
static UG_TEXTBOX energyConsumptionU = FIELD_READONLY_STRING(energyConsumptionUnitText, &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);

static UG_TEXTBOX temperature1StatU= FIELD_READONLY_STRING(tempUnitText, &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);
static UG_TEXTBOX temperature2U = FIELD_READONLY_STRING(tempUnitText, &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);
static UG_TEXTBOX cadenceU = FIELD_READONLY_STRING("rpm", &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);
static UG_TEXTBOX throttleInU = FIELD_READONLY_STRING("V*100", &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);
static UG_TEXTBOX torqueADCU = FIELD_READONLY_STRING("-", &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);
static UG_TEXTBOX torqueKGFU = FIELD_READONLY_STRING("kg*10", &STAT_SCREEN_FONT, STAT_SCREEN_UNIT_ALIGN);

void statScreen2_callback( UG_MESSAGE* msg )
{
	if (msg->type == MSG_TYPE_OBJECT)
	{
		if (msg->id == OBJ_TYPE_BUTTON)
	    {
			if (msg->event == DOWN_CLICK || msg->event == UP_CLICK)
			{
				screen_changeWindow(&statScreen1_window);
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

void statScreen2_init(void)
{
	uint16_t yOffset = STAT_SCREEN_Y_BATTERYCD_OFFSET;
	const uint8_t textHigh = 20;
	const uint8_t rowOffset = textHigh + 2;

	// Create Window
	UG_WindowCreate( &statScreen2_window, statScreen2_objBuff, STAT2_SCREEN_MAX_OBJECTS, statScreen2_callback );
	UG_WindowSetBackColor( &statScreen2_window, C_BLACK);
	UG_WindowSetForeColor( &statScreen2_window, C_WHITE);
	UG_WindowSetStyle(&statScreen2_window, WND_STYLE_2D);

	// ***************************************** Battery cd *********************************** //
	UG_TextboxCreate( &statScreen2_window, &batteryContTB, stat2Menu_batteryCdField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHigh );
	UG_TextboxSetForeColor(&statScreen2_window, stat2Menu_batteryCdField, C_YELLOW);
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen2_window, &capacityTB, stat2Menu_capacityField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &batteryCapacityVB, stat2Menu_capacityFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &batteryCapacityU, stat2Menu_capacityFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHigh );
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen2_window, &consumptionTB, stat2Menu_consumptionField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &currentBatteryConsumptionVB, stat2Menu_consumptionFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &currentBatteryConsumptionU, stat2Menu_consumptionFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHigh );
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen2_window, &consumptionWhTB, stat2Menu_consumptionWhField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &currentBatteryConsumptionWhVB, stat2Menu_consumptionWhFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &currentBatteryConsumptionWhU, stat2Menu_consumptionWhFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHigh );
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen2_window, &remainedTB, stat2Menu_remainedField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &batteryChargePercentageStatVB, stat2Menu_remainedFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &batteryChargePercentageStatU, stat2Menu_remainedFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHigh );
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen2_window, &numberOfChargeCyclesTB, stat2Menu_numberOfChargeCyclesField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &numberOfChargeCyclesVB, stat2Menu_numberOfChargeCyclesFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHigh );
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen2_window, &energyConsumptionTB, stat2Menu_energyConsumptionField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &energyConsumptionVB, stat2Menu_energyConsumptionFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &energyConsumptionU, stat2Menu_energyConsumptionFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHigh );

	// ***************************************** Temperature *********************************** //
	yOffset = STAT_SCREEN_Y_TEMPERATURE_OFFSET;

	UG_TextboxCreate( &statScreen2_window, &temperatureTB, stat2Menu_temperatureField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHigh );
	UG_TextboxSetForeColor(&statScreen2_window, stat2Menu_temperatureField, C_YELLOW);
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen2_window, &temp1TB, stat2Menu_temp1Field, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &temperature1StatVB, stat2Menu_temp1FieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &temperature1StatU, stat2Menu_temp1FieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHigh );
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen2_window, &temp2TB, stat2Menu_temp2Field, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &temperature2VB, stat2Menu_temp2FieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &temperature2U, stat2Menu_temp2FieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHigh );
	// ***************************************** Other *********************************** //
	yOffset = STAT_SCREEN_Y_OTHER_OFFSET;

	UG_TextboxCreate( &statScreen2_window, &otherTB, stat2Menu_otherField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHigh );
	UG_TextboxSetForeColor(&statScreen2_window, stat2Menu_otherField, C_YELLOW);
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen2_window, &cadenceTB, stat2Menu_cadenceField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &cadenceVB, stat2Menu_cadenceFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &cadenceU, stat2Menu_cadenceFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHigh );
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen2_window, &throttleInTB, stat2Menu_throttleInField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &throttleInVB, stat2Menu_throttleInFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &throttleInU, stat2Menu_throttleInFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHigh );
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen2_window, &torqueADCTB, stat2Menu_torqueADCField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &torqueADCVB, stat2Menu_torqueADCFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &torqueADCU, stat2Menu_torqueADCFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHigh );
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen2_window, &torqueKGFTB, stat2Menu_torqueKGFField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &torqueKGFVB, stat2Menu_torqueKGFFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &torqueKGFU, stat2Menu_torqueKGFFieldUnit, STAT_SCREEN_X_UNT_START, yOffset, STAT_SCREEN_X_UNT_END, yOffset + textHigh );
	yOffset += rowOffset;
	UG_TextboxCreate( &statScreen2_window, &softwareVersionTB, stat2Menu_softwareVersionField, STAT_SCREEN_X_TXT_START, yOffset, STAT_SCREEN_X_TXT_END, yOffset + textHigh );
	UG_TextboxCreate( &statScreen2_window, &softwareVersionVB, stat2Menu_softwareVersionFieldVal, STAT_SCREEN_X_VAL_START, yOffset, STAT_SCREEN_X_VAL_END, yOffset + textHigh );
}

void statScreen2_update(statScreen2Field_t field)
{
	for (uint8_t i = 0; i < STAT2_SCREEN_MAX_OBJECTS; i++)
	{
		if (statScreen2_objBuff[i].id == field)
		{
			statScreen2_objBuff[i].state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;
			return;
		}
	}
}
void statScreen2_updateSiImpUnit(void)
{
	snprintf(energyConsumptionUnitText, MPE_MAX_PARAM_LENGTH, MPe_data_configParamValue[ConfigParam_metricImperial] ? "Wh/mi" : "Wh/km");
}
void statScreen2_updateTempUnit(void)
{
	snprintf(tempUnitText, MPE_MAX_PARAM_LENGTH, MPe_data_configParamValue[ConfigParam_temperatureUnit] ? "*F" : "*C");
}
