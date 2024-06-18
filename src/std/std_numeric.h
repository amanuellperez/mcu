// Copyright (C) 2020 Manuel Perez 
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

#ifndef __MCU_STD_NUMERIC_H__
#define __MCU_STD_NUMERIC_H__

/****************************************************************************
 *
 * - DESCRIPCION: El equivalente a <numeric>
 *
 * - HISTORIA:
 *  Manuel Perez 
 *	31/03/2020 gcd, lcm
 *	08/12/2020 iota
 *
 *
 ****************************************************************************/
#include "std_config.h"

#include "type_traits"
#include "cmath"	// abs

namespace STD{

/// Returns the greates common divisor of |a| and |b|.
template <typename Int1, typename Int2>
constexpr common_type_t<Int1, Int2> gcd(Int1 a, Int2 b)
{
    a = __abs(a);  // versiones constexpr
    b = __abs(b);

    while (true) {
        if (b == 0) return a;

        a = a % b;

        if (a == 0) return b;

        b = b % a;
    }
}

/// If either m or n is zero, returns zero. Otherwise, returns the least common
/// multiple of |m| and |n|.
// TODO: esta es una versión naive. ¿Qué pasa si a * b overflow pero al
// dividir por el gcd(a, b) realmente no hubiese overflow? Tendría que poderse
// calcular el valor.
template <typename Int1, typename Int2>
inline constexpr common_type_t<Int1, Int2> lcm(Int1 a, Int2 b)
{
    if (a == 0 or b == 0)   // standard: devuelve 0 si alguno es cero
	return 0;

    return __abs(a * b) / gcd(a, b); 
}

template <typename It, typename T>
constexpr void iota(It p0, It pe, T value)
{
    for (; p0 != pe; ++p0){
	*p0 = value;
	++value;
    }
}

}// namespace


#endif

