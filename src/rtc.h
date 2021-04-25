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

#ifndef _RTC_H_
#define _RTC_H_

#include "stdio.h"

typedef struct _rtc_time {
	uint8_t ui8_hours;
	uint8_t ui8_minutes;
} rtc_time_t;

void rtc_init(void);
void rtc_set_time(rtc_time_t *rtc_time);
rtc_time_t* rtc_get_time(void);
rtc_time_t* rtc_get_time_since_startup(void);
void rtc_updateClock(uint8_t hour, uint8_t min);
void rtc_synchro(void);

extern uint32_t ui32_seconds_since_startup;
extern uint16_t rtc_editTime_hour, rtc_editTime_min;

#endif /* _RTC_H_ */
