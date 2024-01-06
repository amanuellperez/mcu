// Copyright (C) 2019-2020 Manuel Perez 
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

#ifndef __ATD_ASCII_H__
#define __ATD_ASCII_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Interfaz para manejar caracteres ASCII
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *    Manuel Perez
 *    08/02/2019 Escrito
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
			

// Códigos ASCII de la combinación de teclas Ctrl+...
// (???) No he encontrado esto standard. En linux parece funcionar.
// Ejemplo: ASCII::Ctrl::A == código del caracter Ctrl+A
// DUDA: Ctrl ó CTRL???
    struct Ctrl{
	constexpr static char A = 1;
	constexpr static char B = 2;
	constexpr static char C = 3;
	constexpr static char D = 4;
	constexpr static char E = 5;
	constexpr static char F = 6;
	constexpr static char G = 7;
	constexpr static char H = 8;
	constexpr static char I = 9;
	constexpr static char J = 10;
	constexpr static char K = 11;
	constexpr static char L = 12;
	constexpr static char M = 13;
	constexpr static char N = 14;
	constexpr static char O = 15;
	constexpr static char P = 16;
	constexpr static char Q = 17;
	constexpr static char R = 18;
	constexpr static char S = 19;
	constexpr static char T = 20;
	constexpr static char U = 21;
	constexpr static char V = 22;
	constexpr static char W = 23;
	constexpr static char X = 24;
	constexpr static char Y = 25;
	constexpr static char Z = 26;
    };
};


}// namespace

#endif



