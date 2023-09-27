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
 * DUDA
 *	Si el estandar suministra la mayoría de las definiciones de estos
 *	concepts, por qué no dan también el código fuente para poder
 *	incorporarlo directamente?
 *
 * HISTORIA
 *    Manuel Perez
 *    11/12/2022 integral
 *    27/09/2023 same_as, ...
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

template <typename From, typename To>
concept convertible_to =
    is_convertible_v<From, To> and
    requires(add_rvalue_reference_t<From> (&f)()){
	static_cast<To>(f());
    };


template <typename T, typename U>
concept common_reference_with =
	same_as<common_reference_t<T, U>, common_reference_t<U, T>> 
	and convertible_to<T, common_reference_t<T, U>>
	and convertible_to<U, common_reference_t<T, U>>;

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


