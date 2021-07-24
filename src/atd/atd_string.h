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


}// namespace

#endif


