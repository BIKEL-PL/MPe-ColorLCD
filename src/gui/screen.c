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
#include "screen.h"
#include "startScreen.h"
#include "mainScreen.h"
#include "mainScreen2.h"
#include "mainScreenCommon.h"
#include "configScreen.h"
#include "statScreen1.h"
#include "statScreen2.h"
#include "data.h"
#include "lcd.h"
#include "rtc.h"
#include <stdio.h>

#define CLOCK_UPDATE_INTERVAL	1000	//ms
#define START_SCREEN_DURATION	4000	//ms

static UG_WINDOW* currentScreen = NULL;
static UG_MESSAGE eventMsg;
static volatile uint32_t screen_clockUpdateTimer = 0;
static bool screen_startScreenVisible = true;

void screen_updateTimer(void);

void screen_init(void)
{
	startScreen_init();
	mainScreenCommon_init();
	mainScreen_init();
	mainScreen2_init();
	configScreen_init();
	statScreen1_init();
	statScreen2_init();

	currentScreen = &startScreen_window;
	UG_WindowShow( currentScreen );
	eventMsg.type = MSG_TYPE_OBJECT;
	eventMsg.id = OBJ_TYPE_BUTTON;
	UG_Update();
}

void screen_run(uint32_t tick)
{
	if (tick > screen_clockUpdateTimer + CLOCK_UPDATE_INTERVAL)
	{
		screen_clockUpdateTimer = tick;
		screen_updateTimer();
	}
	if (screen_startScreenVisible)
	{
		if (tick >= START_SCREEN_DURATION)
		{
			screen_startScreenVisible = false;
			screen_changeWindow(&mainScreen_window);
		}
	}
	UG_Update();
}

UG_WINDOW* screen_getCurrentScreen(void)
{
	return currentScreen;
}
void screen_changeWindow(UG_WINDOW* wnd)
{
	currentScreen = wnd;
	UG_WindowShow( currentScreen );
}

void screen_setEvent(events_t event, click_t click)
{
	if (event == UPDOWN_LONG_CLICK)
	{
		// przelaczanie ekranow
		if (currentScreen == &mainScreen_window
				|| currentScreen == &mainScreen2_window)
		{
			screen_changeWindow(&statScreen1_window);
		}
		else if (currentScreen == &statScreen1_window
				|| currentScreen == &statScreen2_window)
		{
			screen_changeWindow(&configScreen_window);
		}
	}
	else if (event == ONOFF_LONG_CLICK
			&& click == ONE_CLICK)
	{
		// powrot do ekranu glownego
		if (currentScreen == &mainScreen_window
				|| currentScreen == &mainScreen2_window)
		{
			lcd_power_off();
		}
		else if (currentScreen == &statScreen1_window
				|| currentScreen == &statScreen2_window)
		{
			screen_changeWindow(&mainScreen_window);
		}
		else if (currentScreen == &configScreen_window)
		{
			eventMsg.event = event;
			eventMsg.click = click;
			if (currentScreen) currentScreen->cb(&eventMsg);
		}
	}
	else
	{
		//wywolanie funkcji callback danego ekranu
		eventMsg.event = event;
		eventMsg.click = click;
		if (currentScreen) currentScreen->cb(&eventMsg);
	}

	lcd_setUserActivity();
}

