// Copyright (C) 2023 Manuel Perez 
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

#ifndef __MCU_STD_RANGE_ACCESS_H__
#define __MCU_STD_RANGE_ACCESS_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Las funciones begin()/end() según el estandard tienen que ser 
 *	accesibles desde un montón de archivos de cabecera.
 *	En este archivo metemos estas definiciones comunes. Estas
 *	definiciones el standard las llama "Range access" [iterator.range]
 *
 *
 * HISTORIA
 *    Manuel Perez
 *    05/10/2023 begin/end
 *
 ****************************************************************************/
#include "std_config.h"

namespace STD{

// begin
// -----
template <typename C>
constexpr auto begin(C& c) -> decltype(c.begin())
{ return c.begin(); }

template <typename C>
constexpr auto begin(const C& c) -> decltype(c.begin())
{ return c.begin(); }

template <typename T, size_t N>
constexpr T* begin(T (&a)[N]) noexcept 
{ return a; }


// end
// ---
template <typename C>
constexpr auto end(C& c) -> decltype(c.end())
{ return c.end(); }

template <typename C>
constexpr auto end(const C& c) -> decltype(c.end())
{ return c.end(); }

template <typename T, size_t N>
constexpr T* end(T (&a)[N]) noexcept 
{ return a + N; }



}// namespace STD
#endif


