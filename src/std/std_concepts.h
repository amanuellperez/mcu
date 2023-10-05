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
 *    05/10/2023 Escribo bastantes.
 *
 ****************************************************************************/
#include "std_config.h"
#include "std_type_traits.h"
#include "std_utility.h"    // STD::forward

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

// common_with
// -----------
template <typename T, typename U>
concept common_with =
	same_as< common_type_t<T, U>, common_type_t<U, T> > and
	requires {
	    static_cast<common_type_t<T, U>>(STD::declval<T>());
	    static_cast<common_type_t<T, U>>(STD::declval<U>());
	} and
	common_reference_with<add_lvalue_reference_t<const T>,
			      add_lvalue_reference_t<const U>> and
	common_reference_with<
		    add_lvalue_reference_t< common_type_t<T,U> >,
		    common_reference_t< add_lvalue_reference_t<const T>,
				        add_lvalue_reference_t<const U> >
			    >;


// -------------------
// arithmetic concepts
// -------------------
// is_integral
// ----------
template <typename T>
concept integral = is_integral_v<T>;

// signed_integral
// ---------------
template <typename T>
concept signed_integral = integral<T> and is_signed_v<T>;

// unsigned_integral
// -----------------
template <typename T>
concept unsigned_integral = integral<T> and !signed_integral<T>;


// floating_point
// --------------
template <typename T>
concept floating_point = is_floating_point_v<T>;


// assignable_from
// ---------------
template <typename L, typename R>
concept assignable_from =
	is_lvalue_reference_v<L> and
	common_reference_with<const remove_reference_t<L>&, 
			      const remove_reference_t<R>&> and
	requires(L lhs, R&& rhs){
	    { lhs = STD::forward<R>(rhs) } -> same_as<L>;
	};


// swappable
// ---------
// Implementado más adelante por dependencias.

// destructible
// ------------
template <typename T>
concept destructible = is_nothrow_destructible_v<T>;


// constructible_from
// ------------------
template <typename T, typename... Args>
concept constructible_from = destructible<T> and
			     is_constructible_v<T, Args...>;

// default_initializable
// ---------------------
template <typename T>
concept default_initializable = constructible_from<T> and
				requires { 
				    T{}; 
				    (void) ::new T;
				};


// move_constructible
// ------------------
template <typename T>
concept move_constructible = constructible_from<T,T> and
			     convertible_to<T, T>;
	
// swappable
// ---------
// TODO: esta implementación sigue la línea de gcc.
// Aunque la he intentado aclarar un poco, sigue muy liada para mi gusto.
// Simplificarla!
namespace private_{
template<typename T>
concept class_or_enum = is_class_v<T> or 
			is_union_v<T> or
			is_enum_v<T>;
}// private_
 
namespace ranges {
namespace impl_of_swap {

template<typename T> void swap(T&, T&) = delete;

template<typename T, typename U>
concept adl_swap = 
    ( private_::class_or_enum<remove_reference_t<T>> or
			    private_::class_or_enum<remove_reference_t<U>>) and
    requires(T&& t, U&& u) {
	    swap(static_cast<T&&>(t), static_cast<U&&>(u));
	};

struct Swap
{
private:
    template<typename T, typename U>
    static constexpr bool swap_noexcept()
    {
	if constexpr (adl_swap<T, U>)
	    return noexcept(swap(STD::declval<T>(), STD::declval<U>()));

	else
	    return is_nothrow_move_constructible_v<remove_reference_t<T>> and
		   is_nothrow_move_assignable_v<remove_reference_t<T>>;
    }

public:
    template<typename T, typename U>
    requires adl_swap<T, U> or 
	    (same_as<T, U> && is_lvalue_reference_v<T> and 
	     move_constructible<remove_reference_t<T>>  and 
	     assignable_from<T, remove_reference_t<T>>
	     )
    constexpr 
    void operator()(T&& t, U&& u) const noexcept(swap_noexcept<T, U>())
    {
        if constexpr (adl_swap<T, U>)
	    swap(static_cast<T&&>(t), static_cast<U&&>(u));
	else
	{
	    auto tmp = static_cast<remove_reference_t<T>&&>(t);
	    t = static_cast<remove_reference_t<T>&&>(u);
	    u = static_cast<remove_reference_t<T>&&>(tmp);
	}
    }

    template<typename T, typename U, size_t _Num>
	requires 
	    requires(const Swap& swap, T& e1, U& e2) { swap(e1, e2); }
    constexpr 
    void operator()(T (&e1)[_Num], U (&e2)[_Num]) const
		    noexcept(noexcept(STD::declval<const Swap&>()(*e1, *e2)))
    {
	for (size_t n = 0; n < _Num; ++n)
	    (*this)(e1[n], e2[n]);
    }
};
} // namespace impl_of_swap

inline namespace cust
{
  inline constexpr impl_of_swap::Swap swap{};
} // inline namespace cust

} // ranges

template<typename T>
concept swappable = requires(T& a, T& b) { ranges::swap(a, b); };


// swappable_with
// --------------
template<typename T, typename U>
concept swappable_with = common_reference_with<T, U> and
	requires(T&& t, U&& u) {
		ranges::swap(STD::forward<T>(t), STD::forward<T>(t));
		ranges::swap(STD::forward<U>(u), STD::forward<U>(u));
		ranges::swap(STD::forward<T>(t), STD::forward<U>(u));
		ranges::swap(STD::forward<U>(u), STD::forward<T>(t));
	    };



// copy_constructible
// ------------------
template <typename T>
concept copy_constructible = 
	move_constructible<T> and
	constructible_from<T, T&>	and convertible_to<T&, T> and
	constructible_from<T, const T&> and convertible_to<const T&, T> and
	constructible_from<T, const T>  and convertible_to<const T, T>;


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


// ---------------
// object concepts
// ---------------
// movable
// -------
template <typename T>
concept movable = is_object_v<T> and
		  move_constructible<T> and
		  assignable_from<T&, T> and
		  swappable<T>;

// copyable
// --------
template <typename T>
concept copyable = copy_constructible<T> and
		   movable<T> and
		   assignable_from<T&, T&> and
		   assignable_from<T&, const T&> and
		   assignable_from<T&, const T>;

// semiregular
// -----------
template <typename T>
concept semiregular = copyable<T> and
		      default_initializable<T>;

// regular
// -------
template <typename T>
concept regular = semiregular<T> and
		  equality_comparable<T>;



// -----------------
// callable concepts
// -----------------
// invocable
// ---------
// TODO

// regular_invocable
// -----------------
// TODO

// predicate
// ---------
// TODO

// relation
// --------
// TODO

// equivalence_relation
// --------------------
// TODO

// strict_weak_order
// -----------------
// TODO


}// namespace

#endif


