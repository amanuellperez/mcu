// Copyright (C) 2022 Manuel Perez 
//           mail: <manuel2perez@proton.me>
//           https://github.com/amanuellperez/mcu
//
// This file is part of the MCU++ Library.
//
// MCU++ Library is a free library: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#ifndef __MEGA_H__
#define __MEGA_H__
/****************************************************************************
 *
 *  DESCRIPCION
 *	Microcontroladores de la familia mega
 *
 *	De momento solo es para el atmega328p. 
 *
 *
 *  HISTORIA:
 *    Manuel Perez
 *    03/12/2022 Escrito
 *    04/11/2024 avr_atmega.h -> mega.h
 *
 ****************************************************************************/
// Cosas genéricas a todas las familias de avrs
// --------------------------------------------
#include <mega_import_avr.h>

// Configuración del micro
// -----------------------
#include <mega_cfg.h>

// Micro
// -----
#include <mega_micro.h>

// Traductores
// -----------
#include <mega_USART_basic.h>
#include <mega_SPI_basic.h>
#include <mega_TWI_basic.h>

#include <mega_timer0_basic.h>
#include <mega_timer1_basic.h>
#include <mega_timer2_basic.h>

#include <mega_ADC_basic.h>

#include <mega_interrupt.h>

#include <mega_pin.h>
#include <mega_port_basic.h>
#include <mega_sleep.h>
#include <mega_watchdog.h>


// Dispositivos genéricos
// ----------------------
#include <mega_SPI.h>

#include <mega_timer0.h>
#include <mega_timer1.h>
#include <mega_timer2.h>

#include <mega_ADC.h>

#include <mega_UART.h>
#include <mcu_UART_iostream.h>	// es comodo meterlo aqui

// Built-in devices for atmega
#include <mega_atmega_dev.h>

#endif


