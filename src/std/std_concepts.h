// Copyright (C) 2022-2023 Manuel Perez 
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

#ifndef __MCU_STD_CONCEPTS_H__
#define __MCU_STD_CONCEPTS_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	El correspondiente a <concepts>
 *
 * HISTORIA
 *    Manuel Perez
 *    11/12/2022 integral
 *    27/09/2023 same_as
 *
 ****************************************************************************/
#include "std_config.h"
#include "std_type_traits.h"

namespace STD{
// language-related concepts
// --------------------------
namespace impl_of{
template <typename T, typename U>
concept same_as = is_same_v<T,U>;
}// namespace impl_of
 
template <typename T, typename U>
concept same_as = impl_of::same_as<T,U> and impl_of::same_as<U,T>;

template <typename Derived, typename Base>
concept derived_from =
	is_base_of_v<Base, Derived> and
	is_convertible_v<const volatile Derived*, const volatile Base*>;

//convertible_to
//common_reference_with
//common_with

// arithmetic concepts
template <typename T>
concept integral = is_integral_v<T>;

//signed_integral
//unsigned_integral
//floating_point
//
//assignable_from
//
//// TODO: ranges 
//
//destructible
//constructible_from
//default_initializable
//move_constructible
//copy_constructible
//
//// comparison concepts
//// -------------------
//equality_comparable
//equality_comparable_with
//totally_ordered
//totally_ordered_with
//
//// object concepts
//// ---------------
//movable
//copyable
//semiregular
//regular
//
// etc...

}// namespace

#endif


