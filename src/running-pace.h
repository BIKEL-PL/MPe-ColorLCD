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

#ifndef RUNNING_PACE_H_
#define RUNNING_PACE_H_

#include <stdint.h>

void running_pace_init(void);
float running_pace_getPace(void);
void running_pace_updateSpeed(void);

#endif /* RUNNING_PACE_H_ */
