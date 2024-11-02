// Copyright (C) 2024 Manuel Perez 
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

#ifndef __AVR_TIME_H__
#define __AVR_TIME_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Funciones para manejo de tiempos en los AVR
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	02/11/2024 wait_cpu_ticks
 *
 ****************************************************************************/
#ifdef F_CPU
#include "avr_time_fcpu.h"
#endif

#include <concepts>

namespace avr_{

// wait_cpu_ticks
// --------------
// Espera t ticks de CPU aproximadamente.
// (RRR) ¿cómo esperar un tiempo cuando no está definida F_CPU?
//       Una forma es esperar en ticks de F_CPU
//
// La siguiente función, al experimentar, genera el siguiente código asm:
//
//	ldi r24,lo8(123)
//	ldi r25,0
//.L2:
//	sbiw r24, 1	
//	brne .L2
//
// Da la impresión de que si se llama a `wait_cpu_ticks(t)` la función
// realmente espera `2*t + 2` ticks de CPU. Aproximadamente `2*t`
//
// TODO: el compilador elimina la función si se llama con t < 18!!!
//      ¿considera que no le va a dar tiempo a ejecutar nada y por eso la
//      elimina?
template <std::integral Int>
inline constexpr void 
__attribute__((optimize("O0")))	// impide que se optimice la función 
				// (con -Os gcc directamente la elimina)
__attribute__((always_inline))  // fuerza que sea inline
wait_cpu_ticks(Int t) 
{
    while (t > 0){
	--t;
    }
}

}// namespace





#endif
