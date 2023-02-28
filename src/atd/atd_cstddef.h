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

#ifndef __ATD_CSTDDEF_H__
#define __ATD_CSTDDEF_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Ampliación de std_cstddef.h
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	25/10/2019 operator<<(ostream, byte)
 *
 ****************************************************************************/
#include "atd_bit.h"
#include <ostream>
#include <istream>

#include <cstddef>

namespace atd{

template <uint8_t pos, typename Int>
void print_bit(std::ostream& out, Int x)
{
    if (atd::is_one_bit<pos>::of(x))
	out << '1';
    else 
	out << '0';
}

void print_in_binary(std::ostream& out, std::byte b);

constexpr char nibble__[16] = {
    '0', '1', '2', '3', '4',
    '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E',
    'F',
};

inline void print_nibble(std::ostream& out, std::byte b)
{ out << nibble__[std::to_integer<uint8_t>(b)]; }

void print_in_hex(std::ostream& out, std::byte b);

/// Imprime el byte en el flujo out.
inline std::ostream& write(std::ostream& out, std::byte b)
{
    out.write(reinterpret_cast<const char*>(&b), 1);

    return out;
}


/// Lee del flujo in un byte almacenándolo en b.
inline std::istream& read(std::istream& in, std::byte& b)
{
    in.read(reinterpret_cast<char*>(&b), 1);

    return in;
}

// OJO: El compilador creo que busca este operador en std!!!
// ignorando atd!!!
inline std::ostream& operator<<(std::ostream& out, std::byte b)
{
    write(out, b);

    return out;
}

}// namespace

#endif
