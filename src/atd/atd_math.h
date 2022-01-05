// Copyright (C) 2019-2020 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#ifndef __ATD_MATH_H__
#define __ATD_MATH_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Funciones matemáticas.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    07/03/2020 ten_to_the
 *    05/04/2020 number_of_digits, div
 *    14/12/2020 abs
 *    05/03/2021 is_power_of_ten, exponent_of_power_of_ten
 *    23/07/2021 remove_trailing_zeros
 *
 *
 ****************************************************************************/
#include <cstdlib>
#include <utility>
#include <type_traits>

#undef abs

namespace atd{

/// Devuelve 10^n
template <typename Int>
inline constexpr Int ten_to_the(int n)
{
    if (n == 0)
	return Int{1};

    else 
	return Int{10} * ten_to_the<Int>(n - 1);
}


// if (x == 10^n) return true;
// else           return false;
//
// El 1 == 10^0, lo considero una potencia de 10 (lo normal en matemáticas)
// Las potencias de 10 son números positivos.
// TODO: de momento solo funciona con n >= 0. Implementar el caso n < 0
template <typename Int>
inline constexpr bool is_power_of_ten(Int x)
{
    if (x == 1)
	return true;

    if (!(x % Int{10})){
	    return is_power_of_ten(x / Int{10});
    }
    
    else
	return false;

}

/// Devuelve el exponente de una potencia de 10.
/// precondicion: x == 10^n.
/// En caso de que x no sea una potencia de 10, devuelve 0.
template <typename Int>
inline constexpr int exponent_of_power_of_ten(Int x, int exp = 0)
{
    if (x == 1)
	return exp;

    if (!(x % Int{10})){
	    return exponent_of_power_of_ten(x / Int{10}, exp + 1);
    }
    
    else
	return 0;

}





/// Devuelve los n most significant digits de x.
///
/// Ejemplo:
///	    int x = most_significant_digits<int, 2>(1234);
///
/// Devuelve x == 12;
template <typename Int, int n>
inline constexpr Int most_significant_digits(Int x)
{
    static_assert(n > 0, "n must be greater than 0");

    while (x >= ten_to_the<Int>(n))
	x /= Int{10};

    return x;
}

// Devuelve el número de cifras que tiene x.
// Ejemplo: number_of_digits(324) == 3;
template <typename Int>
inline constexpr int number_of_digits(Int x)
{
    int n = 1;

    while (x >= Int{10}){
	++n;
	x /= 10;
    }

    return n;
}



// El standard no define std::div como constexpr!!!
template <typename Int>
inline constexpr std::pair<Int, Int> div(Int x, Int y)
{
    return {x / y, x % y};
}


// El standard no suministra std::abs(unsigned), ni es constexpr
template <typename Int>
inline constexpr Int abs(Int x)
{ return x >= 0? x: -x; }


/// Devuelve x pero sin los 0 a la derecha.
/// Ejemplo: si x = 12300, devuelve 123.
template <typename Int>
inline constexpr Int remove_trailing_zeros(Int x)
{
    while (x != 0 and (x % 10) == 0){
	x /= Int{10};
    }

    return x;
}


} // namespace


#endif

