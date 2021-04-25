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

#ifndef ADDRESS_H_
#define ADDRESS_H_

#include "data.h"

#define MPE_MAX_ADDRESS_LENGTH		4	// 3 address + \0
#define MPE_ADDRESS_ERROR			0xFF

extern const char MPe_address_configParams[ConfigParam_NumberOfParam][MPE_MAX_ADDRESS_LENGTH];

uint8_t address_getConfigParamNr(const char *string);

#endif /* ADDRESS_H_ */
