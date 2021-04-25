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

#ifndef STATSCREEN2_H_
#define STATSCREEN2_H_

#include "screen.h"

typedef enum {
	stat2Menu_batteryCdField,
	stat2Menu_capacityField, stat2Menu_capacityFieldVal, stat2Menu_capacityFieldUnit,
	stat2Menu_consumptionField, stat2Menu_consumptionFieldVal, stat2Menu_consumptionFieldUnit,
	stat2Menu_consumptionWhField, stat2Menu_consumptionWhFieldVal, stat2Menu_consumptionWhFieldUnit,
	stat2Menu_remainedField, stat2Menu_remainedFieldVal, stat2Menu_remainedFieldUnit,
	stat2Menu_numberOfChargeCyclesField, stat2Menu_numberOfChargeCyclesFieldVal,
	stat2Menu_energyConsumptionField, stat2Menu_energyConsumptionFieldVal, stat2Menu_energyConsumptionFieldUnit,

	stat2Menu_temperatureField,
	stat2Menu_temp1Field, stat2Menu_temp1FieldVal, stat2Menu_temp1FieldUnit,
	stat2Menu_temp2Field, stat2Menu_temp2FieldVal, stat2Menu_temp2FieldUnit,

	stat2Menu_otherField,
	stat2Menu_cadenceField, stat2Menu_cadenceFieldVal, stat2Menu_cadenceFieldUnit,
	stat2Menu_throttleInField, stat2Menu_throttleInFieldVal, stat2Menu_throttleInFieldUnit,
	stat2Menu_torqueADCField, stat2Menu_torqueADCFieldVal, stat2Menu_torqueADCFieldUnit,
	stat2Menu_torqueKGFField, stat2Menu_torqueKGFFieldVal, stat2Menu_torqueKGFFieldUnit,
	stat2Menu_softwareVersionField, stat2Menu_softwareVersionFieldVal,

	stat2Menu_Fieldcount
} statScreen2Field_t;

extern UG_WINDOW statScreen2_window;

void statScreen2_init(void);
void statScreen2_update(statScreen2Field_t field);
void statScreen2_updateSiImpUnit(void);
void statScreen2_updateTempUnit(void);

#endif /* STATSCREEN2_H_ */
