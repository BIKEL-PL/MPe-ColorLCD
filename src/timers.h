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

#ifndef TIMERS_H_
#define TIMERS_H_

void systick_init (void);
volatile uint32_t get_time_base_counter_1ms (void);
void delay_ms (uint32_t ms);
void timer_LCD_BL_init(void);
void timer4_init(void);

#endif /* TIMERS_H_ */
