// Copyright (C) 2023-2024 Manuel Perez 
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

#ifndef __ATD_CONCEPTS_H__
#define __ATD_CONCEPTS_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Ampliación de concepts.
 *
 *	Los voy a meter en el namespace Type, para poder escribir cosas del
 *	tipo:
 *	    template <Type::Integer Int>
 *	    ...
 *
 *	Si los dejo en `atd` podría tener conflicto de nombres en el futuro.
 *
 *
 * HISTORIA
 *    Manuel Perez
 *    12/10/2023 Integer, Decimal
 *    16/10/2023 Arithmetic, Ostream, Istream, Iostream
 *    16/06/2024 Numeric
 *
 ****************************************************************************/
#include <concepts>
#include "atd_type_traits.h"
#include <iostream> // std::ostream...

namespace Type{

// Integer
// --------
template <typename T>
concept Integer = atd::is_integer_v<T>;

// Signed_integer
// --------------
template <typename T>
concept Signed_integer = Integer<T> and std::is_signed_v<T>;

// Unsigned_integer
// --------------
template <typename T>
concept Unsigned_integer = Integer<T> and !Signed_integer<T>;

// Decimal
// --------
template <typename T>
concept Decimal = atd::is_decimal_v<T>;


// Arithmetic
// ----------
template <typename T>
concept Arithmetic = Integer<T> or Decimal<T>;

// Ostream
// -------
template <typename Out>
concept Ostream = std::derived_from<Out, std::ostream>;

// Istream
// -------
template <typename In>
concept Istream = std::derived_from<In, std::istream>;

// IOstream
// --------
template <typename In_out>
concept IOstream = Ostream<In_out> and Istream<In_out>;

// Numeric
// -------
template <typename Int>
concept Numeric = std::integral<Int> or std::floating_point<Int>;

}// namespace Type

#endif


