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
#ifndef __MCU_STD_CHAR_TRAITS_H__
#define __MCU_STD_CHAR_TRAITS_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Implementación mínima del sistema de flujos.
 *
 *  - COMENTARIOS: Iré implementando las cosas según las vaya necesitando.
 *
 *  - HISTORIA:
 *	Manuel Perez- 24/09/2019 v0.0
 *
 ****************************************************************************/
#include "std_config.h"

#include <stdio.h>  // EOF
#include <string.h> // memcpy

namespace STD{

template <typename CharT> struct char_traits;

template<>
struct char_traits<char>{
    using char_type = char;
    using int_type  = int;
    
    static constexpr void assign(char_type& x, const char_type& a) noexcept
    { x = a;}

    static constexpr bool eq(const char_type& x, const char_type& y) noexcept
    { return x == y;}


    static char_type* copy(char_type* dest, const char_type* src, size_t n)
    {
	if (n == 0)
	    return dest;

	return static_cast<char_type*>(memcpy(dest, src, n));
    }

    static constexpr char_type to_char_type(const int_type& c) noexcept
    {return static_cast<char_type>(c);}


    // DUDA: esto es copia del char_traits del compilador gcc. ¿Por qué es
    // necesario hacer el cast a unsigned char?
    static constexpr int_type to_int_type(const char_type& c) noexcept
    {return static_cast<int_type>(static_cast<unsigned char>(c));}
//    static constexpr int_type to_int_type(char_type c) noexcept
//    {return static_cast<int_type>(c);}

    static constexpr bool eq_int_type(const int_type& c1, const int_type& c2) noexcept
    {return c1 == c2;}

    static constexpr int_type eof() noexcept
    {return static_cast<int_type>(EOF);}

    static constexpr int_type not_eof(const int_type& c) noexcept
    {return (c == eof()) ? 0 : c;}

};


}// namespace

#endif

