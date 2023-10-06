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
 *	05/10/2023: random_access_iterator_tag
 *
 ****************************************************************************/
#include "std_config.h"
#include "std_atd.h"
#include "std_type_traits.h"	// remove_cvref_t
#include "std_concepts.h"	// integral
#include "std_iterator.h"

// TODO: esto hay que ordenarlo. Hay un montón de dependencias circulares
#include "std_utility.h"	// declval
#include "std_memory.h"		// to_address
namespace STD{


// -------
// General
// -------
namespace private_ {
// is_specialization_of_iterator_traits
// ------------------------------------
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
// iter_traits<I, T>:
//  if (is_base_of<__iterator_traits<I>, iterator_traits<I>>)
//	return T;
//  else
//	return iter_traits<I>;
//
template<typename I>
concept is_specialization_of_iterator_traits_generated_primary_template 
    = __is_base_of(__iterator_traits<I>, iterator_traits<I>);


// ITER_TRAITS
// -----------
template<typename I, typename T>
struct iter_traits
{ using type = iterator_traits<I>; };

template<typename I, typename T>
    requires is_specialization_of_iterator_traits_generated_primary_template<I>
struct iter_traits<I, T>
{ using type = T; };
 
template<typename I, typename T = I>
using iter_traits_t = typename iter_traits<I, T>::type;


// ITER_CONCEPT
// ------------
template<typename I>
struct iter_concept;

// If the qualified-id ITER_TRAITS(I)::iterator_concept is valid and names 
// a type, then ITER_CONCEPT(I) denotes that type.
template<typename I>
    requires requires { typename iter_traits_t<I>::iterator_concept; }
struct iter_concept<I>{ 
    using type = typename iter_traits_t<I>::iterator_concept; 
};

// Otherwise, if the qualified-id ITER_TRAITS(I)::iterator_category is 
// valid and names a type, then ITER_CONCEPT(I) denotes that type.
template<typename I>
    requires (!requires { typename iter_traits_t<I>::iterator_concept; } and 
	       requires { typename iter_traits_t<I>::iterator_category; })
struct iter_concept<I>{ 
    using type = typename iter_traits_t<I>::iterator_category; 
};

// Otherwise, if iterator_traits<I> names a specialization generated from 
// the primary template, then ITER_CONCEPT(I) denotes 
// random_access_iterator_tag.
template<typename I>
    requires (!requires { typename iter_traits_t<I>::iterator_concept; } and 
	      !requires { typename iter_traits_t<I>::iterator_category; } and
	      is_specialization_of_iterator_traits_generated_primary_template<I>)
struct iter_concept<I>{ 
    using type = random_access_iterator_tag;
};

// Otherwise, ITER_CONCEPT(I) does not denote a type.
template<typename I>
struct iter_concept { };

template <typename I>
using iter_concept_t = typename iter_concept<I>::type;



}// private_


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
//	template <typename I>
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


template<typename T>
using iter_value_t =
	typename iter_traits_t<T, indirectly_readable_traits<T>>::value_type;

}// namespace private_
 
template <typename T>
using iter_value_t = private_::iter_value_t<remove_cvref_t<T>>;


// iter_reference_t
// ----------------
template<atd_::dereferenceable T>
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
	// TODO: no tengo implementado ranges::iter_move
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


// weakly_incrementable
// --------------------
// TODO: revisar. Los de gcc incluyen que T pueda ser __max_diff_type.
// __max_diff_type es una clase interna definida en bits/max_size_type.h
// ¿Para qué sirve?
namespace private_{
template<typename T>
concept is_signed_integer_like = signed_integral<T>; // or
		//		 same_as<T, __max_diff_type>;
}// private_
 
template <typename I>
concept weakly_incrementable =
	   default_initializable<I> and
	   movable<I> and
	   requires(I i) {
	       typename iter_difference_t<I>;
	       requires private_::is_signed_integer_like<iter_difference_t<I>>;
	       { ++i } -> same_as<I&>;
	       i++;
	   };

	

// incrementable
// -------------
template <typename I>
concept incrementable = regular<I> and
			weakly_incrementable<I> and
			requires(I i) {
			    { i++ } -> same_as<I>;
			};

// input_or_output_iterator
// ------------------------
template <typename I>
concept input_or_output_iterator = 
	    requires(I i) {
		{ *i } -> atd_::can_reference;
	    } and
	    weakly_incrementable<I>;

// sentinel_for
// ------------
template <typename S, typename I>
concept sentinel_for = semiregular<S> and
		       input_or_output_iterator<I> and
		       private_::weakly_equality_comparable_with<S, I>;

// disable_size_sentinel_for
// -------------------------
template <typename S, typename I>
inline constexpr bool disable_size_sentinel_for = false;

// size_sentinel_for
// -----------------
template <typename S, typename I>
concept sized_sentinel_for =
	sentinel_for<S, I> and
	!disable_size_sentinel_for<remove_cv_t<S>, remove_cv_t<I>> and
	requires (const I& i, const S& s) {
	    { s - i } -> same_as<iter_difference_t<I>>;
	    { i - s } -> same_as<iter_difference_t<I>>;
	};


// input_iterator
// --------------
template <typename I>
concept input_iterator = 
	    input_or_output_iterator<I> and
	    indirectly_readable<I> and
	    requires { typename private_::iter_concept_t<I>; } and
	    derived_from<private_::iter_concept_t<I>, input_iterator_tag>;


// forward_iterator
// ----------------
template <typename I>
concept forward_iterator = 
	input_iterator<I> and
	derived_from<private_::iter_concept_t<I>, forward_iterator_tag> and
	incrementable<I> and
	sentinel_for<I, I>;

// bidirectional_iterator
// ----------------------
template <typename I>
concept bidirectional_iterator = 
	forward_iterator<I> and
	derived_from<private_::iter_concept_t<I>, bidirectional_iterator_tag> and
	requires(I i) {
	    { --i } -> same_as<I&>;
	    { i-- } -> same_as<I>;
	};


// random_access_iterator
// ----------------------
template <typename I>
concept random_access_iterator =
	bidirectional_iterator<I> and
	derived_from<private_::iter_concept_t<I>, random_access_iterator_tag> and
	totally_ordered<I> and
	sized_sentinel_for<I, I> and
	requires(I i, const I j, const iter_difference_t<I> n){
	    { i += n } -> same_as<I&>;
	    { j + n  } -> same_as<I>;
	    { n + j  } -> same_as<I>;
	    { i -= n } -> same_as<I&>;
	    { j - n  } -> same_as<I>;
	    {  j[n]  } -> same_as<iter_reference_t<I>>;
	};


// contiguous_iterator
// -------------------
template <typename I>
concept contiguous_iterator = 
	random_access_iterator<I> and
	derived_from<private_::iter_concept_t<I>, contiguous_iterator_tag> and
	is_lvalue_reference_v<iter_reference_t<I>> and
	same_as<iter_value_t<I>, remove_cvref_t<iter_reference_t<I>>> and
	requires(const I& i){
	    { STD::to_address(i) } -> same_as<add_pointer_t<iter_reference_t<I>>>;
	};

}// namespace STD
#endif



