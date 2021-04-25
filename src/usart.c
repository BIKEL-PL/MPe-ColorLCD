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

#include <string.h>
#include "usart.h"
#include "stm32f10x.h"
#include "pins.h"
#include "stm32f10x_usart.h"
#include "main.h"
#include "data.h"
#include "timers.h"

#define MPE_MAX_DATA_RX				((MPE_MAX_PARAM_LENGTH + 1) * ConfigParam_NumberOfParam + 4)
#define MPE_MAX_SILENT_TIME			(50)	// ms
#define MPE_END_TRANSMISSION_SIGN	('\n')

uint8_t MPe_rxBuffer[MPE_MAX_DATA_RX];
volatile FlagStatus MPe_rxPackageFlag = RESET;

uint8_t MPe_txBuffer[MPE_MAX_DATA_TX];
uint8_t MPe_txCounter = 0, MPe_txSize = 0;

void usart_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// enable GPIO clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);

	// USART pins
	GPIO_InitStructure.GPIO_Pin = USART1_RX__PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART1__PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART1_TX__PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(USART1__PORT, &GPIO_InitStructure);

	USART_DeInit(USART1);
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	// enable the USART Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =
			USART1_INTERRUPT_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	USART_ClearITPendingBit(USART1, USART_IT_TC);

	USART_ClearFlag(USART1, USART_FLAG_TC);

	// enable the USART
	USART_Cmd(USART1, ENABLE);

	// enable USART Receive and Transmit interrupts
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
}

// USART1 Tx and Rx interrupt handler.
void USART1_IRQHandler()
{
	uint8_t data;
	uint32_t curTicks;
	static uint8_t rxBuffer[MPE_MAX_DATA_RX];
	static uint16_t rxCounter = 0;
	static uint32_t lastRxTick = 0;

	// The interrupt may be from Tx, Rx, or both.
	if (USART_GetITStatus(USART1, USART_IT_ORE) == SET)
	{
		USART_ReceiveData(USART1); // get ride of this interrupt flag
		return;
	}
	else if (USART_GetITStatus(USART1, USART_IT_TXE) == SET)
	{
		USART_SendData(USART1, MPe_txBuffer[MPe_txCounter++]);

		if (MPe_txCounter >= MPe_txSize)
		{
			// Disable the USARTy Transmit interrupt
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
		}
	}
	else if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
	    // receive byte
		data = (uint8_t) USART1->DR;
		curTicks = get_time_base_counter_1ms();

		// if too much time has passed since the last byte, the transmission starts from 0
		if(curTicks - lastRxTick >= MPE_MAX_SILENT_TIME)
		{
			rxCounter = 0;
			memset(rxBuffer, 0, MPE_MAX_DATA_RX);
		}
		lastRxTick = curTicks;

		// see if is the last byte of the package
		if (data == MPE_END_TRANSMISSION_SIGN)
		{
			// copy to the other buffer only if we processed already the last package
			if (MPe_rxPackageFlag != SET)
			{
				MPe_rxPackageFlag = SET;

				// store the received data to rx_buffer
				memcpy(MPe_rxBuffer, rxBuffer, MPE_MAX_DATA_RX);
			}
			rxCounter = 0;
			memset(rxBuffer, 0, MPE_MAX_DATA_RX);
		}
		else
		{
			if (rxCounter < MPE_MAX_DATA_RX)
			{
				rxBuffer[rxCounter++] = data;
			}
		}
	}
}

uint8_t* usart_get_rx_buffer(void)
{
	return MPe_rxBuffer;
}

uint8_t usart_received_package(void)
{
	return (uint8_t) MPe_rxPackageFlag;
}

void usart_reset_received_package(void)
{
	MPe_rxPackageFlag = RESET;
}

uint8_t* usart_get_tx_buffer(void)
{
	return MPe_txBuffer;
}
void usart_send_tx_buffer(uint8_t size)
{
	if (size > MPE_MAX_DATA_TX) size = MPE_MAX_DATA_TX;
	MPe_txSize = size;
	MPe_txCounter = 0;
	if (size) USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}
/**
 * @brief Returns pointer to RX buffer ready for parsing or NULL
 */
const uint8_t* usart_get_rx_buffer_rdy(void)
{
	if(!usart_received_package()) {
		return NULL;
	}

	uint8_t *r = usart_get_rx_buffer();
	usart_reset_received_package();
	return r;
}
