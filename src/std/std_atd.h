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
 *    13/10/2019 int_to_cstring (definido en std_cstdio.h)
 *    26/09/2023 dereferenceable
 *    04/10/2024 int_to_cstring (metido en aquí para poder usarlo en atd)
 *
 ****************************************************************************/
#include "std_config.h"
#include "std_cstddef.h"    // size_t, nullptr_t

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


// is_const
// --------
template <typename T>
struct is_const : public false_type { };

template <typename T>
struct is_const<T const> : public true_type { };

template <typename T>
inline constexpr bool is_const_v = is_const<T>::value;

// is_volatile
// -----------
template <typename T>
struct is_volatile : public false_type { };

template <typename T>
struct is_volatile<T volatile> : public true_type { };

template <typename T>
inline constexpr bool is_volatile_v = is_volatile<T>::value;


// void_t
// ------
template <typename...>
using void_t = void;


namespace atd_{

// is_a_type
// ---------
// Nos dice si una expresión es un tipo o no
template <typename T>
concept is_a_type = 
    requires {
	typename T;
    };

// has_destructor
// --------------
template <typename T>
concept has_destructor =
	requires (T t){ t.~T();};


// -----------
// type_member
// -----------
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


// add_reference
// -------------
template<typename T>
using add_reference = T&;


// is_referenceable
// ----------------
template<typename T>
concept is_referenceable = requires { typename add_reference<T>; };


// dereferenceable
// ---------------
template<typename T>
concept dereferenceable = 
	requires(T& t)
	{
	    { *t } -> is_referenceable;
	};


// can_reference
// -------------
template <typename T>
concept can_reference = requires{ typename add_reference<T>; };

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





// type copy_cv<type From, type To> 
// {
//	type res = To;
//	
//	if (is_const_v<From>)
//	    res = add_const_t<res>;
//
//	if (is_volatile_v<From>)
//	    res = add_volatile_t<res>;
//
//	return res;
// }
//
// (impl) ¿Cómo implementarlo? 
// Como los condicionales se basan en 2 bools y no en tipos, 
// una forma de hacerlo es pasando esos valores como parámetros de template.
template <typename From, typename To,
	  bool isConst = is_const_v<From>,
	  bool isVolatile = is_volatile_v<From>>
struct copy_cv;

template <typename From, typename To>
struct copy_cv<From, To, false, false>{
    using type = To;
};

template <typename From, typename To>
struct copy_cv<From, To, false, true>{
    using type = volatile To;
};

template <typename From, typename To>
struct copy_cv<From, To, true, false>{
    using type = const To;
};

template <typename From, typename To>
struct copy_cv<From, To, true, true>{
    using type = const volatile To;
};

template <typename From, typename To>
using copy_cv_t = typename copy_cv<From, To>::type;




namespace private_{
// posint_to_cstring
// -----------------
/// Transforma el entero x en una cadena tipo c, almacenándola en el buffer 
/// [p0, pe). Devuelve la posición pc en donde EMPIEZA el entero como cadena
/// El número x como cadena estará almacenado en [pc, pe).
/// Precondición: x > 0
/// Es el equivalente a sprintf.
template <typename It, typename Int>
It posint_to_cstring(It p0, It pe, Int x)
{
    It pc = pe;

    while (x != 0 and pc != p0){
	Int r = x % 10;
	x = (x -  r) / 10;
	
	--pc;
	*pc = '0' + r;	// to_ascii(r)
    }

    return pc;
}
} // namespace private_

// int_to_cstring
// --------------
/// Transforma el entero x en una cadena tipo c, almacenándola en el buffer 
/// [p0, pe). Devuelve la posición pc en donde EMPIEZA el entero como cadena
/// El número x como cadena estará almacenado en [pc, pe).
///
/// Si Int es signed esta función falla para el caso en que valga
/// std::numeric_limits::min() ya que falla el operador -x para este caso.
/// Es el equivalente a sprintf.
template <typename It, typename Int>
It int_to_cstring(It p0, It pe, Int x)
{
    if (p0 == pe)
	return p0;

    It pc = pe; // = ultima posición donde insertamos una cifra
    
    if (x == 0){
	--pc;
	*pc = '0';
	return pc;
    }

    bool negativo  = false;
    if (x < 0){
	negativo = true;
	x = -x;	// Esta operación falla si x == std::numeric_limits::min()!!!
    }

    pc = private_::posint_to_cstring(p0, pe, x);

    if (negativo and pc != p0){
	--pc;
	*pc = '-';
    }

    return pc;
}

}// namespace atd_


}// namespace STD
#endif


