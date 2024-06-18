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

#ifndef __ATD_CMATH_H__
#define __ATD_CMATH_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Ampliación de <cmath>
 *
 * HISTORIA
 *    Manuel Perez
 *    05/04/2020 div
 *    14/12/2020 abs
 *    12/10/2023 Traido de atd_math.h (tenía dependencias circulares)
 *
 ****************************************************************************/
#include <utility>

#include "atd_concepts.h"

#undef abs
namespace atd{

// abs
// ---
// El standard no suministra std::abs(unsigned), ni es constexpr
template <Type::Integer Int>
inline constexpr Int abs(Int x)
{ return x >= 0? x: -x; }


// div
// ---
// El standard no define std::div como constexpr!!!
template <typename Int>
inline constexpr std::pair<Int, Int> div(Int x, Int y)
{
    return {x / y, x % y};
}

}// namespace

#endif


