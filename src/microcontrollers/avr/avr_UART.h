// Copyright (C) 2019-2020 Manuel Perez 
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

#ifndef __AVR_UART_H__
#define __AVR_UART_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Driver del UART.
 *
 *	Recordar:
 *	    1. avr_UART_basic.h
 *	       Aquí escribimos el traductor `UART_basic` del UART. Se limita a
 *	       traducir la datasheet. NO añade funcionalidad.
 *
 *	    2. avr_UART.h
 *	       Este es el driver del UART. Se trata de una clase `UART`o
 *	       funciones basadas en el traductor que simplifican su uso.
 *
 *	    3. avr_UART_iostream.h
 *	       Concebimos el UART como un iostream. 
 *
 * HISTORIA
 *    Manuel Perez
 *    17/09/2023 Reestructurado
 *
 ****************************************************************************/

#include "avr_UART_basic.h"
#include "avr_cfg.h"	// clock_frequency_in_hz

namespace avr_{
template <uint32_t baud_rate = 9600u,
	  uint32_t f_clock   = clock_frequency_in_hz,
	  uint32_t max_error = 2>
void UART_basic_cfg()
{                                
    using UART = UART_basic;

    UART::baud_speed<f_clock, baud_rate, max_error>();

    UART::parity_mode_disabled();

    UART::one_stop_bit();
    UART::character_size_8();
}

// Como de momento solo tengo esta función de driver la defino como función.
// Con todo es más práctico meterlo en una clase `UART` hija de `UART_basic`
// para poderla pasar como parámetro a UART_iostream.
// Hace un flush del flujo (fundamental llamar a esta función antes de dormir
// el microcontrolador. En caso de no llamarla se genera basura)
int UART_flush(uint16_t time_out_ms);


}// namespace
 


#endif

