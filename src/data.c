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
#include "data.h"
#include "usart.h"
#include "cmd.h"
#include "address.h"
#include "assistMode.h"
#include "screen.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MPE_SEND_INTERVAL	200
#define MPE_MAX_WAITING_FOR_REQUEST		3000
#define MPE_DISPLAY_DATA		1
#define MPE_CONFIG_DATA			2
#define MPE_ONE_PARAM_DATA		3
#define MPE_MAX_EDITING_TIME		3000

typedef enum {ConfigParamState_idle, ConfigParamState_sendNewValue, ConfigParamState_sendValueRequest} ConfigParamStates_t;

static const char MPe_dataHeadline[] = {"MPe"};
static const char MPe_configDataHeadline[] = {"ALL"};
static const char MPe_oneParamDataHeadline[] = {"CFG"};

// DISPLAY PARAM
char MPe_data_param[Param_NumberOfParam][MPE_MAX_PARAM_LENGTH];
// CONFIG PARAM
char MPe_data_configParam[ConfigParam_NumberOfParam][MPE_MAX_PARAM_LENGTH];
uint16_t MPe_data_configParamValue[ConfigParam_NumberOfParam];
static uint16_t MPe_data_configParamValue_old[ConfigParam_NumberOfParam];
static ConfigParamStates_t MPe_data_configParamValue_state[ConfigParam_NumberOfParam];
static uint32_t MPe_data_configParamValue_sendTime[ConfigParam_NumberOfParam];
static uint32_t MPe_data_lastSendTime;

static BOOL MPe_data_sendAllDataRequest, MPe_data_sendResetCurrValuesRequest,
			MPe_data_sendResetBatteryIndicatorRequest, MPe_data_sendResetCurrentSensorRequest,
			MPe_data_sendCruiseControlSwitchOnRequest, MPe_data_sendCruiseControlSwitchOffRequest,
			MPe_data_sendCruiseControlSlowDownRequest, MPe_data_sendCruiseControlAccelerationRequest,
			MPe_data_sendPresetBattChargeNoRequest, MPe_data_sendPresetDistanceRequest,
			MPe_data_initialization,MPe_data_sendLightToggleRequest;

static uint16_t data_presetDistance, data_presetBattChargeNo;
uint16_t data_presetDistance_edit, data_presetBattChargeNo_edit;

static uint8_t MPe_data_countCharInStr(const uint8_t *s, const char c);

