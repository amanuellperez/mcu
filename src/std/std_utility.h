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

#ifndef __MCU_STD_UTILITY_H__
#define __MCU_STD_UTILITY_H__
/****************************************************************************
 *
 *   - DESCRIPCION: El equivalente a <utility>
 *
 *   - COMENTARIOS: 
 *
 *   - HISTORIA:
 *    Manuel Perez
 *    26/08/2019 Version de pair básica (no incluye
 *					todo lo indicado por el estandar)
 *    23/02/2020 tuple_element, move, forward.
 *
 ****************************************************************************/
#include "std_config.h"

#include "std_type_traits.h"	// incluye declval
namespace STD{

// forward/move
// ------------
// Implementado en std_type_traits0.h


// pair
// ----
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



// tuple_element
// -------------
template <size_t i, typename T>
struct tuple_element;

template <size_t i, typename T0, typename T1>
struct tuple_element<i, pair<T0, T1>>
{
    static_assert(i < 2, "std::pair has only 2 elements");
};

// ----------------------------------
// tuple_element<i, pair<T0, T1>>:
//
//  if (i == 0) return T0;
//  else	return T1;
// ----------------------------------
template <typename T0, typename T1>
struct tuple_element<0, pair<T0, T1>>
{
    using type = T0;
};


template <typename T0, typename T1>
struct tuple_element<1, pair<T0, T1>>
{
    using type = T1;
};



// tuple_element_t
// El standard marca que esto esté en <tuple>. gcc lo mete aquí en utility.
// De momento lo dejo como gcc.
template <size_t i, typename T>
using tuple_element_t = typename tuple_element<i, T>::type;

}// namespace

#endif


