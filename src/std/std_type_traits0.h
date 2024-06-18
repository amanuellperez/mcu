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

#ifndef __MCU_STD_FIRST_H__
#define __MCU_STD_FIRST_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Archivo de cabecera interno.
 *	La distribución de funciones en los archivos de cabecera dadas por el
 *	estandar genera dependencias circulares.
 *	Ejemplo: en type_traits.h se define is_swappable_with que depende de
 *	la función swap, que se define en algorithm que depende ... Al final
 *	se genera una dependencia circular que no se puede compilar.
 *	Además si quieres definir swap, necesitas unos requires que dependen
 *	de traits,  que dependen de ...
 *
 *	Con este archivo pretendo romper esas dependencias circulares.
 *	(en realidad:
 *	    1º.- Es verdad que intento eliminar dependencias circulares.
 *	    2º.- Los requires de swap son los que determinan qué funciones 
 *		 debo meter aquí. Así que, de momento, este archivo gira en
 *		 torno a swap.
 *	)
 *
 * HISTORIA
 *    Manuel Perez
 *    04/10/2023 Escrito
 *
 ****************************************************************************/
#include "std_atd.h"

namespace STD{

// declval
// -------
// Implementación propuesta de Eric Niebler:
// https://bugs.llvm.org/show_bug.cgi?id=27798. Ver discusión en:
// https://stackoverflow.com/questions/56212117/understanding-declval-optimized-implementation
namespace impl_of{
template<typename T, typename Up = T&&>
Up declval(int); 

template<typename T>
T declval(long);

// (???) ¿Qué es esto? 
// Los de gcc lo tienen puesto. Lo he descubierto al ver que fallaba un test
// en un trait, pero desconozco de dónde procede. 
template<typename Tp>
struct declval_protector{
    static const bool stop = false; 
};

}// impl_of
 
//template<typename T>
//auto declval() noexcept -> decltype(impl_of::declval<T>(0));

// (???) ¿Qué es esto? 
// Los de gcc lo tienen puesto. Lo he descubierto al ver que fallaba un test
// en un trait, pero desconozco de dónde procede. El comentario de gcc es:
//
// Utility to simplify expressions used in unevaluated operands
// since C++11
//
// El static_assert siempre es true, con lo que es imposible instanciar esta
// función.
template<typename T>
auto declval() noexcept -> decltype(impl_of::declval<T>(0))
{
  static_assert(impl_of::declval_protector<T>::stop,
		"declval() must not be used!");
  return impl_of::declval<T>(0);
}

// type_identity
// -------------
template <typename T>
struct type_identity {
    using type = T;
};

template <typename T>
using type_identity_t = typename type_identity<T>::type;

// is_same
// -------
template <typename X, typename Y>
struct is_same : public false_type {};

template <typename X>
struct is_same<X, X> : public true_type {};

template <typename X, typename Y>
inline constexpr bool is_same_v = is_same<X, Y>::value;

// remove_const
// ------------
template <typename T>
struct remove_const {
    using type = T;
};

template <typename T>
struct remove_const <const T> { using type = T; };

template <typename T>
using remove_const_t = typename remove_const<T>::type;


// remove_volatile
// ---------------
template <typename T>
struct remove_volatile {using type = T; };

template <typename T>
struct remove_volatile <volatile T> {using type = T;};

template <typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

// remove_cv
// ---------
template <typename T>
struct remove_cv {
    using type = remove_volatile_t<remove_const_t<T>>;
};


template <typename T>
using remove_cv_t = typename remove_cv<T>::type;

// is_void
// -------
namespace impl_of{
template <typename T>
constexpr inline bool is_void()
{
    using U = remove_cv_t<T>;

    if (is_same_v<U, void>)
	return true;
    else
	return false;
}
}// impl_of
 
template <typename T>
struct is_void : public bool_constant<impl_of::is_void<T>()> {};

template <typename T>
inline constexpr bool is_void_v = is_void<T>::value;


// is_array
// --------
template <typename T>
struct is_array : public false_type { };

template <typename T, size_t sz>
struct is_array<T[sz]> : public true_type { };

template <typename T>
struct is_array<T[]> : public true_type { };

template <typename T>
inline constexpr bool is_array_v = is_array<T>::value;

// extent
// ------
// size_t extent<type T, unsigned N = 0>()
// {
//	if (!is_array_v<T> or 
//	    rank<T> <= N or 
//	    (N = 0 and is_array_of_unknown_bound<T>)
//	    return 0;
//	else
//	    "the bound of the Nth dimension of T, where indexing of N
//	     is zero-base";
// }
// Esa es la definición del estandard. Está más claro cppreference.
//
template <typename T, unsigned N>
struct extent : integral_constant<size_t, 0> { };

template<typename T>
struct extent<T[], 0> : integral_constant<size_t, 0>
{ };

template<typename T, unsigned N>
struct extent<T[], N> : extent<T, N - 1>
{ };

template<typename T, size_t sz>
struct extent<T[sz], 0> : integral_constant<size_t, sz>
{ };

template<typename T, size_t sz, unsigned N>
struct extent<T[sz], N> : extent<T, N - 1>
{ };


template <typename T, unsigned N = 0>
inline constexpr size_t extent_v = extent<T, N>::value;


// is_function
// -----------
// Esta función la copié de gcc o cppreference, no recuerdo.
// Revisándola no hay mucho que reescribir.
// primary template
template <typename>
struct is_function : false_type {
};

// specialization for regular functions
template <typename Ret, typename... Args>
struct is_function<Ret(Args...)> : true_type {
};

// specialization for variadic functions such as printf
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...)> : true_type {
};

