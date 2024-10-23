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

#ifndef __AVR_ATMEGA_H__
#define __AVR_ATMEGA_H__
/****************************************************************************
 *
 *  - DESCRIPCION: La idea es intentar desvincular las aplicaciones del bare
 *	metal. Para ello necesito crear 2 cosas:
 *	    1. Traductores: acaban en _basic.h
 *	       Si la aplicación necesita usar características especiales del
 *	       microcontrolador llamará a estas funciones directamente.
 *
 *	    2. Dispositivos genéricos.
 *	       Sería la capa del microcontrolador. 
 *
 *	Si todos los microcontroladores ATMEGA... tienen la misma estructura
 *	parece lógico agruparlos dentro del mismo namespace `atmega`. Si
 *	resulta que esto no es cierto más adelante habrá que separarlo
 *	descomponiendo en `atmega16...`, `atmega32...`.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    03/12/2022 Escrito
 *
 ****************************************************************************/
// Cosas genéricas a todas las familias de avrs
// --------------------------------------------
#include <mega_private.h>

// Configuración del micro
// -----------------------
#include <mega_cfg.h>

// Micro
// -----
#include <mega_micro.h>

// Traductores
// -----------
#include <mega_UART_basic.h>
#include <mega_SPI_basic.h>
#include <mega_TWI_basic.h>

#include <mega_timer0_basic.h>
#include <mega_timer1_basic.h>
#include <mega_timer2_basic.h>

#include <mega_ADC_basic.h>

#include <mega_interrupt.h>
#include <mega_memory.h>

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

// TODO: mirar qué depende del micro realmente y qué se basa en un UART
// genérico. Mover a `mcu` lo que sea genérico.
#include <mega_UART.h>
#include <mega_UART_iostream.h>

// Built-in devices for atmega
#include <mega_atmega_dev.h>

#endif


