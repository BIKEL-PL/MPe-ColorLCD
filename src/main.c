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

#include <running-pace.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "system_stm32f10x.h"
#include "stdio.h"
#include "stdbool.h"
#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "pins.h"
#include "timers.h"
#include "buttons.h"
#include "usart.h"
#include "rtc.h"
#include "data.h"
#include "lcd.h"
#include "adc.h"
#include "eeprom.h"
#include "screen.h"
#include "texts.h"

void SetSysClockTo104Mhz(void);

int main(void)
{
	volatile uint32_t currTick = 0;
	volatile uint32_t lcdUpdateLoop = 0;
	volatile uint32_t autoPowerOffLoop = 0;

	SetSysClockTo104Mhz();
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_WWDG, DISABLE);


	#if defined USE_WITH_BOOTLOADER && defined USE_WITH_8X0C
	// if building for original bootloader (from manufacturer), relocate flash after 20K (0x5000) that is the space that bootloader uses
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, (uint32_t) 0x5000);
	#endif

	#if defined USE_WITH_BOOTLOADER && defined USE_WITH_560C
	// if building for original bootloader (from manufacturer), relocate flash after 16K (0x4000) that is the space that bootloader uses
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, (uint32_t) 0x4000);
	#endif

	systick_init();
	pins_init();
	delay_ms(500);
	adc_init();
	system_power(1);
	usart_init();
	eeprom_init();
	buttons_init();
	rtc_init();
	timer_LCD_BL_init(); // drives LCD backlight
	running_pace_init();
	lcd_init();
	data_init();
	texts_init();
	screen_init();

	while (1)
	{
		currTick = get_time_base_counter_1ms();

		data_run(currTick);
		eeprom_synchro();

		if (currTick > lcdUpdateLoop + LCD_UPDATE_INTERVAL)
		{
			lcdUpdateLoop = currTick;
			buttons_run();
			screen_run(currTick);
		}
		if (currTick > autoPowerOffLoop + AUTO_POWER_OFF_INTERVAL)
		{
			autoPowerOffLoop = currTick;
			lcd_automaticPowerOffManagement();
		}
	}
}

void SetSysClockTo104Mhz(void)
{
	ErrorStatus HSEStartUpStatus;

	/* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if (HSEStartUpStatus == SUCCESS) {
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);

		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);

		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1);

		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);

		/* PLLCLK = 8MHz * 16 = 128 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_13);

		/* Enable PLL */
		RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
		}

		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source */
		while (RCC_GetSYSCLKSource() != 0x08) {
		}
	}
	else { /* If HSE fails to start-up, the application will have wrong clock configuration.
	 User can add here some code to deal with this error */

		/* Go to infinite loop */
		while (1) {
		}
	}
}
