// Copyright (C) 2022 Manuel Perez 
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

#ifndef __MCU_STD_FUNCTIONAL_H__
#define __MCU_STD_FUNCTIONAL_H__
/****************************************************************************
 *
 *  - DESCRIPCION: El correspondiente a <functional>
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	31/01/2022 std::less, less_equal, greater, greater_equal, 
 *		   equal_to, not_equal_to. (TODO: faltan especializaciones
 *		   tipo std::less<void>).
 *	21/09/2024 identity
 *
 ****************************************************************************/
#include "std_config.h"
#include "std_type_traits0.h" // forward para identity

namespace STD{
// Comparisons
// -----------
// TODO: faltan definir todas las especializaciones less<void>,
// greater<void>...
template <typename T>
struct less{
    constexpr bool operator()(const T& x, const T& y) const
    { return x < y;}
};

template <typename T>
struct less_equal{
    constexpr bool operator()(const T& x, const T& y) const
    { return x <= y;}
};

template <typename T>
struct greater{
    constexpr bool operator()(const T& x, const T& y) const
    { return x > y;}
};

template <typename T>
struct greater_equal{
    constexpr bool operator()(const T& x, const T& y) const
    { return x >= y;}
};

template <typename T>
struct equal_to{
    constexpr bool operator()(const T& x, const T& y) const
    { return x == y;}
};


template <typename T>
struct not_equal_to{
    constexpr bool operator()(const T& x, const T& y) const
    { return x != y;}
};


// identity
// --------
// gcc define esta clase en otro header que luego incluye functional.
// Además define con el atributo [[nodiscard]] el operator(), pero el
// estandard de C++ del 2023 no lo define así (???)

namespace impl_of {
    struct is_transparent; // gcc no lo define
}; 

struct identity
{
    template <typename T>
    constexpr T&& operator()(T&& t) const noexcept
    {return STD::forward<T>(t); }

    using is_transparent = impl_of::is_transparent;
};

}// namespace

#endif

