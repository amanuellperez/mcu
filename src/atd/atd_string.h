// Copyright (C) 2021 A.Manuel L.Perez 
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

#ifndef __ATD_STRING_H__
#define __ATD_STRING_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Algoritmos para manejar cadenas.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    24/07/2021 to_int, to_char
 *
 ****************************************************************************/

namespace atd{

/// Convierte el dígito c de char a int.
/// digito = del 0 al 9
template <typename Int>
inline Int digit_to_(char c) {return c - '0';}

/// Convierte el char c de int a char.
/// digito = del 0 al 9
template <typename Int>
inline char digit_to_char(Int x) {return x + '0';}


// TODO: se puede llamar to_cstring sin el "int" identificando o bien Int como
// concept integer, o con metaprogramming.
// ¿por qué no usar sprintf? Porque sprintf no gestiona el tamaño de la
// cadena pudiendo generar overflow.
/// Convierte el entero x en cadena de C guardándolo en [p, sz). Garantiza
/// que no haya overflow.
/// Devuelve el número de caracteres ocupados.
//template <typename Int>
//size_t int_to_cstring(Int x, const char* p, size_t sz)
//{
//    if (x == Int{0}){
//	*p = '0';
//	return 1;
//    }
////
////    size_t i = 0;
////
////    for (; i < sz - 1; ++i)
////    {
////        x *= 10.0;
////	Int y = static_cast<int>(y);
////	y /= 10;
////	str[i] = digit_to_char(y);
////    }
////
////    str[i - 1] = '\0';
//
//}




}// namespace

#endif