void data_init(void)
{
	MPe_data_sendAllDataRequest = TRUE;
	MPe_data_sendResetCurrValuesRequest = FALSE;
	MPe_data_sendResetBatteryIndicatorRequest = FALSE;
	MPe_data_sendResetCurrentSensorRequest = FALSE;
	MPe_data_sendCruiseControlSwitchOnRequest = FALSE;
	MPe_data_sendCruiseControlSwitchOffRequest = FALSE;
	MPe_data_sendCruiseControlAccelerationRequest = FALSE;
	MPe_data_sendCruiseControlSlowDownRequest = FALSE;
	MPe_data_sendLightToggleRequest = FALSE;
	MPe_data_sendPresetDistanceRequest = FALSE;
	MPe_data_sendPresetBattChargeNoRequest = FALSE;
	MPe_data_initialization = TRUE;
	MPe_data_lastSendTime = 0;

	// initialization data that will be displayed until the display receives data
	for (uint8_t i = 0; i < Param_NumberOfParam; i++)
	{
		strcpy(MPe_data_param[i], "0");
	}
	strcpy(MPe_data_param[Param_DailyDistance], "000");
	strcpy(MPe_data_param[Param_DistanceToGo], "000");
	strcpy(MPe_data_param[Param_Speed], "000");
	strcpy(MPe_data_param[Param_CurrentPower], "00000");
	strcpy(MPe_data_param[Param_BatteryChargePercentage], "000");
	strcpy(MPe_data_param[Param_Temperature1], "000");
	strcpy(MPe_data_param[Param_Temperature2], "000");
}
void data_run(uint32_t ticksMs)
{
	const uint8_t *p_rxBuffer = usart_get_rx_buffer_rdy();

	// checking if new data has been received
	if (p_rxBuffer)
	{
		char buffStr[MPE_MAX_PARAM_LENGTH] = {0};	// temporary buffer
		uint8_t j = 0, paramNr = 0, headline = 1, dataType = 0;
		char addr[MPE_MAX_ADDRESS_LENGTH] = {0}; // temporary buffer
		bool updateSpeed = false;

		while (*p_rxBuffer)
		{
			if (*p_rxBuffer != ';')
			{
				if (j < MPE_MAX_PARAM_LENGTH) buffStr[j++] = (char)*p_rxBuffer;
			}
			else
			{
				if (headline)
				{
					// checking if data headline is equal MPe
					if (!strncmp(buffStr, MPe_dataHeadline, 3))
					{
						// if it is less ';' than expected parameters - reject MSG
						if (MPe_data_countCharInStr(p_rxBuffer + 1, ';') < Param_NumberOfParam) return;
						headline = 0;
						dataType = MPE_DISPLAY_DATA;
					}
					// checking if data headline is equal ALL
					else if (!strncmp(buffStr, MPe_configDataHeadline, 3))
					{
						// if it is less ';' than expected parameters - reject MSG
						if (MPe_data_countCharInStr(p_rxBuffer + 1, ';') < ConfigParam_NumberOfParam) return;
						headline = 0;
						dataType = MPE_CONFIG_DATA;
					}
					// checking if data headline is equal CFG
					else if (!strncmp(buffStr, MPe_oneParamDataHeadline, 3))
					{
						headline = 0;
						dataType = MPE_ONE_PARAM_DATA;
					}
					// data is not equal MPe, CFG and ALL - reject MSG
					if (headline) return;
				}
				else
				{
					if (dataType == MPE_DISPLAY_DATA)
					{
						if (paramNr < Param_NumberOfParam)
						{
							if (strncmp(buffStr, MPe_data_param[paramNr], MPE_MAX_PARAM_LENGTH) || MPe_data_initialization)
							{
								memcpy(MPe_data_param[paramNr], buffStr, MPE_MAX_PARAM_LENGTH);
								if (paramNr == Param_Speed)
								{
									running_pace_updateSpeed();
									updateSpeed = true;
								}
								screen_updateData(paramNr);

								// exception for parameters sent in a permanent data frame
								if (paramNr == Param_legalIllegal)
								{
									memcpy(MPe_data_configParam[ConfigParam_LegalLimitOnOff], buffStr, MPE_MAX_PARAM_LENGTH);
									MPe_data_configParamValue_old[ConfigParam_LegalLimitOnOff] = (uint16_t)atoi(buffStr);
									MPe_data_configParamValue_state[ConfigParam_LegalLimitOnOff] = ConfigParamState_idle;
									assistMode_setLegalMode((BOOL)MPe_data_configParamValue_old[ConfigParam_LegalLimitOnOff]);

									if (MPe_data_initialization)
									{
										MPe_data_configParamValue[ConfigParam_LegalLimitOnOff] = MPe_data_configParamValue_old[ConfigParam_LegalLimitOnOff];
									}
								}
								else if (paramNr == Param_AssistMode)
								{
									memcpy(MPe_data_configParam[ConfigParam_assistMode], buffStr, MPE_MAX_PARAM_LENGTH);
									MPe_data_configParamValue_old[ConfigParam_assistMode] = (uint16_t)atoi(buffStr);
									MPe_data_configParamValue_state[ConfigParam_assistMode] = ConfigParamState_idle;

									if (MPe_data_initialization)
									{
										MPe_data_configParamValue[ConfigParam_assistMode] = MPe_data_configParamValue_old[ConfigParam_assistMode];
										assistMode_updateValue();
									}
								}
								else if (paramNr == Param_TotalDistance)
								{
									data_presetDistance_edit = data_presetDistance = (uint16_t)atoi(buffStr);
								}
								else if (paramNr == Param_NumberOfChargeCycles)
								{
									data_presetBattChargeNo_edit = data_presetBattChargeNo = (uint16_t)atoi(buffStr);
								}
							}
							paramNr++;
						}
					}
					else if (dataType == MPE_CONFIG_DATA)
					{
						if (paramNr < ConfigParam_NumberOfParam)
						{
							if (strncmp(buffStr, MPe_data_configParam[paramNr], MPE_MAX_PARAM_LENGTH))
							{
								memcpy(MPe_data_configParam[paramNr], buffStr, MPE_MAX_PARAM_LENGTH);
								MPe_data_configParamValue[paramNr] = (uint16_t)atoi(buffStr);
								MPe_data_configParamValue_old[paramNr] = MPe_data_configParamValue[paramNr];

								if (paramNr == ConfigParam_metricImperial) screen_updateSiImpUnit();
								else if (paramNr == ConfigParam_temperatureUnit) screen_updateTempUnit();
							}
							paramNr++;
						}
					}
					else if (dataType == MPE_ONE_PARAM_DATA)
					{
						if (paramNr == 0)
						{ // address target parameter
							memcpy(addr, buffStr, MPE_MAX_ADDRESS_LENGTH);
						}
						else if (paramNr == 1)
						{ // value target parameter
							uint8_t configParamNr = address_getConfigParamNr(addr);
							if (configParamNr == MPE_ADDRESS_ERROR) return;
							else
							{
								memcpy(MPe_data_configParam[configParamNr], buffStr, MPE_MAX_PARAM_LENGTH);
								MPe_data_configParamValue[configParamNr] = (uint16_t)atoi(buffStr);
								MPe_data_configParamValue_old[configParamNr] = MPe_data_configParamValue[configParamNr];
								MPe_data_configParamValue_state[configParamNr] = ConfigParamState_idle;

								if (configParamNr == ConfigParam_metricImperial) screen_updateSiImpUnit();
								else if (configParamNr == ConfigParam_temperatureUnit) screen_updateTempUnit();
							}
						}
						paramNr++;
					}
					else return;
				}
				memset(buffStr, 0, MPE_MAX_PARAM_LENGTH);
				j = 0;
			}
			p_rxBuffer++;
		}

		if (MPe_data_initialization && dataType == MPE_DISPLAY_DATA) MPe_data_initialization = FALSE;
		if (dataType == MPE_DISPLAY_DATA && updateSpeed == false) running_pace_updateSpeed();
	}

	// checking if all configuration data is requested
	if (MPe_data_sendAllDataRequest == TRUE)
	{
		uint8_t size = (ALL_DATA_REQUEST_SIZE < MPE_MAX_DATA_TX ? ALL_DATA_REQUEST_SIZE : MPE_MAX_DATA_TX) - 1;
		memcpy(usart_get_tx_buffer(), ALL_DATA_REQUEST, size);
		usart_send_tx_buffer(size);
		MPe_data_sendAllDataRequest = FALSE;
	}
	// checking if reset current values is requested
	else if (MPe_data_sendResetCurrValuesRequest == TRUE)
	{
		uint8_t size = (RESET_CURR_VALUES_REQUEST_SIZE < MPE_MAX_DATA_TX ? RESET_CURR_VALUES_REQUEST_SIZE : MPE_MAX_DATA_TX) - 1;
		memcpy(usart_get_tx_buffer(), RESET_CURR_VALUES_REQUEST, size);
		usart_send_tx_buffer(size);
		MPe_data_sendResetCurrValuesRequest = FALSE;
	}
	// checking if reset battery indicator is requested
	else if (MPe_data_sendResetBatteryIndicatorRequest == TRUE)
	{
		uint8_t size = (RESET_BATTERY_INDICATOR_REQUEST_SIZE < MPE_MAX_DATA_TX ? RESET_BATTERY_INDICATOR_REQUEST_SIZE : MPE_MAX_DATA_TX) - 1;
		memcpy(usart_get_tx_buffer(), RESET_BATTERY_INDICATOR_REQUEST, size);
		usart_send_tx_buffer(size);
		MPe_data_sendResetBatteryIndicatorRequest = FALSE;
	}
	// checking if reset current sensor is requested
	else if (MPe_data_sendResetCurrentSensorRequest == TRUE)
	{
		uint8_t size = (RESET_CURRENT_SENSOR_REQUEST_SIZE < MPE_MAX_DATA_TX ? RESET_CURRENT_SENSOR_REQUEST_SIZE : MPE_MAX_DATA_TX) - 1;
		memcpy(usart_get_tx_buffer(), RESET_CURRENT_SENSOR_REQUEST, size);
		usart_send_tx_buffer(size);
		MPe_data_sendResetCurrentSensorRequest = FALSE;
	}
	// checking if switching on cruise control is requested
	else if (MPe_data_sendCruiseControlSwitchOnRequest == TRUE)
	{
		uint8_t size = (CRUISE_CONTROL_SWITCH_ON_REQUEST_SIZE < MPE_MAX_DATA_TX ? CRUISE_CONTROL_SWITCH_ON_REQUEST_SIZE : MPE_MAX_DATA_TX) - 1;
		memcpy(usart_get_tx_buffer(), CRUISE_CONTROL_SWITCH_ON_REQUEST, size);
		usart_send_tx_buffer(size);
		MPe_data_sendCruiseControlSwitchOnRequest = FALSE;
	}
	// checking if switching off cruise control is requested
	else if (MPe_data_sendCruiseControlSwitchOffRequest == TRUE)
	{
		uint8_t size = (CRUISE_CONTROL_SWITCH_OFF_REQUEST_SIZE < MPE_MAX_DATA_TX ? CRUISE_CONTROL_SWITCH_OFF_REQUEST_SIZE : MPE_MAX_DATA_TX) - 1;
		memcpy(usart_get_tx_buffer(), CRUISE_CONTROL_SWITCH_OFF_REQUEST, size);
		usart_send_tx_buffer(size);
		MPe_data_sendCruiseControlSwitchOffRequest = FALSE;
	}
	// checking if acceleration cruise control is requested
	else if (MPe_data_sendCruiseControlAccelerationRequest == TRUE)
	{
		uint8_t size = (CRUISE_CONTROL_ACCELERATION_REQUEST_SIZE < MPE_MAX_DATA_TX ? CRUISE_CONTROL_ACCELERATION_REQUEST_SIZE : MPE_MAX_DATA_TX) - 1;
		memcpy(usart_get_tx_buffer(), CRUISE_CONTROL_ACCELERATION_REQUEST, size);
		usart_send_tx_buffer(size);
		MPe_data_sendCruiseControlAccelerationRequest = FALSE;
	}
	// checking if slow down cruise control is requested
	else if (MPe_data_sendCruiseControlSlowDownRequest == TRUE)
	{
		uint8_t size = (CRUISE_CONTROL_SLOW_DOWN_REQUEST_SIZE < MPE_MAX_DATA_TX ? CRUISE_CONTROL_SLOW_DOWN_REQUEST_SIZE : MPE_MAX_DATA_TX) - 1;
		memcpy(usart_get_tx_buffer(), CRUISE_CONTROL_SLOW_DOWN_REQUEST, size);
		usart_send_tx_buffer(size);
		MPe_data_sendCruiseControlSlowDownRequest = FALSE;
	}
	// checking if light toggle is requested
	else if (MPe_data_sendLightToggleRequest == TRUE)
	{
		uint8_t size = (LIGHT_TOGGLE_REQUEST_SIZE < MPE_MAX_DATA_TX ? LIGHT_TOGGLE_REQUEST_SIZE : MPE_MAX_DATA_TX) - 1;
		memcpy(usart_get_tx_buffer(), LIGHT_TOGGLE_REQUEST, size);
		usart_send_tx_buffer(size);
		MPe_data_sendLightToggleRequest = FALSE;
	}
	// checking if preset distance is requested
	else if (MPe_data_sendPresetDistanceRequest == TRUE)
	{
		char cmd[MPE_MAX_DATA_TX] = {0};
		uint8_t size = snprintf(cmd, sizeof(cmd), "%s %d", PRESET_DISTANCE_REQUEST, data_presetDistance);
		if (size > MPE_MAX_DATA_TX) size = MPE_MAX_DATA_TX;
		memcpy(usart_get_tx_buffer(), cmd, size);
		usart_send_tx_buffer(size);
		MPe_data_sendPresetDistanceRequest = FALSE;
	}
	// checking if preset battery charge number is requested
	else if (MPe_data_sendPresetBattChargeNoRequest == TRUE)
	{
		char cmd[MPE_MAX_DATA_TX] = {0};
		uint8_t size = snprintf(cmd, sizeof(cmd), "%s %d", PRESET_BATT_CHARGE_NO_REQUEST,
				(uint16_t)(data_presetBattChargeNo * MPe_data_configParamValue[ConfigParam_batteryCapacityAh]) / 10);
		if (size > MPE_MAX_DATA_TX) size = MPE_MAX_DATA_TX;
		memcpy(usart_get_tx_buffer(), cmd, size);
		usart_send_tx_buffer(size);
		MPe_data_sendPresetBattChargeNoRequest = FALSE;
	}
	else
	{
		if (ticksMs > MPe_data_lastSendTime + MPE_SEND_INTERVAL)
		{
			MPe_data_lastSendTime = ticksMs;

			// checking if the new parameter value has been sent
			for (uint8_t i = 0; i < ConfigParam_NumberOfParam; i++)
			{
				if (MPe_data_configParamValue_state[i] == ConfigParamState_sendNewValue)
				{
					char cmd[MPE_MAX_DATA_TX] = {0};
					uint8_t size = snprintf(cmd, sizeof(cmd), "%s %s", ONE_PARAM_REQUEST, MPe_address_configParams[i]);
					if (size > MPE_MAX_DATA_TX) size = MPE_MAX_DATA_TX;
					memcpy(usart_get_tx_buffer(), cmd, size);
					usart_send_tx_buffer(size);
					MPe_data_configParamValue_state[i] = ConfigParamState_sendValueRequest;
					MPe_data_configParamValue_sendTime[i] = ticksMs;
					return;
				}
			}

			// checking if the user has set a new value for any of the parameters
			for (uint8_t i = 0; i < ConfigParam_NumberOfParam; i++)
			{
				if (MPe_data_configParamValue_state[i] == ConfigParamState_idle)
				{
					if (MPe_data_configParamValue[i] != MPe_data_configParamValue_old[i])
					{
						char cmd[MPE_MAX_DATA_TX] = {0};
						uint8_t size = snprintf(cmd, sizeof(cmd), "%s %s %d", SAVE_DATA, MPe_address_configParams[i], MPe_data_configParamValue[i]);
						if (size > MPE_MAX_DATA_TX) size = MPE_MAX_DATA_TX;
						memcpy(usart_get_tx_buffer(), cmd, size);
						usart_send_tx_buffer(size);

						if (i == ConfigParam_assistMode || i == ConfigParam_LegalLimitOnOff)
						{
							// exception for parameters received in a permanent data frame
							// we do not send a request for the latest value
							MPe_data_configParamValue_state[i] = ConfigParamState_sendValueRequest;
							MPe_data_configParamValue_sendTime[i] = ticksMs;
						}
						else MPe_data_configParamValue_state[i] = ConfigParamState_sendNewValue;
						break;
					}
				}
			}

			// checking if we received confirmation of setting new values
			for (uint8_t i = 0; i < ConfigParam_NumberOfParam; i++)
			{
				if (MPe_data_configParamValue_state[i] == ConfigParamState_sendValueRequest)
				{
					if (ticksMs > MPe_data_configParamValue_sendTime[i] + MPE_MAX_WAITING_FOR_REQUEST)
					{
						// we have not received confirmation - we are restoring the old values
						MPe_data_configParamValue_state[i] = ConfigParamState_idle;
						MPe_data_configParamValue[i] = MPe_data_configParamValue_old[i];

						if (i == ConfigParam_assistMode) assistMode_updateValue();
						else if (i == ConfigParam_LegalLimitOnOff) assistMode_setLegalMode((BOOL)MPe_data_configParamValue_old[i]);
					}
				}
			}
		}
	}
}

