// Copyright (C) 2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __MCU_STD_CMATH_H__
#define __MCU_STD_CMATH_H__

/****************************************************************************
 *
 * - DESCRIPCION: El equivalente a <cmath>
 *
 * - HISTORIA:
 *  A.Manuel L.Perez 
 *	31/03/2020 abs
 *
 *
 ****************************************************************************/
#include "std_config.h"

namespace STD{

inline int abs(int n) { return n >= 0? n: -n; }
inline long abs(long n) { return n >= 0? n: -n; }
inline long long abs(long long n) { return n >= 0? n: -n; }


// El standard no define abs como constexpr y parece ser que prohibe a las
// implementaciones añadir constexpr cuando no está indicado así. Defino estas
// internas para poder usarlas.
constexpr inline int __abs(int n) { return n >= 0? n: -n; }
constexpr inline long __abs(long n) { return n >= 0? n: -n; }
constexpr inline long long __abs(long long n) { return n >= 0? n: -n; }

}// namespace


#endif

