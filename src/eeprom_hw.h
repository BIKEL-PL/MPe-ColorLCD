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

#pragma once
#include "stdint.h"
#include "stdbool.h"

#define ADDRESS_KEY 0
#define KEY 7

void eeprom_hw_init(void);
uint32_t eeprom_write(uint32_t ui32_address, uint8_t ui8_data);

bool flash_write_words(const void *value, uint16_t length_words);

// Read raw EEPROM data, return false if it is blank or malformatted
bool flash_read_words(void *dest, uint16_t length_words);


