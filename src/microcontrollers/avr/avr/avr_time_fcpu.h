// Copyright (C) 2019-2024 Manuel Perez 
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

#ifndef __AVR_TIME_FCPU_H__
#define __AVR_TIME_FCPU_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Funciones para manejo de tiempos en los AVR
 *
 *  - COMENTARIOS:
 *     CUIDADO: Todas las macros dependen de F_CPU, luego no se pueden 
 *     usar dentro para implementar el .cpp de una bibioteca!!!
 *     He intentado hacer una función genérica:
 *	    inline void wait_ms(uint8_t ms);
 *	pero no funciona, ya que todas las macros esperan una constante.
 *	Además en la práctica no necesito esa función.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	27/01/2019 Reestructurado.
 *	11/12/2022 Encapsulamos macros en funciones de C++
 *	10/07/2024 Wait_1_us/Wait_1_ms
 *
 ****************************************************************************/
#ifndef F_CPU
#error "To include these header you need to define F_CPU"
#endif

// Si no se define __DELAY_ROUND_CLOSEST__ antes de delay.h
// genera un error _delay_ms al definir F_CPU = 16MHz/6.
#define __DELAY_ROUND_CLOSEST__
#include <util/delay.h>
#undef __DELAY_ROUND_CLOSEST__

#include <concepts>

namespace avr_{
/// Espera t microsegundos
/// t tiene que ser una constante en tiempo de compilación.
template <std::integral Int>
inline constexpr void wait_us(Int t)
{
    _delay_us(t);
}

/// Espera t milisegundos.
/// t tiene que ser una constante en tiempo de compilación.
template <std::integral Int>
inline constexpr void wait_ms(Int t)
{
    _delay_ms(t);
}

#undef _delay_us
#undef _delay_ms

// Wait_1_us/ms
// ------------
struct Wait_1_us{
    void operator()() {wait_us(1);}
};

struct Wait_1_ms{
    void operator()() {wait_ms(1);}
};

}// namespace



#endif