// specialization for function types that have cv-qualifiers
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) volatile> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const volatile> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) volatile> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const volatile> : true_type {
};

// specialization for function types that have ref-qualifiers
template <typename Ret, typename... Args>
struct is_function<Ret(Args...)&> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const&> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) volatile&> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const volatile&> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...)&> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const&> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) volatile&> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const volatile&> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) &&> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const&&> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) volatile&&> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const volatile&&> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) &&> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const&&> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) volatile&&> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const volatile&&> : true_type {
};

// specializations for noexcept versions of all the above (C++17 and later)
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) volatile noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const volatile noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) volatile noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const volatile noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) & noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const& noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) volatile& noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const volatile& noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) & noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const& noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) volatile& noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const volatile& noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) && noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const&& noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) volatile&& noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const volatile&& noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) && noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const&& noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) volatile&& noexcept> : true_type {
};
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const volatile&& noexcept> : true_type {
};

template <typename T>
inline constexpr bool is_function_v = is_function<T>::value;



// is_lvalue_reference
// -------------------
template <typename T>
struct is_lvalue_reference : public false_type { };

template <typename T>
struct is_lvalue_reference<T&> : public true_type { };

template <typename T>
inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

// is_rvalue_reference
// -------------------
template <typename T>
struct is_rvalue_reference : public false_type { };

template <typename T>
struct is_rvalue_reference<T&&> : public true_type { };

template <typename T>
inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;



// is_reference
// ------------
namespace impl_of{
template <typename T>
constexpr inline bool is_reference()
{
    if (is_rvalue_reference_v<T> or is_lvalue_reference_v<T>)
	return true;

    return false;
}
} // namespace
  
 
template <typename T>
struct is_reference : public bool_constant<impl_of::is_reference<T>()>{ };

template <typename T>
inline constexpr bool is_reference_v = is_reference<T>::value;


// remove_reference
// ----------------
template <typename T>
struct remove_reference{ using type = T; };

template <typename T>
struct remove_reference<T&> { using type = T;};

template <typename T>
struct remove_reference<T&&> { using type = T;};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

namespace private_{
// is_array_known_bounds
// ---------------------
// DUDA: Los de gcc no comprueban que sea array. ¿queda incluido en extent?
template <typename T>
struct is_array_known_bounds
    //: atd_::static_and<is_array_v<T>, extent_v<T> > 0>
    : bool_constant<(extent_v<T> > 0)>
{ };

// is_array_unknown_bounds
// -----------------------
template <typename T>
struct is_array_unknown_bounds
    : atd_::static_and<is_array_v<T>, extent_v<T> == 0>
{ };

template <typename T>
inline constexpr bool is_array_unknown_bounds_v 
					= is_array_unknown_bounds<T>::value;

// is_complete_or_unbounded
// ------------------------
template <typename T, size_t = sizeof(T)>
constexpr true_type is_complete_or_unbounded(type_identity<T>)
{ return true_type{}; }

template <typename T,
	  typename N = typename T::type>
constexpr atd_::static_or<is_reference_v<N>,
		    is_function_v<N>,
		    is_void_v<N>,
		    is_array_unknown_bounds_v<N>
		   >
	    is_complete_or_unbounded(T)
{ return {}; }


}// private_




