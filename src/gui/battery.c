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

#include "battery.h"
#include "screen.h"
#include "data.h"
#include <stdlib.h>

#define BATTERY_FRAME_START_X		9
#define BATTERY_FRAME_START_Y 		4
#define BATTERY_FRAME_END_X 		66
#define BATTERY_FRAME_END_Y			25
#define BATTERY_FRAME_COLOR			C_WHITE

static void clearBatteryLevel(uint32_t xstart)
{
	uint32_t xs, xe, ys, ye;
	xs = xstart;
	xe = BATTERY_FRAME_END_X - 7;
	ys = BATTERY_FRAME_START_Y + 2; ye = BATTERY_FRAME_END_Y - 2;
	if (xe >= xstart) UG_FillFrame(xs, ys, xe, ye, C_BLACK);

	xe = BATTERY_FRAME_START_X + 51;
	ys = BATTERY_FRAME_START_Y + 3; ye = BATTERY_FRAME_END_Y - 3;
	if (xe >= xstart) UG_DrawLine(xs, ys, xe, ye, C_BLACK);

	xe = BATTERY_FRAME_START_X + 54;
	ys = BATTERY_FRAME_START_Y + 6; ye = BATTERY_FRAME_END_Y - 6;
	if (xe >= xstart) UG_FillFrame(xs, ys, xe, ye, C_BLACK);

	xe = BATTERY_FRAME_START_X + 55;
	ys = BATTERY_FRAME_START_Y + 7; ye = BATTERY_FRAME_END_Y - 7;
	if (xe >= xstart) UG_DrawLine(xs, ys, xe, ye, C_BLACK);
}
static void renderBatteryLevel(uint32_t percentageValue)
{
	uint32_t xs, xe, ys, ye, level;
	UG_COLOR c;

	if (percentageValue > 100) percentageValue = 100;

	// find the color to draw the bars
	if (percentageValue > 61) c = C_GREEN;
	else if (percentageValue > 41) c = C_YELLOW;
	else if (percentageValue > 21) c = C_ORANGE;
	else c = C_RED;

	// left strip 0-2%
	xs = xe = BATTERY_FRAME_START_X + 2;
	ys = BATTERY_FRAME_START_Y + 3; ye = BATTERY_FRAME_END_Y - 3;
	if (percentageValue < 2)
	{
		UG_DrawLine(xs, ys, xe, ye, C_BLACK);
		clearBatteryLevel(xe + 1);
		return;
	}
	else UG_DrawLine(xs, ys, xe, ye, c);

	ys = BATTERY_FRAME_START_Y + 2; ye = BATTERY_FRAME_END_Y - 2;

	// first level 2-20%
	level = percentageValue > 20 ? 20 : percentageValue;
	xs = BATTERY_FRAME_START_X + 3; xe = BATTERY_FRAME_START_X + 1 + (level / 2);
	if (xe >= xs) UG_FillFrame(xs, ys, xe, ye, c);

	if (percentageValue > 20)
	{
		// second level 21-40%
		level = percentageValue > 40 ? 20 : percentageValue - 20;
		xs = BATTERY_FRAME_START_X + 13; xe = BATTERY_FRAME_START_X + 12 + (level / 2);
		if (xe >= xs) UG_FillFrame(xs, ys, xe, ye, c);

		// third level - 41-60%
		if (percentageValue > 40)
		{
			level = percentageValue > 60 ? 20 : percentageValue - 40;
			xs = BATTERY_FRAME_START_X + 24; xe = BATTERY_FRAME_START_X + 23 + (level / 2);
			if (xe >= xs) UG_FillFrame(xs, ys, xe, ye, c);

			// fourth level 61-80%
			if (percentageValue > 60)
			{
				level = percentageValue > 80 ? 20 : percentageValue - 60;
				xs = BATTERY_FRAME_START_X + 35; xe = BATTERY_FRAME_START_X + 34 + (level / 2);
				if (xe >= xs) UG_FillFrame(xs, ys, xe, ye, c);

				// fifth level > 80%
				if (percentageValue > 80)
				{
					// 81-90%
					level = percentageValue > 90 ? 10 : percentageValue - 80;
					xs = BATTERY_FRAME_START_X + 46; xe = BATTERY_FRAME_START_X + 45 + (level / 2);
					if (xe >= xs) UG_FillFrame(xs, ys, xe, ye, c);

					// 91-92%
					if (percentageValue > 90)
					{
						xs = BATTERY_FRAME_START_X + 51; xe = BATTERY_FRAME_START_X + 51;
						ys = BATTERY_FRAME_START_Y + 3; ye = BATTERY_FRAME_END_Y - 3;
						UG_DrawLine(xs, ys, xe, ye, c);

						// 93-98%
						if (percentageValue > 92)
						{
							level = percentageValue > 98 ? 18 : percentageValue - 80;
							xs = BATTERY_FRAME_START_X + 52; xe = BATTERY_FRAME_START_X + 45 + (level / 2);
							ys = BATTERY_FRAME_START_Y + 6; ye = BATTERY_FRAME_END_Y - 6;
							if (xe >= xs) UG_FillFrame(xs, ys, xe, ye, c);

							// 99-100%
							if (percentageValue > 98)
							{
								xs = BATTERY_FRAME_START_X + 55; xe = BATTERY_FRAME_START_X + 55;
								ys = BATTERY_FRAME_START_Y + 7; ye = BATTERY_FRAME_END_Y - 7;
								UG_DrawLine(xs, ys, xe, ye, c);
							}
						}
					}
				}
			}
		}
	}
	// clear rest battery level
	clearBatteryLevel(xe + 1);
}

