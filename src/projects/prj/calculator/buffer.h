// Copyright (C) 2021 Manuel Perez 
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

#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <ostream>
#include <stdlib.h> // strtod

#include <atd_array.h>
#include <atd_double.h>


#undef getchar

template <size_t N>
using Linear_array= atd::Linear_array<char, N>;

// Devuelve el número de caracteres copiados
template <size_t N>
uint8_t push_back(Linear_array<N>& buf, const char* str)
{
    uint8_t i = 0;

    while (*str and buf.end() != buf.me()){
	buf.push_back(*str);
	++str;
	++i;
    }

    return i;
}


// Devuelve un iterador a donde continuar insertando elementos.
template <size_t N>
typename Linear_array<N>::iterator insert(Linear_array<N>& buf,
                                          typename Linear_array<N>::iterator p,
                                          const char* str)
{
    while (*str and p != buf.end()){
	buf.insert(p, *str);
	++str;
	++p;
    }

    return p;
}


// devuelve: true, todo bien
//           false, error al hacer la conversion
template <size_t N>
bool read(Linear_array<N>& buf, double& x) 
{
    char* p{};
    x = strtod(buf.begin(), &p); 
    if (p == buf.begin())
	return false;

    buf.begin(p);
    return true;
}


template <size_t N>
void print(std::ostream& out, const Linear_array<N>& buf)
{
    for (auto x: buf)
	out << x;
}

#endif


