// Copyright (C) 2019-2020 A.Manuel L.Perez
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

#ifndef __ATD_ASCII_H__
#define __ATD_ASCII_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Interfaz para manejar caracteres ASCII
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           alp  - 08/02/2019 Escrito
 *
 ****************************************************************************/
#include <cstddef> // char
#include <algorithm>

namespace atd{

/// Devuelve el código ascii de 'd'
/// Ejemplo: 
///	uint8_t d = 5;
///	char c = digit_to_ascii(d);
///	si d = 5 (= 0x05) ---> c = 0x35 (código ascii de 5)
/// Precondition: d is a digit.
constexpr char digit_to_ascii(char d)
{return '0' + d;}

template <typename It>
constexpr void transform_digit_to_ascii(It first, It last)
{
    std::for_each(first, last, [](char& x){ x = digit_to_ascii(x);});
}


/// Devuelve el código ascii de 'd'
/// Ejemplo: si d = 5 (= 0x05) ---> c = 0x35 (código ascii de 5)
/// Precondition: d is a digit.
constexpr char ascii_to_digit(char d)
{return d - '0';}

struct ASCII {
    constexpr static char NUL = 0;	// Null
    constexpr static char SOH = 1;	// Start of header
    constexpr static char STX = 2;	// Start of text
    constexpr static char ETX = 3;	// End of text
    constexpr static char EOT = 4;	// End of transmission
    constexpr static char ACK = 6;	// Acknowledge
    constexpr static char NACK = 25;	// Negative acknowledge
};



}// namespace

#endif



