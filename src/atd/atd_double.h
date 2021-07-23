// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __ATD_DOUBLE_H__
#define __ATD_DOUBLE_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Soporte para 'double' para los micros. Algunas funciones
 *  útiles.
 *
 *    Aunque es posible que sea más eficiente usar atd::Decimal (???) para
 *    hacer la calculadora voy a experimentar usando 'double'.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    23/07/2021 v0.0
 *
 ****************************************************************************/
#include <cmath>
#include <utility>
#include <ostream>

#include "atd_math.h"



namespace atd{


/// Convierte el número 'x' en '0.x'
/// Ejemplo: si x = 435, devuelve y = 0.435.
/// Precondition: x >= 0.
template <typename Double, typename Int>
inline Double integer_to_mantissa(const Int& x)
{
    Double y = static_cast<Double>(x);

    while (y > Double{1}){
	y /= Double{10};
    }

    return y;
}

/// Ejemplo: si x = 0.123 devuelve 123.
/// Precondition: abs(x) < 1
template <typename Int, typename Double>
Int mantissa_to_integer(Double x)
{
    constexpr int d10 = std::numeric_limits<Int>::digits10;

    x *= ten_to_the<Int>(d10);

    Int y = static_cast<Int>(x);

    return remove_trailing_zeros(y);
}

/// Descompone un double en parte entera y fraccionaria.
/// Si x = 123.456 devuelve {123, 0.456}. Observar que la parte fraccionaria es
/// menor que 1.
inline std::pair<double, double> modf(double x)
{
    double integer_part{};
    double decimal_part = std::modf(x, &integer_part);

    return std::pair{integer_part, decimal_part};
}


/// Descompone un double en parte entera y fraccionaria.
// TODO: no funciona bien si Int es un tipo de 64 bits, ya que guarda
// El motivo es que 2.354 lo guarda como 2.3540000000000001024!!! 
// Al descomponerlo devuelve el 35400...01024!!! (la función funciona
// correctamente, aunque se pasa de precisión).
template <typename Int>
inline std::pair<Int, Int> modf_as_int(double x)
{
    auto [i, f] = modf(x);

    return std::pair{i, mantissa_to_integer<Int>(f)};
}




inline void print(std::ostream& out, double x)
{
    constexpr char decimal_point = '.'; // TODO: ¿dónde guardarlo? locale?

    auto [i, f] = modf_as_int<int>(x);

    out << static_cast<int>(i);
    
    if (f != 0){
	out << decimal_point << static_cast<int>(f);
    }
}



}// namespace

#endif


