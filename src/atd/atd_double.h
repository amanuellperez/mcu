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
 *    23/07/2021 modf, print
 *
 ****************************************************************************/
#include <cmath>
#include <utility>
#include <ostream>
#include <array>

#include "atd_math.h"
#include "atd_string.h"
#include "atd_algorithm.h"


namespace atd{


/// Descompone un double en parte entera y fraccionaria.
/// Si x = 123.456 devuelve {123, 0.456}. Observar que la parte fraccionaria es
/// menor que 1.
inline std::pair<double, double> modf(double x)
{
    double integer_part{};
    double decimal_part = std::modf(x, &integer_part);

    return std::pair{integer_part, decimal_part};
}


// No se trata de una función genérica. Sabé que str es un número, y lo que
// hace es eliminar todos los ceros a la izquierda. Si el número fuera todo
// ceros devuelve "0" y no "" que sería lo que haría una función genérica.
template <size_t N>
std::array<char, N>::iterator 
		    __remove_trailing_zeros(std::array<char, N>& str)
{
    static_assert(N >= 1); // std::next(begin) ==> minimo 1 elemento.

    auto p = std::find_if(str.rbegin(), str.rend(), 
			 [](auto x){ return x != '0';});

    if (p == str.rend()) // todo ceros
	return std::next(str.begin());

    else if (p == str.rbegin())
	return str.end();

    else
	return p.base();
}


// Convierte el double 'x' en la cadena de caracteres 'str'.
//  Ejemplo: si x = 0.123 y str es de 5 caracteres devuelve str = 12300
//  No añade '\0' al final, cuidado si se imprime.
// Precondition: x0 = 0.yyyy (parte entera(x0)  < 1)
template <size_t N>
void __mantissa_to_str(const double& x0, std::array<char, N>& str)
{
    static_assert(N >= 1); // prev ==> necesita minimo 1 elemento!!!
    static_assert(N <= 8); // al usar int32_t no puedo almacenar más de 10^8
                           // si se necesitan más cambiar implementación

    int32_t x = static_cast<int32_t>(x0 * ten_to_the<double>(N));

// to_string:
    auto p = str.rbegin(); 
    for (; p != str.rend(); ++p){
	int y = x % 10;
	*p = digit_to_char(y);
	x /= 10;
    }

    
// La siguiente implementación no funciona por culpa de que double x = 1.0 no
// es realmente .0!!!
//    for (size_t i = 0; i < N - 1; ++i)
//    {// llenará de 0 str si x = 0
//        x *= 10.0;
//	int y = static_cast<int>(y);
//	y /= 10;
//	str[i] = digit_to_char(y);
//	x -= static_cast<double>(y);
//    }
//    str[N - 1] = '\0';
}



// Precondition: x = +0.yyyy (parte entera = 0)
template <size_t ndigits = 8>
void __print_mantissa(std::ostream& out, const double& x)
{
    std::array<char, ndigits> str; 

    __mantissa_to_str(x, str);
    auto pe = __remove_trailing_zeros(str);

    print(out, str.begin(), pe);
}


// TODO: meter en .cpp 
// ¿cómo imprimir un double en un LCD? Esta función se encarga de ello.
inline void print(std::ostream& out, double x)
{
    constexpr char decimal_point = '.'; // TODO: ¿dónde guardarlo? locale?

    auto [i, f] = modf(x);
    
    out << static_cast<int>(i);
    
    if (f != 0){
	out << decimal_point;
	__print_mantissa(out, f);
    }
}



}// namespace

#endif


