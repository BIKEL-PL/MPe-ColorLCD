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

#ifdef USE_WITH_560C
#include "stm32f10x.h"
#include "startScreen.h"
#include "screen.h"
#include "version.h"
#include <stdio.h>
#include "img/logo/bikelpl.h"


#define START_SCREEN_MAX_OBJECTS		startScreen_Fieldcount
#define START_SCREEN_VERSION_SIZE		24

UG_WINDOW startScreen_window;

static UG_OBJECT startScreen_objBuff[START_SCREEN_MAX_OBJECTS];
static char versionText[START_SCREEN_VERSION_SIZE] = "";

// TEXTBOX
static UG_TEXTBOX logoTB = FIELD_READONLY_STRING("MPe", &MEDIUM_TEXT_FONT, ALIGN_TOP_CENTER);
static UG_TEXTBOX wwwTB = FIELD_READONLY_STRING("ebikecomputer.com", &CONFIGURATIONS_TEXT_FONT, ALIGN_TOP_CENTER);
static UG_TEXTBOX sloganTB = FIELD_READONLY_STRING("Have a nice ride!", &CONFIGURATIONS_TEXT_FONT, ALIGN_TOP_CENTER);
static UG_TEXTBOX versionTB = FIELD_READONLY_STRING(versionText, &CONFIGURATIONS_TEXT_FONT, ALIGN_TOP_CENTER);

// IMG
static UG_IMAGE imageStart;

void startScreen_callback( UG_MESSAGE* msg )
{
	if (msg->type == MSG_TYPE_OBJECT)
	{
		if (msg->id == OBJ_TYPE_BUTTON)
	    {
	    }
	}
}

void startScreen_init(void)
{
	// Create Window
	UG_WindowCreate( &startScreen_window, startScreen_objBuff, START_SCREEN_MAX_OBJECTS, startScreen_callback );
	UG_WindowSetBackColor( &startScreen_window, C_BLACK);
	UG_WindowSetForeColor( &startScreen_window, C_WHITE);
	UG_WindowSetStyle(&startScreen_window, WND_STYLE_2D);

	// Create Textboxes
	UG_TextboxCreate( &startScreen_window, &logoTB, logoMPeField, 10, 25, 230, 55 );

	UG_TextboxCreate( &startScreen_window, &wwwTB, wwwField, 10, 70, 230, 90 );

	UG_TextboxCreate( &startScreen_window, &sloganTB, sloganField, 10, 295, 230, 315 );

	snprintf(versionText, START_SCREEN_VERSION_SIZE, "MiniColor 560C / v%d.%d", VERSION_HIGH, VERSION_LOW);
	UG_TextboxCreate( &startScreen_window, &versionTB, versionField, 10, 350, 230, 370 );

	// Create the images

	//bikel.pl
	UG_ImageCreate ( &startScreen_window, &imageStart, startImageField, 70, 130, 170, 275 );
	UG_ImageSetBMP( &startScreen_window, startImageField, &bikelplBmp );

}

void startScreen_update(startScreenField_t field)
{
	for (uint8_t i = 0; i < START_SCREEN_MAX_OBJECTS; i++)
	{
		if (startScreen_objBuff[i].id == field)
		{
			startScreen_objBuff[i].state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;
			return;
		}
	}
}
#endif
