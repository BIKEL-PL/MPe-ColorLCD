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

#include "stm32f10x.h"
#include "buttons.h"
#include "pins.h"
#include "screen.h"
#include "main.h"
#include "eeprom.h"

#define BUTTONS_RUN_INTERVAL	LCD_UPDATE_INTERVAL
#define BUTTONS_BUTTON_NONE		0xFF

#define BUTTONS_LONG_CLICK_MS		500
#define BUTTONS_SLOW_REPEAT_MS		200
#define BUTTONS_MEDIUM_REPEAT_MS	100
#define BUTTONS_FAST_REPEAT_MS		50

#define BUTTONS_FAST_REPEAT_LIMIT	90
#define BUTTONS_MEDIUM_REPEAT_LIMIT	20

typedef enum {
	BUTTON_ONOFF,
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_UP_DOWN,
	BUTTON_ONOFF_UP,
	BUTTON_ONOFF_DOWN,
	BUTTON_COUNT
} buttons_t;
typedef enum {
	BUTTON_ONE_CLICK,
	BUTTON_LONG_CLICK,
	BUTTON_SLOW_REPEAT,
	BUTTON_MEDIUM_REPEAT,
	BUTTON_FAST_REPEAT
} buttonEvent_t;

typedef struct
{
   uint32_t timer;
   uint16_t counter;
   uint8_t lastState;
} buttonsHandling_t;

static uint16_t buttonsInverted;
uint16_t buttonsInverted_edit;

static buttonsHandling_t buttons_data[BUTTON_COUNT];
static uint8_t buttons_pressedButtonNr;

void buttons_keyDown(buttons_t button);
void buttons_keyUp(buttons_t button);
void buttons_keyReset(buttons_t button);
void buttons_setEvent(buttons_t button, buttonEvent_t event);

void buttons_init(void)
{
	buttons_pressedButtonNr = BUTTONS_BUTTON_NONE;
	for (uint8_t i = 0; i < BUTTON_COUNT; i++)
	{
		buttons_data[i].timer = 0;
		buttons_data[i].counter = 0;
		buttons_data[i].lastState = FALSE;
	}

	// Eeprom read
	buttonsInverted_edit = buttonsInverted = (uint16_t)eeprom_readParam(eepromParam_buttonsInverted);
}

void buttons_synchro(void)
{
	if (buttonsInverted_edit != buttonsInverted)
	{
		buttonsInverted = buttonsInverted_edit;
		eeprom_saveParam(eepromParam_buttonsInverted, buttonsInverted);
	}
}

void buttons_run(void)
{
	uint8_t onoff_state = GPIO_ReadInputDataBit(BUTTON_ONOFF__PORT, BUTTON_ONOFF__PIN);
	uint8_t up_state = GPIO_ReadInputDataBit(BUTTON_UP__PORT, BUTTON_UP__PIN);
	uint8_t down_state = GPIO_ReadInputDataBit(BUTTON_DOWN__PORT, BUTTON_DOWN__PIN);

	#if defined USE_WITH_560C || defined DISPLAY_860C
	uint8_t m_state = GPIO_ReadInputDataBit(BUTTON_M__PORT, BUTTON_M__PIN);
	#endif


	if(buttonsInverted)
	{
		uint8_t temp_up_state = up_state;
		uint8_t temp_down_state = down_state;

		up_state = temp_down_state;
		down_state = temp_up_state;
	}
    #if defined USE_WITH_560C || defined DISPLAY_860C
	if (!onoff_state || !m_state)
    #else
	if (!onoff_state)
    #endif
	{
		if (!up_state) // wcisniete przyciski onoff i up
		{
			buttons_keyDown(BUTTON_ONOFF_UP);
			buttons_keyReset(BUTTON_ONOFF);					//  ________
			buttons_keyReset(BUTTON_ONOFF_DOWN);			// ||###| - ||
			buttons_keyReset(BUTTON_UP);					// ||  ###	||
			buttons_keyReset(BUTTON_DOWN);					//	---------
			buttons_keyReset(BUTTON_UP_DOWN);
		}
		else if (!down_state) // wcisniete przyciski onoff i down
		{
			buttons_keyDown(BUTTON_ONOFF_DOWN);
			buttons_keyReset(BUTTON_ONOFF);					//  ________
			buttons_keyReset(BUTTON_ONOFF_UP);				// || + |###||
			buttons_keyReset(BUTTON_UP);					// ||  ###	||
			buttons_keyReset(BUTTON_DOWN);					//	---------
			buttons_keyReset(BUTTON_UP_DOWN);
		}
		else // wcisniety przycisk onoff
		{
			buttons_keyDown(BUTTON_ONOFF);
			buttons_keyReset(BUTTON_ONOFF_DOWN);			//  ________
			buttons_keyReset(BUTTON_ONOFF_UP);				// || + | - ||
			buttons_keyReset(BUTTON_UP);					// ||  ###	||
			buttons_keyReset(BUTTON_DOWN);					//	---------
			buttons_keyReset(BUTTON_UP_DOWN);
		}
	}
	else // przycisk onoff NIE wciœniêty
	{
		buttons_keyUp(BUTTON_ONOFF);
		buttons_keyReset(BUTTON_ONOFF_DOWN);
		buttons_keyReset(BUTTON_ONOFF_UP);

		if (!up_state)
		{
			if (!down_state) // wcisniete przyciski up i down
			{
				buttons_keyDown(BUTTON_UP_DOWN);			//  ________
				buttons_keyReset(BUTTON_UP);				// ||###|###||
				buttons_keyReset(BUTTON_DOWN);				// ||   O	||
			}												//	---------
			else // wcisniety przycisk up
			{
				buttons_keyDown(BUTTON_UP);					//  ________
				buttons_keyReset(BUTTON_UP_DOWN);			// ||###| - ||
				buttons_keyReset(BUTTON_DOWN);				// ||   O	||
			}												//	---------
		}
		else // przyciski onoff i up NIE wcisniete
		{
			buttons_keyUp(BUTTON_UP);
			buttons_keyReset(BUTTON_UP_DOWN);

			if (!down_state) // wcisniety przycisk down
			{												//  ________
				buttons_keyDown(BUTTON_DOWN);				// || + |###||
			}												// ||   O	||
			else 											//	---------
			{
				buttons_keyUp(BUTTON_DOWN); // zaden przycisk nie wcisniety
				buttons_pressedButtonNr = BUTTONS_BUTTON_NONE;
			}
		}
	}
}

