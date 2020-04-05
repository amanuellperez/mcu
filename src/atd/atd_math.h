// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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
 *
 ****************************************************************************/
#include <cstdlib>
#include <utility>

namespace atd{

template <typename Int>
inline constexpr Int ten_to_the(int n)
{
    if (n == 0)
	return Int{1};

    else 
	return Int{10} * ten_to_the<Int>(n - 1);
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

    while (x > ten_to_the<Int>(n))
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


// TODO: borrar todo esto (07/03/20)
///// Resta a-b siempre que a sea mayor que b.
///// Esta función esta para unsigned int.
//template <typename T>
//inline T resta_si_se_puede(T a, T b)
//{
//    return (a > b ? a - b : b);
//}
//
//
///***************************************************************************
// *		FORMAS EFICIENTES DE DIVIDIR ENTRE 2^x
// ***************************************************************************/
///// Dividimos entre 2
//template<typename T>
//inline T divide_entre_2(T x) {return x >> 1;}
//
///// Dividimos entre 4
//template<typename T>
//inline T divide_entre_4(T x) {return x >> 2;}
//
///// Dividimos entre 8
//template<typename T>
//inline T divide_entre_8(T x) {return x >> 3;}
//
///// Dividimos entre 16
///// Si redondea = true, redondeamos; en caso contrario, truncamos.
///// Como en C++ lo habitual al dividir es truncar, por eso pongo por defecto
///// que se trunque.
////
//// TODO: convertir en template el bool redondea, para que sea más
//// eficiente???
//template<typename T>
//inline T divide_entre_16(T x, bool redondea = false)
//{
//    if(redondea)
//	return (x+8) >> 4;
//
//    return x >> 4;
//}
//
//
///// Dividimos entre 1024
//template<typename T>
//inline T divide_entre_1024(T x) {return x >> 10;}


} // namespace


#endif