void renderBattery(UG_WINDOW* wnd, UG_OBJECT* obj)
{
	if (obj->state & OBJ_STATE_UPDATE)
	{
		uint32_t xs, xe, ys, ye;

		if (obj->state & OBJ_STATE_VISIBLE)
		{
			// Full redraw necessary?
			if (obj->state & OBJ_STATE_REDRAW)
			{
				// need to draw battery frame

				// first, clear the full symbol area
				xs = BATTERY_FRAME_START_X; xe = BATTERY_FRAME_END_X;
				ys = BATTERY_FRAME_START_Y; ye = BATTERY_FRAME_END_Y;
				UG_FillFrame(xs, ys, xe, ye, C_BLACK);

				// draw left side
				xs = xe = BATTERY_FRAME_START_X;
				ys = BATTERY_FRAME_START_Y + 2; ye = BATTERY_FRAME_END_Y - 2;
				UG_DrawLine(xs, ys, xe, ye, BATTERY_FRAME_COLOR);

				// draw up-left corner
				xs = BATTERY_FRAME_START_X; xe = BATTERY_FRAME_START_X + 1;
				ys = ye = BATTERY_FRAME_START_Y + 1;
				UG_DrawLine(xs, ys, xe, ye, BATTERY_FRAME_COLOR);

				// draw up side
				xs = BATTERY_FRAME_START_X + 1; xe = BATTERY_FRAME_END_X - 5;
				ys = ye = BATTERY_FRAME_START_Y;
				UG_DrawLine(xs, ys, xe, ye, BATTERY_FRAME_COLOR);

				// draw up-right corner
				xs = BATTERY_FRAME_END_X - 5; xe = BATTERY_FRAME_END_X - 4;
				ys = ye = BATTERY_FRAME_START_Y + 1;
				UG_DrawLine(xs, ys, xe, ye, BATTERY_FRAME_COLOR);

				xs = xe = BATTERY_FRAME_END_X - 4;
				ys = BATTERY_FRAME_START_Y + 2; ye = BATTERY_FRAME_START_Y + 3;
				UG_DrawLine(xs, ys, xe, ye, BATTERY_FRAME_COLOR);

				xs = BATTERY_FRAME_END_X - 4; xe = BATTERY_FRAME_END_X - 1;
				ys = ye = BATTERY_FRAME_START_Y + 4;
				UG_DrawLine(xs, ys, xe, ye, BATTERY_FRAME_COLOR);

				xs = BATTERY_FRAME_END_X - 1; xe = BATTERY_FRAME_END_X;
				ys = ye = BATTERY_FRAME_START_Y + 5;
				UG_DrawLine(xs, ys, xe, ye, BATTERY_FRAME_COLOR);

				// draw right side
				xs = xe = BATTERY_FRAME_END_X;
				ys = BATTERY_FRAME_START_Y + 6; ye = BATTERY_FRAME_END_Y - 6;
				UG_DrawLine(xs, ys, xe, ye, BATTERY_FRAME_COLOR);

				// draw down-right corner
				xs = BATTERY_FRAME_END_X - 1; xe = BATTERY_FRAME_END_X;
				ys = ye = BATTERY_FRAME_END_Y - 5;
				UG_DrawLine(xs, ys, xe, ye, BATTERY_FRAME_COLOR);

				xs = BATTERY_FRAME_END_X - 4; xe = BATTERY_FRAME_END_X - 1;
				ys = ye = BATTERY_FRAME_END_Y - 4;
				UG_DrawLine(xs, ys, xe, ye, BATTERY_FRAME_COLOR);

				xs = xe = BATTERY_FRAME_END_X - 4;
				ys = BATTERY_FRAME_END_Y - 3; ye = BATTERY_FRAME_END_Y - 2;
				UG_DrawLine(xs, ys, xe, ye, BATTERY_FRAME_COLOR);

				xs = BATTERY_FRAME_END_X - 5; xe = BATTERY_FRAME_END_X - 4;
				ys = ye = BATTERY_FRAME_END_Y - 1;
				UG_DrawLine(xs, ys, xe, ye, BATTERY_FRAME_COLOR);

				// draw down side
				xs = 10; xe = BATTERY_FRAME_END_X - 5;
				ys = ye = BATTERY_FRAME_END_Y;
				UG_DrawLine(xs, ys, xe, ye, BATTERY_FRAME_COLOR);

				// draw down-left corner
				xs = BATTERY_FRAME_START_X; xe = BATTERY_FRAME_START_X + 2;
				ys = ye = BATTERY_FRAME_END_Y - 1;
				UG_DrawLine(xs, ys, xe, ye, BATTERY_FRAME_COLOR);

				obj->state &= ~OBJ_STATE_REDRAW;
			}
			// need to update battery level
			renderBatteryLevel(atoi(MPe_data_param[Param_BatteryChargePercentage]));
		}
		else
		{
			// clear the full symbol area
			xs = BATTERY_FRAME_START_X; xe = BATTERY_FRAME_END_X;
			ys = BATTERY_FRAME_START_Y; ye = BATTERY_FRAME_END_Y;
			UG_FillFrame(xs, ys, xe, ye, C_BLACK);
			obj->state &= ~OBJ_STATE_REDRAW;
		}

		obj->state &= ~OBJ_STATE_UPDATE;
	}
}
