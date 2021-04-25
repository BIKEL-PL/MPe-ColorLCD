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

#ifndef _USART_H_
#define _USART_H_

#include "stdio.h"

#define MPE_MAX_DATA_TX		20

void usart_init(void);
const uint8_t* usart_get_rx_buffer_rdy(void);
uint8_t* usart_get_rx_buffer(void);
uint8_t usart_received_package(void);
void usart_reset_received_package(void);

uint8_t* usart_get_tx_buffer(void);
void usart_send_tx_buffer(uint8_t size);

#endif
