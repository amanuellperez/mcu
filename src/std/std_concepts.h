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
// same_as
// -------
namespace impl_of{
template <typename T, typename U>
concept same_as = is_same_v<T,U>;
}// namespace impl_of
 
template <typename T, typename U>
concept same_as = impl_of::same_as<T,U> and impl_of::same_as<U,T>;

// derived_from
// ------------
template <typename Derived, typename Base>
concept derived_from =
	is_base_of_v<Base, Derived> and
	is_convertible_v<const volatile Derived*, const volatile Base*>;


// convertible_to
// --------------
template <typename From, typename To>
concept convertible_to =
    is_convertible_v<From, To> and
    requires(add_rvalue_reference_t<From> (&f)()){
	static_cast<To>(f());
    };


// common_reference_with
// ---------------------
template <typename T, typename U>
concept common_reference_with =
	same_as<common_reference_t<T, U>, common_reference_t<U, T>> 
	and convertible_to<T, common_reference_t<T, U>>
	and convertible_to<U, common_reference_t<T, U>>;

//common_with

// -------------------
// arithmetic concepts
// -------------------
// is_integral
// ----------
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
template <typename T>
concept destructible = is_nothrow_destructible_v<T>;

template <typename T, typename... Args>
concept constructible_from = destructible<T> and
			     is_constructible_v<T, Args...>;
//default_initializable
template <typename T>
concept move_constructible = constructible_from<T,T> and
			     convertible_to<T, T>;
	
//copy_constructible
//
// -------------------
// comparison concepts
// -------------------

// boolean
// -------
// Stroustrup ("A tour of C++", 14.5.1) tiene una implementación de boolean.
// Voy a basarme en la de gcc, aunque no tengo claro el `static_cast<T&&>`???
namespace atd_{
namespace impl_of {
template <typename T>
concept boolean_convertible = convertible_to<T, bool>;
}// impl_of
 
template<typename T>
concept boolean = impl_of::boolean_convertible<T> and
		  requires (T&& t){
		      {!static_cast<T&&>(t) } -> impl_of::boolean_convertible;
		  };
}// atd_
 
// equality_comparable
// -------------------
namespace private_{

template <typename T, typename U>
concept weakly_equality_comparable_with = 
    requires(const remove_reference_t<T>& t,
	     const remove_reference_t<U>& u){
	{ t == u } -> atd_::boolean;
	{ t != u } -> atd_::boolean;
	{ u == t } -> atd_::boolean;
	{ u != t } -> atd_::boolean;
    };

}// private_
 
template <typename T>
concept equality_comparable 
		    = private_::weakly_equality_comparable_with<T,T>;

// equality_comparable_with
// -------------------------
template <typename T, typename U>
concept equality_comparable_with =
	equality_comparable<T> and
	equality_comparable<U> and
	common_reference_with<const remove_reference_t<T>&, 
			      const remove_reference_t<U>&> and
	equality_comparable<common_reference_t<
					    const remove_reference_t<T>&,
					    const remove_reference_t<U>&>> and
	private_::weakly_equality_comparable_with<T, U>;



// totally_ordered
// ---------------
namespace private_ {
// Aunque el standard no da explícitamente la definición de
// partially_ordered_with, se saca de lo que dice en la explicación
// de totally_ordered_with
template<typename T, typename U>
concept partially_ordered_with = 
    requires (const remove_reference_t<T>& t,
	      const remove_reference_t<U>& u) {
		  { t <  u } -> atd_::boolean;
		  { t >  u } -> atd_::boolean;
		  { t <= u } -> atd_::boolean;
		  { t >= u } -> atd_::boolean;
		  { u <  t } -> atd_::boolean;
		  { u >  t } -> atd_::boolean;
		  { u <= t } -> atd_::boolean;
		  { u >= t } -> atd_::boolean;
    };
}// private_
 
template <typename T>
concept totally_ordered =
	equality_comparable<T> and
	private_::partially_ordered_with<T,T>;


// totally_ordered_with
// --------------------
template <typename T, typename U>
concept totally_ordered_with =
	totally_ordered<T> and
	totally_ordered<U> and
	equality_comparable_with<T, U> and
	totally_ordered<
	    common_reference_t<
		    const remove_reference_t<T>&,
		    const remove_reference_t<U>&
			      >
		       > and
	private_::partially_ordered_with<T, U>;


// object concepts
// ---------------
template <typename T>
concept movable = is_object_v<T> and
		  move_constructible<T> and
		  assignable_from<T&, T> and
		  swappable<T>;

//copyable
//semiregular
//regular
//
// etc...

}// namespace

#endif


