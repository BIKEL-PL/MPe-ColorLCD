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

#ifndef CMD_H_
#define CMD_H_

#include "stdio.h"

#define ONE_PARAM_REQUEST		("6004 1")
#define ONE_PARAM_REQUEST_SIZE	(sizeof(ONE_PARAM_REQUEST))

#define ALL_DATA_REQUEST		("6004 6")
#define ALL_DATA_REQUEST_SIZE	(sizeof(ALL_DATA_REQUEST))

#define SAVE_DATA				("6004 2")
#define SAVE_DATA_SIZE			(sizeof(SAVE_DATA))

#define RESET_CURR_VALUES_REQUEST		("6004 4")
#define RESET_CURR_VALUES_REQUEST_SIZE	(sizeof(RESET_CURR_VALUES_REQUEST))

#define RESET_BATTERY_INDICATOR_REQUEST			("6004 5")
#define RESET_BATTERY_INDICATOR_REQUEST_SIZE	(sizeof(RESET_BATTERY_INDICATOR_REQUEST))

#define RESET_CURRENT_SENSOR_REQUEST		("6004 3")
#define RESET_CURRENT_SENSOR_REQUEST_SIZE	(sizeof(RESET_CURRENT_SENSOR_REQUEST))

#define CRUISE_CONTROL_SWITCH_ON_REQUEST		("6004 7")
#define CRUISE_CONTROL_SWITCH_ON_REQUEST_SIZE	(sizeof(CRUISE_CONTROL_SWITCH_ON_REQUEST))

#define CRUISE_CONTROL_SWITCH_OFF_REQUEST		("6004 99")
#define CRUISE_CONTROL_SWITCH_OFF_REQUEST_SIZE	(sizeof(CRUISE_CONTROL_SWITCH_OFF_REQUEST))

#define CRUISE_CONTROL_ACCELERATION_REQUEST			("6004 9")
#define CRUISE_CONTROL_ACCELERATION_REQUEST_SIZE	(sizeof(CRUISE_CONTROL_ACCELERATION_REQUEST))

#define CRUISE_CONTROL_SLOW_DOWN_REQUEST		("6004 8")
#define CRUISE_CONTROL_SLOW_DOWN_REQUEST_SIZE	(sizeof(CRUISE_CONTROL_SLOW_DOWN_REQUEST))

#define LIGHT_TOGGLE_REQUEST		("6004 11")
#define LIGHT_TOGGLE_REQUEST_SIZE	(sizeof(LIGHT_TOGGLE_REQUEST))

#define PRESET_DISTANCE_REQUEST				("6004 2 4")
#define PRESET_BATT_CHARGE_NO_REQUEST		("6004 2 44")

#endif