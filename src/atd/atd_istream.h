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

#ifndef __ATD_ISTREAM_H__
#define __ATD_ISTREAM_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Ampliación de istream
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    A.Manuel Lopez
 *	10/11/2019 read_as_int8_t
 *
 ****************************************************************************/
#include <istream>
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



}// namespace

#endif