// is_move_constructible
// ---------------------
namespace impl_of{
template <typename T>
constexpr bool is_move_constructible()
{
    if constexpr (atd_::is_referenceable<T>)
	return __is_constructible(T, T&&);

    else
	return false;
}

}// impl_of

template<typename T>
struct is_move_constructible
	: bool_constant<impl_of::is_move_constructible<T>()>
{
  static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T>
inline constexpr bool is_move_constructible_v = is_move_constructible<T>::value;


// is_move_assignable
// ------------------
namespace impl_of{
template <typename T>
constexpr bool is_move_assignable()
{
    if constexpr (atd_::is_referenceable<T>)
	return __is_assignable(T&, T&&);

    else
	return false;
}

}// impl_of

template<typename T>
struct is_move_assignable
	: bool_constant<impl_of::is_move_assignable<T>()>
{
  static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T>
inline constexpr bool is_move_assignable_v = is_move_assignable<T>::value;



// is_nothrow_move_constructible
// ------------------------------
namespace impl_of{

template <typename T>
constexpr bool is_nothrow_move_constructible()
{
    if constexpr (atd_::is_referenceable<T>)
	return __is_nothrow_constructible(T, T&&);

    else
	return false;
}

}// impl_of
 
template<typename T>
struct is_nothrow_move_constructible
    : bool_constant<impl_of::is_nothrow_move_constructible<T>()>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};


template <typename T>
inline constexpr bool is_nothrow_move_constructible_v
				=   is_nothrow_move_constructible<T>::value;

// is_nothrow_move_assignable
// --------------------------
namespace impl_of{

template <typename T>
constexpr bool is_nothrow_move_assignable()
{
    if constexpr (atd_::is_referenceable<T>)
	return __is_nothrow_assignable(T&, T&&);

    else
	return false;
}

}// impl_of
 
template<typename T>
struct is_nothrow_move_assignable
    : bool_constant<impl_of::is_nothrow_move_assignable<T>()>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};


template <typename T>
inline constexpr bool is_nothrow_move_assignable_v
				=   is_nothrow_move_assignable<T>::value;





// forward
// -------
template <typename T>
constexpr T&& forward(remove_reference_t<T>& t) noexcept
{ return static_cast<T&&>(t); }

template <typename T>
constexpr T&& forward(remove_reference_t<T>&& t) noexcept
{
    static_assert(!is_lvalue_reference_v<T>, 
		    "template argument is an lvalue reference type");

    return static_cast<T&&>(t); 
}

// move
// ----
template <typename T>
constexpr remove_reference_t<T>&& move(T&& t) noexcept
{ return static_cast<remove_reference_t<T>&&>(t); }

// swap
// ----
template <typename T>
struct is_swappable;

template <typename T>
inline constexpr bool is_swappable_v = is_swappable<T>::value;

// ¿por qué pongo STD::move cuando no es necesario STD aqui?
// En los test de pruebas comparo STD::swap con std::swap. Si pruebo a hacer
// un swap con std::string el compilador se queja porque no sabe a que move
// estoy llamando. Por eso lo pongo de forma explícita, para poder probar con
// std::string.
template <typename T>
    requires (is_move_constructible_v<T> and
	      is_move_assignable_v<T> //and
	      // is_tuple_like<T> and   TODO: los de gcc tienen este require
	    )
constexpr void swap(T& a, T& b)
// TODO: los de gcc añaden algo parecido a :
//	 noexcept(requires (is_nothrow_move_constructible_v<T> and
//				is_nothrow_move_assignable_v<T>))
{
    T tmp = STD::move(a);
    a = STD::move(b);
    b = STD::move(tmp);
}

// TODO: no está implementado swap_ranges, así que si se llama a esta función
// fallará
template <typename T, size_t N>
    requires (is_swappable_v<T>)
inline 
constexpr void swap(T (&a)[N], T (&b)[N])// TODO: noexcept(...)
{ swap_ranges(a, a + N, b); }


// is_swappable
// ------------
namespace impl_of{
template<typename T>
constexpr bool is_swappable()
{
    if constexpr 
	    (requires { STD::swap(STD::declval<T&>(), STD::declval<T&>()); })
	return true;

    else
	return false;
}


}// impl_of

template<typename T>
struct is_swappable
    : bool_constant<impl_of::is_swappable<T>()>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "First template argument must be a complete class or an unbounded array");
};


}// namespace
 
#endif
