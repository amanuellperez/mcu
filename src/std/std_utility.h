// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __MCU_STD_UTILITY_H__
#define __MCU_STD_UTILITY_H__
/****************************************************************************
 *
 *   - DESCRIPCION: El equivalente a <utility>
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *           A.Manuel L.Perez- 26/08/2019 Version de pair básica (no incluye
 *					todo lo indicado por el estandar)
 *
 ****************************************************************************/
#include "std_config.h"

#include "std_type_traits.h"	// incluye declval
namespace STD{

// TODO: versión básica de pair. El standard tiene muchos más constructores y
// funciones.
template <typename T1, typename T2>
struct pair{
    using first_type  = T1;
    using second_type = T2;

    T1 first;
    T2 second;

    constexpr pair():first{T1{}}, second{T2{}} {}
    constexpr pair(const T1& x, const T2& y):first{x}, second{y} {}

    pair(const pair& p) = default;
    pair(pair&& p)      = default;

    constexpr pair& operator=(const pair& other)
    {
        first  = other.first;
        second = other.second;
    }

};


template <typename T1, typename T2>
inline constexpr bool operator==(const pair<T1,T2>& x, const pair<T1,T2>& y)
{
    return x.first == y.first and x.second == y.second;
}

template <typename T1, typename T2>
inline constexpr bool operator!=(const pair<T1,T2>& x, const pair<T1,T2>& y)
{
    return !(x == y);
}

// TODO: Falta la relación de orden de pair definida por el standard.


}// namespace

#endif