void data_synchro(void)
{
	if (data_presetDistance_edit != data_presetDistance)
	{
		data_presetDistance = data_presetDistance_edit;
		MPe_data_sendPresetDistanceRequest = TRUE;
	}
	if (data_presetBattChargeNo_edit != data_presetBattChargeNo)
	{
		data_presetBattChargeNo = data_presetBattChargeNo_edit;
		MPe_data_sendPresetBattChargeNoRequest = TRUE;
	}
}

void data_allDataRequest(void)
{
	MPe_data_sendAllDataRequest = TRUE;
}
void data_resetCurrValuesRequest(void)
{
	MPe_data_sendResetCurrValuesRequest = TRUE;
}
void data_resetBatteryIndicatorRequest(void)
{
	MPe_data_sendResetBatteryIndicatorRequest = TRUE;
}
void data_resetCurrentSensorRequest(void)
{
	MPe_data_sendResetCurrentSensorRequest = TRUE;
}
void data_cruiseControlSwitchOnRequest(void)
{
	MPe_data_sendCruiseControlSwitchOnRequest = TRUE;
}
void data_cruiseControlSwitchOffRequest(void)
{
	MPe_data_sendCruiseControlSwitchOffRequest = TRUE;
}
void data_cruiseControlAccelerationRequest(void)
{
	MPe_data_sendCruiseControlAccelerationRequest = TRUE;
}
void data_cruiseControlSlowDownRequest(void)
{
	MPe_data_sendCruiseControlSlowDownRequest = TRUE;
}
void data_lightToggleRequest(void)
{
	MPe_data_sendLightToggleRequest = TRUE;
}


static uint8_t MPe_data_countCharInStr(const uint8_t *s, const char c)
{
	uint8_t result = 0;
	while (*s)
	{
		if (*s == c) result++;
		s++;
	}
	return result;
}
