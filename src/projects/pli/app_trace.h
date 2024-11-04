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

#ifndef __APP_TRACE_H__
#define __APP_TRACE_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Depuremos la calculadora
 *    En caso de estar definida TRACES_ON se va llevando un log de lo que va
 *    haciendo la calculadora vía UART.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    07/01/2022 Escrito
 *
 ****************************************************************************/
#ifdef TRACES_ON
#include <mega.h>

inline void init_traces()
{
    atmega::UART_iostream uart;
    atmega::basic_cfg(uart);
    uart.on();

    uart << "\nTrazas on\n"
	    "---------\n";
}


struct Trace{
    template <typename T>
    Trace& operator<<(const T& a) 
    { 
	atmega::UART_iostream uart;
	uart << a;
	return *this; 
    }

};


#else

struct Trace{
    template <typename T>
    Trace& operator<<(const T& a) { return *this; }

};

inline void init_traces() {}

#endif

extern Trace ctrace;




#endif