void buttons_keyDown(buttons_t button)
{
	if (buttons_data[button].lastState == TRUE)
	{
		buttons_data[button].timer += BUTTONS_RUN_INTERVAL;

		if (buttons_data[button].counter == 0)
		{
			if (buttons_data[button].timer >= BUTTONS_LONG_CLICK_MS)
			{
				buttons_setEvent(button, BUTTON_LONG_CLICK);
				buttons_data[button].counter++;
				buttons_data[button].timer = 0;
			}
		}
		else if (buttons_data[button].counter > BUTTONS_FAST_REPEAT_LIMIT)
		{
			if (buttons_data[button].timer >= BUTTONS_FAST_REPEAT_MS)
			{
				buttons_setEvent(button, BUTTON_FAST_REPEAT);
				buttons_data[button].counter++;
				buttons_data[button].timer = 0;
			}
		}
		else if (buttons_data[button].counter > BUTTONS_MEDIUM_REPEAT_LIMIT)
		{
			if (buttons_data[button].timer >= BUTTONS_MEDIUM_REPEAT_MS)
			{
				buttons_setEvent(button, BUTTON_MEDIUM_REPEAT);
				buttons_data[button].counter++;
				buttons_data[button].timer = 0;
			}
		}
		else // SLOW REPEAT
		{
			if (buttons_data[button].timer >= BUTTONS_SLOW_REPEAT_MS)
			{
				buttons_setEvent(button, BUTTON_SLOW_REPEAT);
				buttons_data[button].counter++;
				buttons_data[button].timer = 0;
			}
		}
	}
	else
	{
		buttons_data[button].lastState = TRUE;
	}
}

void buttons_keyUp(buttons_t button)
{
	if (buttons_data[button].lastState == TRUE
			&& buttons_data[button].counter == 0)
	{
		// one press action
		buttons_setEvent(button, BUTTON_ONE_CLICK);
	}
	buttons_keyReset(button);
}

void buttons_keyReset(buttons_t button)
{
	buttons_data[button].timer = 0;
	buttons_data[button].counter = 0;
	buttons_data[button].lastState = FALSE;
}

void buttons_setEvent(buttons_t button, buttonEvent_t event)
{
	if (buttons_pressedButtonNr != button
			&& buttons_pressedButtonNr != BUTTONS_BUTTON_NONE) return;
	else buttons_pressedButtonNr = button;

	click_t click = ONE_CLICK;
	switch (event)
	{
	case BUTTON_SLOW_REPEAT: click = SLOW_REPEAT;
	break;
	case BUTTON_MEDIUM_REPEAT: click = MEDIUM_REPEAT;
	break;
	case BUTTON_FAST_REPEAT: click = FAST_REPEAT;
	break;
	default: click = ONE_CLICK;
	}

	if (event == BUTTON_LONG_CLICK)
	{
		switch (button)
		{
		case BUTTON_ONOFF:
			screen_setEvent(ONOFF_LONG_CLICK, click);
			break;
		case BUTTON_UP:
			screen_setEvent(UP_LONG_CLICK, click);
			break;
		case BUTTON_DOWN:
			screen_setEvent(DOWN_LONG_CLICK, click);
			break;
		case BUTTON_ONOFF_UP:
			screen_setEvent(ONOFFUP_LONG_CLICK, click);
			break;
		case BUTTON_ONOFF_DOWN:
			screen_setEvent(ONOFFDOWN_LONG_CLICK, click);
			break;
		case BUTTON_UP_DOWN:
			screen_setEvent(UPDOWN_LONG_CLICK, click);
			break;
		default: __NOP();
		}
	}
	else
	{
		switch (button)
		{
		case BUTTON_ONOFF:
			screen_setEvent(ONOFF_CLICK, click);
			break;
		case BUTTON_UP:
			screen_setEvent(UP_CLICK, click);
			break;
		case BUTTON_DOWN:
			screen_setEvent(DOWN_CLICK, click);
			break;
		default: __NOP();
		}
	}
}
