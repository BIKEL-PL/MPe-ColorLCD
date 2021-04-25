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
#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>

extern uint16_t lcd_backlightDayBrightness_edit, lcd_backlightNightBrightness_edit, lcd_powerOffTimeMinutes_edit;

void lcd_init(void);
void lcd_power_off(void);
void lcd_synchro(void);
void lcd_toggleDayNightBacklight(void);
void lcd_automaticPowerOffManagement(void);
void lcd_setUserActivity(void);

#endif /* LCD_H_ */