void screen_updateData(uint8_t paramNr)
{
	mainScreen2_updateData(paramNr);

	switch (paramNr)
	{
	case Param_DailyDistance:
		mainScreen_update(dailyDistanceField);
		statScreen1_update(stat1Menu_dailyFieldVal);
		break;
	case Param_DistanceToGo:
		mainScreen_update(distanceToGoField);
		statScreen1_update(stat1Menu_toGoFieldVal);
		break;
	case Param_Speed:
		mainScreen_updateSpeed();
		break;
	case Param_CurrentPower:
		mainScreen_update(currentPowerField);
		statScreen1_update(stat1Menu_powerFieldVal);
		break;
	case Param_BatteryChargePercentage:
		mainScreen_update(batteryChargePercentageField);
		mainScreen2_update(batteryChargePercentageField2);
		mainScreen_update(batteryField);
		mainScreen2_update(batteryField2);
		statScreen2_update(stat2Menu_remainedFieldVal);
		break;
	case Param_Temperature1:
		mainScreen_update(temperature1Field);
		statScreen2_update(stat2Menu_temp1FieldVal);
		break;
	case Param_Temperature2:
		mainScreen_update(temperature2Field);
		statScreen2_update(stat2Menu_temp2FieldVal);
		break;
	case Param_AssistMode:
		mainScreen_updateAssistMode();
		mainScreen_update(dynamicDrawings);
		mainScreen2_update(dynamicDrawings2);
		mainScreen_update(assistModeField);
		break;
	case Param_TotalDistance:
		statScreen1_update(stat1Menu_totalFieldVal);
		break;
	case Param_AverageSpeed:
		statScreen1_update(stat1Menu_averageFieldVal);
		break;
	case Param_MaxSpeed:
		statScreen1_update(stat1Menu_maxFieldVal);
		break;
	case Param_TimeInMotion:
		statScreen1_update(stat1Menu_inMotionFieldVal);
		mainScreenCommon_updateClock(0,0);  //dodane aby zastapic zegar czasem w ruchu
		mainScreen_update(clockField);      //dodane aby zastapic zegar czasem w ruchu
		mainScreen2_update(clockField2);    //dodane aby zastapic zegar czasem w ruchu
		break;
	case Param_BatteryVoltage:
		mainScreen_update(batteryVoltageField);
		mainScreen2_update(batteryVoltageField2);
		statScreen1_update(stat1Menu_voltageFieldVal);
		break;
	case Param_Current:
		statScreen1_update(stat1Menu_currentFieldVal);
		configScreen_update(configScreen_currentTB);
		break;
	case Param_MaxCurrent:
		statScreen1_update(stat1Menu_maxCurrentFieldVal);
		break;
	case Param_MaxPower:
		statScreen1_update(stat1Menu_maxPowerFieldVal);
		break;
	case Param_EnergyConsumptionPerKilometer:
		statScreen2_update(stat2Menu_energyConsumptionFieldVal);
		break;
	case Param_BatteryCapacity:
		statScreen2_update(stat2Menu_capacityFieldVal);
		break;
	case Param_CurrentBatteryConsumption:
		statScreen2_update(stat2Menu_consumptionFieldVal);
		break;
	case Param_WhUsed:
		statScreen2_update(stat2Menu_consumptionWhFieldVal);
		break;
	case Param_NumberOfChargeCycles:
		statScreen2_update(stat2Menu_numberOfChargeCyclesFieldVal);
		break;
	case Param_BrakeActivity:
		mainScreen_updateImg(brakeImgField, MPe_data_param[Param_BrakeActivity][0] == '1' ? true : false);
		mainScreen_update(brakeImgField);
		break;
	case Param_CruiseControlActivity:
		mainScreen_updateImg(cruisecontrolImgField, MPe_data_param[Param_CruiseControlActivity][0] == '1' ? true : false);
		mainScreen_update(cruisecontrolImgField);
		break;
	case Param_Cadence:
		statScreen2_update(stat2Menu_cadenceFieldVal);
		break;
	case Param_ThrottleIn:
		statScreen2_update(stat2Menu_throttleInFieldVal);
		break;
	case Param_TorqueSensorADC:
			statScreen2_update(stat2Menu_torqueADCFieldVal);
			break;
	case Param_WeightOnPedal:
			statScreen2_update(stat2Menu_torqueKGFFieldVal);
			break;
	case Param_SoftwareVersion:
		statScreen2_update(stat2Menu_softwareVersionFieldVal);
		break;
	case Param_legalIllegal:
		break;
	case Param_Warning:
		mainScreen_updateImg(warningImgField, MPe_data_param[Param_Warning][0] == '1' ? true : false);
		mainScreen_update(warningImgField);
		break;
	default:
		break;
	}
}
void screen_updateTimer(void)
{
	static uint8_t hour = 0, min = 0;
	bool isUpdateRequired = false;
	rtc_time_t *p_rtc_time = rtc_get_time();
	if (p_rtc_time->ui8_hours != hour)
	{
		hour = p_rtc_time->ui8_hours;
		isUpdateRequired = true;
	}
	if (p_rtc_time->ui8_minutes != min)
	{
		min = p_rtc_time->ui8_minutes;
		isUpdateRequired = true;
	}
	if (isUpdateRequired)
	{
		mainScreenCommon_updateClock(hour, min);
		rtc_updateClock(hour, min);
		mainScreen_update(clockField);
		mainScreen2_update(clockField2);
	}
}
void screen_updateSiImpUnit(void)
{
	mainScreen_updateSiImpUnit();
	mainScreen_update(speedUnitField);

	statScreen1_updateSiImpUnit();
	statScreen1_update(stat1Menu_averageFieldUnit);
	statScreen1_update(stat1Menu_maxFieldUnit);
	statScreen1_update(stat1Menu_totalFieldUnit);
	statScreen1_update(stat1Menu_dailyFieldUnit);
	statScreen1_update(stat1Menu_toGoFieldUnit);

	statScreen2_updateSiImpUnit();
	statScreen2_update(stat2Menu_energyConsumptionFieldUnit);
}
void screen_updateTempUnit(void)
{
	statScreen2_updateTempUnit();
	statScreen2_update(stat2Menu_temp1FieldUnit);
	statScreen2_update(stat2Menu_temp2FieldUnit);
}
