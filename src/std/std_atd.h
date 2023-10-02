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

#ifndef __MCU_STD_ATD_H__
#define __MCU_STD_ATD_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Cosas genéricas usadas en la implementación de std.
 *	Sistemáticamente me encuentro con código genérico que uso en la
 *	implementación de `std`, código que sería interesante poder usar. Sin
 *	embargo, como es código no estandar no se puede incluir en `std` como
 *	tal.
 *
 *	Para no tener que reescribir dicho código voy a identificarlo
 *	metiéndolo todo en el namespace atd_. De esa forma, lo puedo recuperar
 *	en atd fácilmente sin tener que reescribirlo.
 *
 * HISTORIA
 *    Manuel Perez
 *    26/09/2023 dereferenceable
 *
 ****************************************************************************/
#include "std_config.h"
// Este header es común a todo. 
// Que no incluya ningún otro archivo para evitar dependencias circulares.
namespace STD{


// Helper class: integral_constant
// ------------------------------------
template <typename T, T v>
struct integral_constant{
    using value_type = T;
    using type       = integral_constant<T,v>;

    static constexpr T value = v;

    constexpr operator value_type() const noexcept {return  value;}
    constexpr value_type operator()() const noexcept {return value;}
};


/// bool_constant
template <bool B>
using bool_constant = integral_constant<bool, B>;

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

// void_t
// ------
template <typename...>
using void_t = void;


namespace atd_{

// add_reference
// -------------
template<typename T>
using add_reference = T&;

template<typename T>
concept is_referenceable = requires { typename add_reference<T>; };

template<typename T>
concept dereferenceable = 
	requires(T& t)
	{
	    { *t } -> is_referenceable;
	};


// ------------------
// operadores lógicos
// ------------------
// not
// ---
template <bool x>
struct static_not;

template <>
struct static_not<true> : public false_type { };

template <>
struct static_not<false> : public true_type { };

template <bool x>
using static_not_t = typename static_not<x>::type;


// and
// ---
template <bool x1, bool x2, bool... rest>
struct static_and :
    public static_and<x1 and x2, rest...>
{ };

template <bool x1, bool x2>
struct static_and<x1, x2> :
    public bool_constant<x1 and x2>
{ };

template <bool x1, bool x2, bool... rest>
using static_and_t = typename static_and<x1, x2, rest...>::type;

// or
// --
template <bool x1, bool x2, bool... rest>
struct static_or:
    public static_or<x1 or x2, rest...>
{ };

template <bool x1, bool x2>
struct static_or<x1, x2> :
    public bool_constant<x1 or x2>
{ };


template <bool x1, bool x2, bool... rest>
using static_or_t = typename static_or<x1, x2, rest...>::type;


// has_type_member
// ---------------
template <typename T, typename = void>
struct has_type_member : false_type { };

template <typename T>
struct has_type_member<T, void_t<typename T::type>>
	    : true_type { };

template <typename T>
inline constexpr bool has_type_member_v = has_type_member<T>::value;

// type_member
// -----------
template <typename T>
concept type_member = has_type_member_v<T>;

}// namespace atd_


}// namespace STD
#endif


