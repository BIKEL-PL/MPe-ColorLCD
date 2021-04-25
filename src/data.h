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

#ifndef DATA_H_
#define DATA_H_

#include "stm32f10x.h"

#define MPE_MAX_PARAM_LENGTH	6

enum Params
{
	Param_Speed,
	Param_DistanceToGo,
	Param_BatteryChargePercentage,
	Param_DailyDistance,
	Param_CurrentPower,
	Param_Temperature1,
	Param_AssistMode,
	Param_TotalDistance,
	Param_AverageSpeed,
	Param_MaxSpeed,
	Param_TimeInMotion,
	Param_BatteryVoltage,
	Param_Current,
	Param_MaxCurrent,
	Param_MaxPower,
	Param_EnergyConsumptionPerKilometer,
	Param_BatteryCapacity,
	Param_CurrentBatteryConsumption,
	Param_Temperature2,
	Param_NumberOfChargeCycles,
	Param_BrakeActivity,
	Param_CruiseControlActivity,
	Param_SoftwareVersion,
	Param_legalIllegal,
	Param_WhUsed,
	Param_Warning,
	Param_Cadence,
	Param_ThrottleIn,
	Param_TorqueSensorADC,
	Param_WeightOnPedal,

	Param_NumberOfParam
};

enum ConfigParams
{
	ConfigParam_assistMode,
	ConfigParam_batteryCapacityAh,
	ConfigParam_batteryCapacityWh,
	ConfigParam_lowVoltageCutoff,
	ConfigParam_fullBatteryVoltage,
	ConfigParam_currentMv_a,
	ConfigParam_currentDirection,
	ConfigParam_voltageDivider,
	ConfigParam_currentSensorOK,
	ConfigParam_currentProtection,
	ConfigParam_notUse_2,
	ConfigParam_notUse_3,
	ConfigParam_notUse_4,
	ConfigParam_TOTmin,
	ConfigParam_TOTmax,
	ConfigParam_TINmin,
	ConfigParam_TINmax,
	ConfigParam_throttleReset,
	ConfigParam_throttleSafeVoltage,
	ConfigParam_notUse_6,
	ConfigParam_notUse_7,
	ConfigParam_notUse_8,
	ConfigParam_notUse_9,
	ConfigParam_notUse_10,
	ConfigParam_notUse_11,
	ConfigParam_metricImperial,
	ConfigParam_perimeter,
	ConfigParam_MotorMagnets,
	ConfigParam_GearRatio,
	ConfigParam_notUse_12,
	ConfigParam_notUse_13,
	ConfigParam_notUse_14,
	ConfigParam_notUse_15,
	ConfigParam_notUse_16,
	ConfigParam_notUse_17,
	ConfigParam_brakeHiLo,
	ConfigParam_notUse_18,
	ConfigParam_notUse_19,
	ConfigParam_notUse_20,
	ConfigParam_notUse_21,
	ConfigParam_temperatureUnit,
	ConfigParam_temp1Type,
	ConfigParam_temp2Type,
	ConfigParam_temp1Overheat,
	ConfigParam_temp2Overheat,
	ConfigParam_notUse_22,
	ConfigParam_notUse_23,
	ConfigParam_notUse_24,
	ConfigParam_notUse_25,
	ConfigParam_notUse_26,
	ConfigParam_POWERKP,
	ConfigParam_POWERKI,
	ConfigParam_POWERKD,
	ConfigParam_PLOW,
	ConfigParam_ILOW,
	ConfigParam_DLOW,
	ConfigParam_PID_Low_Threshold,
	ConfigParam_notUse_28,
	ConfigParam_notUse_29,
	ConfigParam_SpeedFactorMin,
	ConfigParam_PIDPWMMax,
	ConfigParam_SpeedFactorRampUp,
	ConfigParam_CruiseControlPowerMin,
	ConfigParam_CruiseControlPowerMax,
	ConfigParam_CruiseControlPowerRampUp,
	ConfigParam_CruiseControlMaxSpeed,
	ConfigParam_notUse_32,
	ConfigParam_notUse_33,
	ConfigParam_notUse_34,
	ConfigParam_notUse_35,
	ConfigParam_AutoLegal,
	ConfigParam_LegalLimitOnOff,
	ConfigParam_LegalLimitSpeed,
	ConfigParam_LegalLimitPower,
	ConfigParam_PASMagnets,
	ConfigParam_PowerLimitPAS_1,
	ConfigParam_PowerLimitPAS_2,
	ConfigParam_PowerLimitPAS_3,
	ConfigParam_PowerLimitPAS_4,
	ConfigParam_PowerLimitPAS_5,
	ConfigParam_SpeedLimitPAS_1,
	ConfigParam_SpeedLimitPAS_2,
	ConfigParam_SpeedLimitPAS_3,
	ConfigParam_SpeedLimitPAS_4,
	ConfigParam_SpeedLimitPAS_5,
	ConfigParam_MinSpeedPAS_1,
	ConfigParam_MinSpeedPAS_2,
	ConfigParam_MinSpeedPAS_3,
	ConfigParam_MinSpeedPAS_4,
	ConfigParam_MinSpeedPAS_5,
	ConfigParam_CadenceMinPAS_1,
	ConfigParam_CadenceMinPAS_2,
	ConfigParam_CadenceMinPAS_3,
	ConfigParam_CadenceMinPAS_4,
	ConfigParam_CadenceMinPAS_5,
	ConfigParam_CadenceMaxPAS_1,
	ConfigParam_CadenceMaxPAS_2,
	ConfigParam_CadenceMaxPAS_3,
	ConfigParam_CadenceMaxPAS_4,
	ConfigParam_CadenceMaxPAS_5,
	ConfigParam_RampUpPAS_1,
	ConfigParam_RampUpPAS_2,
	ConfigParam_RampUpPAS_3,
	ConfigParam_RampUpPAS_4,
	ConfigParam_RampUpPAS_5,
	ConfigParam_BoostPowerPAS_1,
	ConfigParam_BoostPowerPAS_2,
	ConfigParam_BoostPowerPAS_3,
	ConfigParam_BoostPowerPAS_4,
	ConfigParam_BoostPowerPAS_5,
	ConfigParam_BoostTimePAS_1,
	ConfigParam_BoostTimePAS_2,
	ConfigParam_BoostTimePAS_3,
	ConfigParam_BoostTimePAS_4,
	ConfigParam_BoostTimePAS_5,
	ConfigParam_BoostSpeedPAS_1,
	ConfigParam_BoostSpeedPAS_2,
	ConfigParam_BoostSpeedPAS_3,
	ConfigParam_BoostSpeedPAS_4,
	ConfigParam_BoostSpeedPAS_5,
	ConfigParam_BoostRampUpPAS,
	ConfigParam_CadenceRefreshTime,
	ConfigParam_EnableTorqueSensor,
	ConfigParam_WeightOnPedalStartup,
	ConfigParam_TorqueSensorADCMin,
	ConfigParam_TorqueSensorADCMax,
	ConfigParam_TorqueSensorkgFMax,
	ConfigParam_notUse_58,
	ConfigParam_notUse_59,
	ConfigParam_notUse_60,
	ConfigParam_ThrottlePowerLimit_1,
	ConfigParam_ThrottlePowerLimit_2,
	ConfigParam_ThrottlePowerLimit_3,
	ConfigParam_ThrottlePowerLimit_4,
	ConfigParam_ThrottlePowerLimit_5,
	ConfigParam_ThrottleMode_1,
	ConfigParam_ThrottleMode_2,
	ConfigParam_ThrottleMode_3,
	ConfigParam_ThrottleMode_4,
	ConfigParam_ThrottleMode_5,
	ConfigParam_ThrottleRampUp_1,
	ConfigParam_ThrottleRampUp_2,
	ConfigParam_ThrottleRampUp_3,
	ConfigParam_ThrottleRampUp_4,
	ConfigParam_ThrottleRampUp_5,
	ConfigParam_notUse_61,
	ConfigParam_notUse_62,
	ConfigParam_notUse_63,
	ConfigParam_notUse_64,
	ConfigParam_notUse_65,
	ConfigParam_notUse_66,
	ConfigParam_notUse_67,
	ConfigParam_notUse_68,
	ConfigParam_notUse_69,
	ConfigParam_notUse_70,
	ConfigParam_notUse_71,

	ConfigParam_NumberOfParam
};

extern char MPe_data_param[Param_NumberOfParam][MPE_MAX_PARAM_LENGTH];
extern uint16_t MPe_data_configParamValue[ConfigParam_NumberOfParam];
extern uint16_t data_presetDistance_edit, data_presetBattChargeNo_edit;

void data_init(void);
void data_run(uint32_t ticksMs);
void data_synchro(void);
void data_allDataRequest(void);
void data_resetCurrValuesRequest(void);
void data_resetBatteryIndicatorRequest(void);
void data_resetCurrentSensorRequest(void);
void data_cruiseControlSwitchOnRequest(void);
void data_cruiseControlSwitchOffRequest(void);
void data_cruiseControlAccelerationRequest(void);
void data_cruiseControlSlowDownRequest(void);
void data_lightToggleRequest(void);

#endif /* MPE_DATA_H_ */
