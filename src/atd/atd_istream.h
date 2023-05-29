// Copyright (C) 2019-2023 Manuel Perez 
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

#ifndef __ATD_ISTREAM_H__
#define __ATD_ISTREAM_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Ampliación de istream
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	10/11/2019 read_as_int8_t
 *	15/11/2022 read_int_as_hex
 *	20/05/2023 read_int_as_hex(start_with_0x = true)
 *
 ****************************************************************************/
#include <istream>
#include <ctype.h>
#include "atd_cast.h"

namespace atd{

struct _Read_as_uint8_t{ uint8_t* p; };

inline _Read_as_uint8_t read_as_uint8_t(uint8_t& x)
{return _Read_as_uint8_t{&x};}

/// Intenta leer un uint8_t de in. En caso de fallo no modifica el valor de x.
/// El problema es que aunque el standard define uint8_t como un tipo de
/// entero, en la práctica se define como un char, con lo que el operador 
/// in >> x; no lee x como entero sino como char, lo cual es un error.
inline std::istream& operator>>(std::istream& in, _Read_as_uint8_t x)
{
    uint16_t tmp;
    in >> tmp;
    if (in)
	*(x.p) = atd::bounded_cast<uint8_t>(tmp);
    
    return in;
}


struct _Read_as_int8_t{ int8_t* p; };

inline _Read_as_int8_t read_as_int8_t(int8_t& x)
{return _Read_as_int8_t{&x};}

/// Intenta leer un int8_t de in. En caso de fallo no modifica el valor de x.
/// El problema es que aunque el standard define int8_t como un tipo de
/// entero, en la práctica se define como un char, con lo que el operador 
/// in >> x; no lee x como entero sino como char, lo cual es un error.
inline std::istream& operator>>(std::istream& in, _Read_as_int8_t x)
{
    int16_t tmp;
    in >> tmp;
    if (in)
	*(x.p) = atd::bounded_cast<int8_t>(tmp);
    
    return in;
}



/***************************************************************************
 *				FORMATS
 ***************************************************************************/
static constexpr int hex2nibble_error__ = 0x10;

template <typename Int>
Int hex2nibble__(char c)
{
    switch(c){
	break; case '0': return 0x00;
	break; case '1': return 0x01;
	break; case '2': return 0x02;
	break; case '3': return 0x03;
	break; case '4': return 0x04;
	break; case '5': return 0x05;
	break; case '6': return 0x06;
	break; case '7': return 0x07;
	break; case '8': return 0x08;
	break; case '9': return 0x09;
	break; case 'A': case 'a': return 0x0A;
	break; case 'B': case 'b': return 0x0B;
	break; case 'C': case 'c': return 0x0C;
	break; case 'D': case 'd': return 0x0D;
	break; case 'E': case 'e': return 0x0E;
	break; case 'F': case 'f': return 0x0F;
	break; default : return Int{hex2nibble_error__};
    }

}

// Lee el siguiente caracter del flujo comprobando que vale 'res'
// Lo llamo `read` porque consume el caracter. Otra opción sería no consumir
// el caracter en caso de que no valga `res` (pero como siempre: ¿cómo llamar
// a esa función?)
// Return: true si el caracter leido es igual a res
//	   false en caso contrario
inline bool read_next_char(std::istream& in, char res)
{
    char c{};
    in.get(c);
    return c == res;
}

// Esta es la inversa a print_int_as_hex
// Lee una cadena de caracteres de istream que representa un número en
// hexadecimal y la convierte a Int
// Si start_with_0x  == true, el número tiene que empezar con '0x'.
// Devuelve el número de digitos leídos, si todo va bien.
// Si ha leido correctamente y de repente encuentra un dígito no válido
// devuelve 0.
// DUDA: ¿qué pasa si hay overflow? De momento no hago nada.
template <typename Int>
uint8_t read_int_as_hex(std::istream& in, Int& x, bool start_with_0x = true)
{
    if (start_with_0x == true){
	if (!(read_next_char(in, '0') and read_next_char(in, 'x'))){
	    in.setstate(std::ios_base::failbit);
	    return false;
	}
    }

    x = Int{0};
    uint8_t n = 0;
    char c = '\0';
    while (in.get(c)){
	if (isspace(c)) 
	    return n;

	Int d = hex2nibble__<Int>(c);
	if (d == Int{hex2nibble_error__})
	    return 0; // error

	x = x*Int{16} + d; 
	++n;
    }

    return n;

}



}// namespace

#endif


