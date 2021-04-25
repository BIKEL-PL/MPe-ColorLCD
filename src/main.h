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

#ifndef _MAIN_H_
#define _MAIN_H_

// INTERRUPTS PRIORITIES
// Define for the NVIC IRQChannel Preemption Priority
// lower number has higher priority
#define USART1_INTERRUPT_PRIORITY       3
#define USART1_DMA_INTERRUPT_PRIORITY   4
#define TIM4_INTERRUPT_PRIORITY         5
#define RTC_INTERRUT_PRIORITY           6

#define LCD_UPDATE_INTERVAL		10
#define AUTO_POWER_OFF_INTERVAL	1000

#define RUNNING_PACE_MODE		0

#ifdef USE_WITH_8X0C
//#define DISPLAY_850C_LB60 //old
#define DISPLAY_850C_LF60 //new
//#define DISPLAY_860C
#endif

#endif // _MAIN_H_
