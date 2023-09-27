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
#ifndef __MCU_STD_ITERATOR_CONCEPTS_H__
#define __MCU_STD_ITERATOR_CONCEPTS_H__
/****************************************************************************
 *
 *  DESCRIPCION
 *	Definición de los concepts de los iterators
 *
 *  NOTAS
 *	Escribir todos los concepts lleva bastante tiempo, así que los voy a
 *	ir definiendo poco a poco. Primero los definiré de la forma más básica
 *	para poder usarlos y poco a poco los iré completándolo.
 *
 *  HISTORIA
 *    Manuel Perez
 *	22/08/2023: forward_iterator (v-1)
 *	25/09/2023: indirectly_readable, indirectly_readable_traits
 *
 ****************************************************************************/
#include "std_config.h"
#include "std_private.h"
#include "std_type_traits.h"	// remove_cvref_t
#include "std_utility.h"	// declval
#include "std_concepts.h"	// integral
namespace STD{

// versión del estandar
//template <typename It>
//concept forward_iterator = 
//    input_iterator<It> and
//    derived_from<???<It>, forward_iterator_tag> and
//    incrementable<It> and
//    sentinel_for<It, It>;

// Mi miniversión
template <typename It>
concept forward_iterator = 
    requires(It p){
	++p; p++;
	*p;
    };




// -----------------
// iterator concepts
// -----------------

// indirectly_readable_traits
// --------------------------
namespace private_ {// aqui no queda bien impl_of

// remove_cv_for_object_v<T>:
//	if (is_object_v<T>)
//	    return remove_cv_t<T>;
//	else
//	    return;
//
//  (El standard llama a esta función cond_value_t)
//
template <typename T> 
    struct remove_cv_for_object {};

template <typename T>
    requires is_object_v<T>
struct remove_cv_for_object<T> {
    using value_type = remove_cv_t<T>;
};

}// namespace


// type indirectly_readable_traits<T>:
//	switch(type_of(T)){
//	    case T*	      : return remove_cv_for_object<T>;
//	    case is_array_v<T>: return remove_cv_t<remove_extent_t<T>>;
//	    case 'const T'    : return indirectly_readable_traits<T>;
//	    case exist(T::value_type): 
//				return remove_cv_for_object<T::value_type>;
//	    case exist(T::element_type):
//				return remove_cv_for_object<T::element_type>;
//	    default: return;
//	}
//
//  indirectly_readable_traits<T> == type that can be read using operator*
//
template <typename T>
    struct indirectly_readable_traits {};

template <typename T>
struct indirectly_readable_traits<T*>
	: private_::remove_cv_for_object<T>
{};

template <typename I>
    requires is_array_v<I> 
struct indirectly_readable_traits<I> {
    using value_type = remove_cv_t<remove_extent_t<I>>;
};

template <typename I>
struct indirectly_readable_traits<const I>
	:indirectly_readable_traits<I> {};

template <typename T>
    requires requires {typename T::value_type; }
struct indirectly_readable_traits<T>
    : private_::remove_cv_for_object<typename T::value_type> { };

template <typename T>
    requires requires { typename T::element_type; }
struct indirectly_readable_traits<T>
    : private_::remove_cv_for_object<typename T::element_type> { };


// iter_value
// ----------
// type iter_value_t<T>:
//	type R = remove_cvref_t<T>;
//	if (<iterator_traits<R>> is generated from the primery template)
//	    return indirectly_readable_traits<R>::value_type;
//	else
//	    return iterator_traits<R>::value_type;
//
// ¿Qué significa eso de "is generated from the primary template"?
// Parece ser (referencias???) que la primary template es
//	template <typename It>
//	struct iterator_traits;
//
//  y no
//	template <otros tipos>
//	struct iterator_traits<...>
//
//  Entonces (supongo que) si `iterator_traits<R>` procede de la clase original
//  std::iterator_traits<T>, devuelve
//  indirectly_readable_traits<R>::value_type.
//  En caso contrario devuelve el valor que tenga iterator_traits<R>::value_type.
//

namespace private_{ 

// ¿puedo usar la siguiente implementación? No lo tengo claro:
//template <typename T>
//struct is_specialization_of_iterator_traits : std::false_type {};
//
//template <typename T>
//struct is_specialization_of_iterator_traits<iterator_traits<T>> 
//		: std::true_type {};


// Voy a copiar el estilo de gcc para implementarlo.
// Para saber si iterator_traits<R> procede de la primary template usan el
// truco de que la primary template la estamos implementado como clase
// derivada de __iterator_traits. Como esta clase es de implementación nadie
// debería de usarla, y por tanto, cualquier otra clase iterator_traits<...>
// que no proceda de la primary template no será hija de esta.
//
// iter_traits<It, T>:
//  if (is_base_of<__iterator_traits<It>, iterator_traits<It>>)
//	return T;
//  else
//	return iter_traits<It>;
//
template<typename It>
concept is_generated_from_the_primary_template
    = __is_base_of(__iterator_traits<It>, iterator_traits<It>);

template <typename It, typename T>
struct iter_traits
{ using type = iterator_traits<It>; };


template <typename It, typename T>
    requires is_generated_from_the_primary_template<It>
struct iter_traits<It, T>
{ using type = T; };


template<typename It, typename T = It>
using iter_traits_t = typename iter_traits<It, T>::type;


template<typename T>
using iter_value_t =
	typename iter_traits_t<T, indirectly_readable_traits<T>>::value_type;

}// namespace private_
 
template <typename T>
using iter_value_t = private_::iter_value_t<remove_cvref_t<T>>;


// iter_reference_t
// ----------------
template<private_::dereferenceable T>
using iter_reference_t = decltype(*declval<T&>());


// incrementable_traits
// --------------------
// Este es copia literal del estandard (ya que lo detallan tanto, lo podían
// dar en código)
//
// type incrementable_traits<T>:
//	switch(T){
//	    case T*: return ptrdiff_t;
//	    case `const I`: return incrementable_traits<I>;
//	    case exist(T::difference_type): return T::difference_type;
//	    case !exist(T::difference_type)
//		    and (a-b) -> integral : signed(a - b);
//	    default: return;
//	}
template <typename T>
struct incrementable_traits {};

template <typename T>
    requires is_object_v<T>
struct incrementable_traits<T*>{
    using difference_type = ptrdiff_t;
};

template <typename I>
struct incrementable_traits<const I>
    : incrementable_traits<I> {};

template <typename T>
    requires requires { typename T::difference_type; }
struct incrementable_traits<T> {
    using difference_type = typename T::difference_type;
};

template <typename T>
    requires (!requires { typename T::difference_type; } 
		    and
	       requires(const T& a, const T& b) { {a - b} -> integral; })
struct incrementable_traits<T> {
    using difference_type = 
		    make_signed_t<decltype(declval<T>() - declval<T>())>;
};
	      

// iter_difference_t
// -----------------
namespace private_{

template<typename T>
using iter_difference_t = 
	typename iter_traits_t<T, incrementable_traits<T>>::difference_type;

}// namespace
template<typename T>
using iter_difference_t = private_::iter_difference_t<remove_cvref_t<T>>;


// indirectly_readable
// -------------------
// La siguiente implementación es copia de la suministrada por el estandard
// (deja poco a la imaginación)
namespace impl_of{  // implementation_of::indirectly_readable!! <-- se lee bien

template <typename In> // ¿por qué lo llama In el standard?
concept indirectly_readable =
    requires(const In in){
	typename iter_value_t<In>;
	typename iter_reference_t<In>;
// TODO: iter_rvalue_reference_t depende de ranges.
//	typename iter_rvalue_reference_t<In>;
	{ *in } -> same_as<iter_reference_t<In>>;
	// TODO: no tengo implementado ranges,
	// { ranges::iter_move(in) } -> same_as<iter_reference_t<In>>;
    } 
	and
    common_reference_with<iter_reference_t<In>&&, iter_value_t<In>&> ;
//	and
//    common_reference_with<iter_reference_t<In>&&, iter_rvalue_reference_t<In>&&>
//	and
//    common_reference_with<iter_rvalue_reference_t<In>&&, const iter_value<In>&>
    
}// namespace 

template <typename In>
concept indirectly_readable = impl_of::indirectly_readable<remove_cvref_t<In>>;

// iter_const_reference_t
// ----------------------
template<indirectly_readable T>
using iter_const_reference_t 
	= common_reference_t<const iter_value_t<T>&&, iter_reference_t<T>>;

}// namespace STD
#endif



