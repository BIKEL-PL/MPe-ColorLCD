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

#ifndef MPE_ASSISTMODE_H_
#define MPE_ASSISTMODE_H_

#include "stm32f10x.h"
#include "data.h"

extern char MPe_assistMode_editValue[MPE_MAX_PARAM_LENGTH];

void assistMode_init(void);
void assistMode_updateValue(void);
void assistMode_setLegalMode(BOOL mode);
void assistMode_changeLegalMode(void);
void assistMode_incrementValue(void);
void assistMode_decrementValue(void);
uint32_t assistMode_getLastEditingTime(void);

#endif /* MPE_ASSISTMODE_H_ */
