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

#ifndef SCREEN_H_
#define SCREEN_H_

#include "ugui.h"

#define TBL_SIZE(tbl)			    (sizeof(tbl)/sizeof(tbl[0]))

#define FIELD_READONLY_STRING(txt, fnt, alg) { .str = txt, .font = fnt, .align = alg }
#define FIELD_EDITABLE(txt, value, vmin, vmax) { .label = txt, .val = value, .min = vmin, .max = vmax }
#define FIELD_EDITABLE_CUSTOM_STEPS(txt, value, vmin, vmax, sStep, mStep, fStep) { .label = txt, .val = value, .min = vmin, .max = vmax, .slowStep = sStep, .mediumStep = mStep, .fastStep = fStep }
#define FIELD_SELECTION(text, id) { .txt = text, .fieldId = id }
#define FIELD_SLIST(fnt, alg, offset, max, color, fields) { .font = fnt, .align = alg, .y_offset = offset, .maxPosition = max, .frameColor = color, .sel = fields }
#define FIELD_ELIST(fnt, offset, max, color, fields, rows) { .font = fnt, .y_offset = offset, .maxPosition = max, .frameColor = color, .edt = fields, .n_rows = rows }

typedef enum {
	ONOFF_CLICK,
	ONOFF_LONG_CLICK,
	UP_CLICK,
	UP_LONG_CLICK,
	DOWN_CLICK,
	DOWN_LONG_CLICK,
	UPDOWN_LONG_CLICK,
	ONOFFDOWN_LONG_CLICK,
	ONOFFUP_LONG_CLICK
} events_t;
typedef enum {
	ONE_CLICK,
	SLOW_REPEAT,
	MEDIUM_REPEAT,
	FAST_REPEAT
} click_t;

void screen_init(void);
void screen_run(uint32_t tick);
UG_WINDOW* screen_getCurrentScreen(void);
void screen_changeWindow(UG_WINDOW* wnd);
void screen_setEvent(events_t event, click_t click);
void screen_updateData(uint8_t paramNr);
void screen_updateSiImpUnit(void);
void screen_updateTempUnit(void);

#endif /* SCREEN_H_ */
