// Copyright (C) 2019-2023 Manuel Perez 
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

#ifndef __MCU_STD_TYPE_TRAITS_H__
#define __MCU_STD_TYPE_TRAITS_H__
/****************************************************************************
 *
 *  DESCRIPCION
 *	Corresponde con el type_traits del estandar
 *
 *  EJEMPLOS
 *	OBSOLETO? 
 *	Para ver como implementar un switch de tipos ver 
 *	common_type o common_reference. Para ver cómo implementar un trait
 *	usando funciones ver is_destructible (de hecho, seguramente muchos
 *	traits se puedan implementar usando ese estilo).
 *
 *	MODERNO
 *	Los últimos traits ya los he escrito usando `requires`. Hay unos
 *	cuantos. Ver por ejemplo, `is_convertible`. Con los requires se 
 *	entiende mucho mejor el código con lo que es más sencillo de escribir
 *	y mantener. 
 *
 *  HISTORIA
 *    Manuel Perez
 *    07/02/2019 v0.0
 *    22/10/2019 is_const_v, is_volatile_v, conditional_t
 *    03/11/2019 void_t
 *    08/12/2019 add_rvalue_reference/add_lvalue_reference/is_function
 *		 add_pointer/remove_pointer, ...
 *    18/03/2021 make_signed/make_unsigned
 *    14/08/2021 Completando implementación de common_type.
 *               Es copia de cppreference.
 *    23/12/2022 type_identity
 *    25/09/2023 remove_cvref, is_union, is_class, is_base_of,
 *               add_const/volatile/cv
 *               common_reference 
 *               (¿Cómo probar common_reference? La definición del estandard
 *               no está nada clara, ya que dice cómo funciona por dentro pero
 *               no dice para qué sirve. Ni siquiera en cppreference se
 *               encuentran ejemplos de cómo validar esta función. Pero sin
 *               tests lo más probable es que tenga un montón de errores!!!
 *               Forma de probarla: en caso de duda intentar una enumeración
 *               exhaustiva }:), ver el test)
 *    05/10/2023 Escribo bastantes.
 *		 
 *
 ****************************************************************************/
#include "std_config.h"
#include "std_type_traits0.h"

#include "std_cstddef.h"    // size_t, nullptr_t
namespace STD{

// is_same
// -------
// Implementado en std_type_traits0.h

// declval
// -------
// Implementado en std_type_traits0.h


// ----------------------------
// const-volatile modifications
// ----------------------------
// remove_const
// ------------
// Implementado en std_type_traits0.h

// remove_volatile
// ---------------
// Implementado en std_type_traits0.h

// remove_cv
// ---------
// Implementado en std_type_traits0.h


// add_const
// ---------
template <typename T>
struct add_const {
    using type = const T;
};

template <typename T>
using add_const_t = typename add_const<T>::type;

// add_volatile
// ------------
template <typename T>
struct add_volatile {
    using type = volatile T;
};

template <typename T>
using add_volatile_t = typename add_volatile<T>::type;

// add_cv
// ------
template <typename T>
struct add_cv {
    using type = const volatile T;
};

template <typename T>
using add_cv_t = typename add_cv<T>::type;



// -----------------------
// primary type categories
// -----------------------

// is_void
// -------
// Implementado en std_type_traits0.h

// is_null_pointer
// ---------------
namespace impl_of{
template <typename T>
constexpr bool is_null_pointer()
{
    // Los de GCC hacen el remove_cv_t (LWG 2247)
    if constexpr (is_same_v<remove_cv_t<T>, nullptr_t>)
	return true;

    else
	return false;
}
}// impl_of
 
template <typename T>
struct is_null_pointer : bool_constant<impl_of::is_null_pointer<T>()> { };


template <typename T>
inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;


// is_integral
// -----------
// Esta función asocia a cada tipo (sin const ni volatile) true or false
// dependiendo de si es o no integral type
// Observar que se incluye char como tipo integral, pero... ¡es un caracter,
// no un número!!! (legacy code???)
namespace impl_of{
template <typename>
struct is_integral:  false_type {};

template <>
struct is_integral<bool> :  true_type {};

template <>
struct is_integral<char> :  true_type {};

template <>
struct is_integral<signed char> :  true_type {};

template <>
struct is_integral<unsigned char> :  true_type {};

template <>
struct is_integral<char16_t> :  true_type {};

template <>
struct is_integral<char32_t> :  true_type {};

template <>
struct is_integral<short> :  true_type {};

template <>
struct is_integral<unsigned short> :  true_type {};

template <>
struct is_integral<int> :  true_type {};

template <>
struct is_integral<unsigned int> :  true_type {};

template <>
struct is_integral<long> :  true_type {};

template <>
struct is_integral<unsigned long> :  true_type {};

template <>
struct is_integral<long long> :  true_type {};

template <>
struct is_integral<unsigned long long> :  true_type {};
}// impl_of

/// is_integral
// Observar la forma tan curiosa de cómo hacer la composición de dos funciones
template <typename T>
struct is_integral: 
     impl_of::is_integral<remove_cv_t<T>>::type
{};


/// is_integral_v
template <typename T>
inline constexpr bool is_integral_v = is_integral<T>::value;



// is_floating_point
// -----------------
namespace impl_of{
template <typename T>
struct is_floating_point: false_type {};

template <>
struct is_floating_point<float> : true_type {};

template <>
struct is_floating_point<double> : true_type {};

template <>
struct is_floating_point<long double> : true_type {};
} // impl_of

// is_floating_point
// -----------------
// Observar la forma tan curiosa de cómo hacer la composición de dos funciones
template <typename T>
struct is_floating_point: 
	impl_of::is_floating_point<remove_cv_t<T>>::type
{};


template <typename T>
inline constexpr bool is_floating_point_v = is_floating_point<T>::value;


// is_array
// --------
// Implementado en std_type_traits0.h

// is_pointer
// ----------
namespace impl_of{
template <typename T>
struct is_pointer : false_type { };

template <typename T>
struct is_pointer<T*> : true_type { };

}// namespace

template <typename T>
struct is_pointer : impl_of::is_pointer<remove_cv_t<T>>{ };

template <typename T>
inline constexpr bool is_pointer_v= is_pointer<T>::value;


// is_lvalue_reference
// -------------------
// Implementado en std_type_traits0.h

// is_rvalue_reference
// -------------------
// Implementado en std_type_traits0.h


// is_member_object_pointer
// ------------------------
// Implementado más abajo.

// is_member_function_pointer
// --------------------------
// Implementado más abajo.


// is_enum
// --------
// Depende de GCC. __is_enum es de GCC
template<typename T>
struct is_enum 
	: bool_constant<__is_enum(T)> { };

template <typename T>
inline constexpr bool is_enum_v = is_enum <T>::value;

// is_union
// --------
// Depende de GCC. __is_union es de GCC
template<typename T>
struct is_union
	: bool_constant<__is_union(T)> { };

template <typename T>
inline constexpr bool is_union_v = is_union<T>::value;


// is_class
// --------
// Depende de GCC. __is_class es de GCC
// cppreference tiene una implementación que no depende de gcc
template<typename T>
struct is_class
	: bool_constant<__is_class(T)>
{ };


template <typename T>
inline constexpr bool is_class_v = is_class<T>::value;

// is_function
// -----------
// Implementado en std_type_traits0.h

// is_member_object_pointer
// ------------------------
namespace impl_of{
template <typename T>
struct is_member_object_pointer : false_type { };

template <typename T, typename C>
struct is_member_object_pointer<T C::*> 
	: atd_::static_not<is_function_v<T>> { };

}// impl_of

template <typename T>
struct is_member_object_pointer 
    : impl_of::is_member_object_pointer<remove_cv_t<T>> { };

template <typename T>
inline constexpr 
    bool is_member_object_pointer_v = is_member_object_pointer<T>::value;


// is_member_function_pointer
// --------------------------
namespace impl_of{
template <typename T>
struct is_member_function_pointer : false_type { };

template <typename T, typename C>
struct is_member_function_pointer<T C::*> 
	    : is_function<T>::type { };
}// impl_of
 

template <typename T>
struct is_member_function_pointer 
	: impl_of::is_member_function_pointer<remove_cv_t<T>> 
{ };

template <typename T>
inline constexpr bool is_member_function_pointer_v 
				    = is_member_function_pointer<T>::value;

// -------------------------
// composite type categories
// -------------------------
// is_reference
// ------------
// Implementado en std_type_traits0.h


// is_arithmetic
// -------------
template <typename T>
struct is_arithmetic 
    : public integral_constant<bool,
				is_integral_v<T> || is_floating_point_v<T>>
{};
		

template <typename T>
inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;


// is_fundamental
// --------------
namespace impl_of{
template <typename T>
constexpr bool is_fundamental()
{
    if constexpr (is_arithmetic_v<T> or
		  is_void_v<T> or
		  is_null_pointer_v<T>)
	return true;

    return false;
}

}// impl_of

template <typename T>
struct is_fundamental 
	: bool_constant<impl_of::is_fundamental<T>()>
{};


template <typename T>
inline constexpr bool is_fundamental_v = is_fundamental<T>::value;

// is_object
// ---------
// Según cppreference: If T is an object type (that is any possibly cv-qualified
// type other than function, reference, or void types), provides the member
// constant value equal true. For any other type, value is false.
namespace impl_of{
template <typename T>
constexpr inline bool is_object()
{
    if (!(is_function_v<T> or is_reference_v<T> or is_void_v<T>))
	return true;

    else 
	return false;
}
} // impl_of

template <typename T>
struct is_object : public bool_constant<impl_of::is_object<T>()> { };


template <typename T>
inline constexpr bool is_object_v = is_object<T>::value;

// is_member_pointer
// -----------------
namespace impl_of{
template <typename T>
struct is_member_pointer : false_type { };

template <typename T, typename C>
struct is_member_pointer<T C::*> : true_type { };
}// impl_of
 
template <typename T>
struct is_member_pointer 
    : impl_of::is_member_pointer<remove_cv_t<T>>{ };

template <typename T>
inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

// is_scalar
// ---------
namespace impl_of{
template<typename T>
constexpr bool is_scalar()
{
    if constexpr (is_arithmetic_v<T> or
	          is_enum_v<T> or
		  is_pointer_v<T> or
		  is_member_pointer_v<T> or
		  is_null_pointer_v<T>)
	return true;

    else return false;
}

}// impl_of

template <typename T>
struct is_scalar : 
	bool_constant<impl_of::is_scalar<T>()> { };


template <typename T>
inline constexpr bool is_scalar_v = is_scalar<T>::value;


// is_compound
// -----------
template <typename T>
struct is_compound 
	: atd_::static_not<is_fundamental_v<T>>
{ };


template <typename T>
inline constexpr bool is_compound_v = is_compound<T>::value;

// is_member_pointer
// -----------------
// Implementado más arriba


// ---------------
// Type properties
// ---------------
// is_const
// --------
// Implementado en std_type_traits0.h

// is_volatile
// -----------
// Implementado en std_type_traits0.h

// extent
// ------
// Implementado en std_type_traits0.h

// type_identity
// -------------
// Implementado en std_type_traits0.h

// is_trivial
// ----------
// Depende de gcc: __is_trivial
template<typename T>
struct is_trivial : bool_constant<__is_trivial(T)>
{
  static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T>
inline constexpr bool is_trivial_v = is_trivial<T>::value;

// is_trivially_copyable
// ---------------------
// Depende de gcc: __is_trivially_copyable
template<typename T>
struct is_trivially_copyable 
	: bool_constant< __is_trivially_copyable(T)>
{
  static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T>
inline constexpr bool is_trivially_copyable_v
					= is_trivially_copyable<T>::value;


// is_standard_layout
// ------------------
// Depende de gcc: __is_standard_layout
template<typename T>
struct is_standard_layout 
	    : bool_constant<__is_standard_layout(T)>
{
  static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T>
inline constexpr bool is_standard_layout_v =  is_standard_layout<T>::value;


// is_empty
// --------
// Depende de gcc: __is_empty
template<typename T>
struct is_empty : bool_constant<__is_empty(T)>
{ };

template <typename T>
inline constexpr bool is_empty_v =  is_empty<T>::value;

// is_polymorphic
// --------------
// Depende de gcc: __is_polymorphic
template<typename T>
struct is_polymorphic : bool_constant<__is_polymorphic(T)>
{ };

template <typename T>
inline constexpr bool is_polymorphic_v =  is_polymorphic<T>::value;

// is_abstract
// -----------
// Depende de gcc: __is_abstract
template<typename T>
struct is_abstract : bool_constant<__is_abstract(T)>
{ };

template <typename T>
inline constexpr bool is_abstract_v =  is_abstract<T>::value;

// is_final
// --------
// Depende de gcc: __is_final
template<typename T>
struct is_final : bool_constant<__is_final(T)>
{ };

template <typename T>
inline constexpr bool is_final_v =  is_final<T>::value;


// is_aggregate
// ------------
// Depende de gcc: __is_aggregate
template<typename T>
struct is_aggregate 
	    : bool_constant<__is_aggregate(remove_cv_t<T>)>
{ };

template <typename T>
inline constexpr bool is_aggregate_v =  is_aggregate<T>::value;



// is_signed
// ---------
template <typename T, bool = is_arithmetic_v<T>>
struct _is_signed_helper : public false_type {};

template <typename T>
struct _is_signed_helper<T, true> 
	: public integral_constant<bool, T(-1) < T(0)>{};


/// is_signed
template <typename T>
struct is_signed : public _is_signed_helper<T>::type {};


/// is_signed_v
template <typename T>
inline constexpr bool is_signed_v = is_signed<T>::value;


// is_unsigned
// -----------
template <typename T>
struct is_unsigned 
    : public integral_constant<bool, is_arithmetic_v<T> and !is_signed_v<T>>
{};


/// is_unsigned_v
template <typename T>
inline constexpr bool is_unsigned_v = is_unsigned<T>::value;


// is_bounded_array
// ----------------
template<typename T>
struct is_bounded_array 
	    : private_::is_array_known_bounds<T> { };

template <typename T>
inline constexpr bool is_bounded_array_v =  is_bounded_array<T>::value;

// is_unbounded_array
// ------------------
template<typename T>
struct is_unbounded_array 
	: private_::is_array_unknown_bounds<T> { };

template <typename T>
inline constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value;

// is_constructible
// ----------------
namespace impl_of{
// Depende de gcc: __is_constructible
template<typename T, typename... Args>
struct is_constructible
	    : bool_constant<__is_constructible(T, Args...)>
{ };

}// impl_of

template<typename T, typename... Args>
struct is_constructible
	: impl_of::is_constructible<T, Args...>
{
  static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};


template <typename T, typename... Args>
inline constexpr bool is_constructible_v = is_constructible<T, Args...>::value;

// is_default_constructible
// ------------------------
//template <typename T>
//struct is_default_constructible 
//	: bool_constant<is_constructible_v<T>> {};

template<typename T, typename... Args>
struct is_default_constructible
	: impl_of::is_constructible<T, Args...>
{
  // precondición: DUDA: el estandard impone también que pueda ser cv void
  // (???) Lo incluye el __is_constructible()?
  static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T>
inline constexpr bool is_default_constructible_v 
				= is_default_constructible<T>::value;

// is_copy_constructible
// ---------------------
namespace impl_of{
template <typename T>
constexpr bool is_copy_constructible()
{
    if constexpr (atd_::is_referenceable<T>)
	return __is_constructible(T, const T&);

    else
	return false;
}

}// impl_of

template<typename T>
struct is_copy_constructible
	: bool_constant<impl_of::is_copy_constructible<T>()>
{
  static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T>
inline constexpr bool is_copy_constructible_v 
				=  is_copy_constructible<T>::value;

// is_move_constructible
// ---------------------
// Implementado en std_type_traits0.h

// is_assignable
// -------------
// Depende de gcc: __is_assignable
template<typename T, typename U>
struct is_assignable
    : bool_constant<__is_assignable(T, U)>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T, typename U>
inline constexpr bool is_assignable_v = is_assignable<T, U>::value;

// is_copy_assignable
// ------------------
namespace impl_of{
template <typename T>
constexpr bool is_copy_assignable()
{
    if constexpr (atd_::is_referenceable<T>)
	return __is_assignable(T&, const T&);

    else
	return false;
}

}// impl_of

template<typename T>
struct is_copy_assignable
	: bool_constant<impl_of::is_copy_assignable<T>()>
{
  static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T>
inline constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;

// is_move_assignable
// ------------------
// Implementado en std_type_traits0.h

// is_swappable_with
// -----------------
namespace impl_of{
template<typename T, typename U>
constexpr bool is_swappable_with()
{
    if constexpr (is_same_v<T, U> and is_lvalue_reference_v<T>){
	if constexpr 
	    (requires { STD::swap(STD::declval<T&>(), STD::declval<T&>()); })
		return true;

	else
	    return false;
    }

    else{
	if constexpr 
	    (requires { 
		STD::swap(STD::declval<T>(), STD::declval<U>()); 
		STD::swap(STD::declval<U>(), STD::declval<T>()); 
		})
		return true;

	else
	    return false;

    }

}


}// impl_of

template<typename T, typename U>
struct is_swappable_with
    : bool_constant<impl_of::is_swappable_with<T,U>()>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "First template argument must be a complete class or an unbounded array");

    static_assert(private_::is_complete_or_unbounded(type_identity<U>{}),
    "Second template argument must be a complete class or an unbounded array");
};

template <typename T, typename U>
inline constexpr bool is_swappable_with_v 
				=   is_swappable_with<T,U>::value;

// is_swappable
// ------------
// Implementado en std_type_traits0.h

// is_destructible
// ---------------
// Implementado más adelantes

// is_trivially_constructible
// --------------------------
template<typename T, typename... Args>
struct is_trivially_constructible
	: bool_constant<__is_trivially_constructible(T, Args...)>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T, typename... Args>
inline constexpr bool is_trivially_constructible_v 
				= is_trivially_constructible<T, Args...>::value;


// is_trivially_default_constructible
// ----------------------------------
template<typename T>
struct is_trivially_default_constructible
	: bool_constant<__is_trivially_constructible(T)>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T>
inline constexpr bool is_trivially_default_constructible_v
				= is_trivially_default_constructible<T>::value;


// is_trivially_copy_constructible
// -------------------------------
namespace impl_of{
template <typename T>
constexpr bool is_trivially_copy_constructible()
{
    if constexpr (atd_::is_referenceable<T>){
	if constexpr (__is_trivially_constructible(T, const T&) and
		      __is_constructible(T, const T&))
	    return true;

	else 
	    return false;
    }
    else
	return false;
}

}// impl_of

template<typename T>
struct is_trivially_copy_constructible
	: bool_constant<impl_of::is_trivially_copy_constructible<T>()>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T>
inline constexpr bool is_trivially_copy_constructible_v 
				    = is_trivially_copy_constructible<T>::value;


// is_trivially_move_constructible
// -------------------------------
namespace impl_of{
template <typename T>
constexpr bool is_trivially_move_constructible()
{
    if constexpr (atd_::is_referenceable<T>){
	if constexpr (__is_trivially_constructible(T, T&&) and
		      __is_constructible(T, T&&))
	    return true;

	else 
	    return false;
    }
    else
	return false;
}

}// impl_of

template<typename T>
struct is_trivially_move_constructible
	: bool_constant<impl_of::is_trivially_move_constructible<T>()>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T>
inline constexpr bool is_trivially_move_constructible_v 
				    = is_trivially_move_constructible<T>::value;



// is_trivially_assignable
// -----------------------
template<typename T, typename U>
struct is_trivially_assignable
	: bool_constant<__is_trivially_assignable(T, U)>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T, typename U>
inline constexpr bool is_trivially_assignable_v 
				    = is_trivially_assignable<T, U>::value;


// is_trivially_copy_assignable
// ----------------------------
namespace impl_of{
template <typename T>
constexpr bool is_trivially_copy_assignable()
{
    if constexpr (atd_::is_referenceable<T>)
	return __is_trivially_assignable(T&, const T&);

    else
	return false;
}

}// impl_of

template<typename T>
struct is_trivially_copy_assignable
	: bool_constant<impl_of::is_trivially_copy_assignable<T>()>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T>
inline constexpr bool is_trivially_copy_assignable_v 
				    = is_trivially_copy_assignable<T>::value;




// is_trivially_move_assignable
// ----------------------------
namespace impl_of{
template <typename T>
constexpr bool is_trivially_move_assignable()
{
    if constexpr (atd_::is_referenceable<T>)
	return __is_trivially_assignable(T&, T&&);

    else
	return false;
}

}// impl_of

template<typename T>
struct is_trivially_move_assignable
	: bool_constant<impl_of::is_trivially_move_assignable<T>()>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T>
inline constexpr bool is_trivially_move_assignable_v 
				    = is_trivially_move_assignable<T>::value;


// remove_all_extents
// ------------------
template <typename T>
struct remove_all_extents {
    using type = T;
};

template <typename T>
using remove_all_extents_t = typename remove_all_extents<T>::type;


template <typename T, size_t N>
struct remove_all_extents<T[N]>{
    using type = remove_all_extents_t<T>;
};

template <typename T>
struct remove_all_extents<T[]>{
    using type = remove_all_extents_t<T>;
};


// is_destructible
// ----------------
// bool is_destructible<type T>
// {
//	if (is_reference_v<T>)
//	    return true;
//
//	if (is_cv_void<T>)
//	    return false;
//
//	if (is_function_type<T>)
//	    return false;
//
//	if (is_unbound_array<T>)
//	    return false;
//
//	if (is_object_v<T>){
//	    type U = remove_all_extents_t<T>;
//
//	    if (requires {std::declval<U&>().~U})
//		return true;
//
//	    return false;
//	}
//
//	return false; //??? Esto no lo dicen
// }

namespace impl_of{

template <typename T>
constexpr bool is_destructible()
{
    if constexpr (is_void_v<T> or
		 private_::is_array_unknown_bounds_v<T> or
		  is_function_v<T>)
	return false;

    else if constexpr (is_reference_v<T> or
		       is_scalar_v<T>)
	return true;

    else {
	if constexpr (!is_object_v<T>)
	    return false;

	else {
	    using U = remove_all_extents_t<T>;
	    if constexpr ( atd_::has_destructor<U>)
		return true;

	    else
		return false;
	}
    }
}

}// impl_of
 
template<typename T>
struct is_destructible
	: bool_constant<impl_of::is_destructible<T>()>
{
  static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

// is_destructible_v
template <typename T>
inline constexpr bool is_destructible_v = is_destructible<T>::value;


// is_trivially_destructible
// -------------------------
template<typename T>
struct is_trivially_destructible
	: bool_constant<impl_of::is_destructible<T>() and
			__has_trivial_destructor(T)>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T>
inline constexpr bool is_trivially_destructible_v 
				    = is_trivially_destructible<T>::value;


// is_nothrow_constructible
// ------------------------
// Depende de gcc: __is_nothrow_constructible
template<typename T, typename... Args>
struct is_nothrow_constructible
	: bool_constant<__is_nothrow_constructible(T,Args...)>
{
  static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T, typename... Args>
inline constexpr bool is_nothrow_constructible_v 
				=   is_nothrow_constructible<T, Args...>::value;

// is_nothrow_default_constructible
// --------------------------------
// Depende de gcc: __is_nothrow_constructible
template<typename T>
struct is_nothrow_default_constructible
	: bool_constant<__is_nothrow_constructible(T)>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T>
inline constexpr bool is_nothrow_default_constructible_v
				=   is_nothrow_default_constructible<T>::value;


// is_nothrow_copy_constructible
// -----------------------------
namespace impl_of{

template <typename T>
constexpr bool is_nothrow_copy_constructible()
{
    if constexpr (atd_::is_referenceable<T>)
	return __is_nothrow_constructible(T, const T&);

    else
	return false;
}

}// impl_of
 
template<typename T>
struct is_nothrow_copy_constructible
    : bool_constant<impl_of::is_nothrow_copy_constructible<T>()>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};


template <typename T>
inline constexpr bool is_nothrow_copy_constructible_v
				=   is_nothrow_copy_constructible<T>::value;


// is_nothrow_move_constructible
// ------------------------------
// Implementado en std_type_traits0.h

// is_nothrow_assignable
// ---------------------
template <typename T, typename U>
struct is_nothrow_assignable 
	: bool_constant<__is_nothrow_assignable(T,U)> {};

template <typename T, typename U>
inline constexpr bool is_nothrow_assignable_v
				=   is_nothrow_assignable<T,U>::value;
	

// is_nothrow_copy_assignable
// --------------------------
namespace impl_of{

template <typename T>
constexpr bool is_nothrow_copy_assignable()
{
    if constexpr (atd_::is_referenceable<T>)
	return __is_nothrow_assignable(T&, const T&);

    else
	return false;
}

}// impl_of
 
template<typename T>
struct is_nothrow_copy_assignable
    : bool_constant<impl_of::is_nothrow_copy_assignable<T>()>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};


template <typename T>
inline constexpr bool is_nothrow_copy_assignable_v
				=   is_nothrow_copy_assignable<T>::value;




// is_nothrow_move_assignable
// --------------------------
// Implementado en std_type_traits0.h

// is_nothrow_swappable_with
// --------------------------
namespace impl_of{
template<typename T, typename U>
constexpr bool is_nothrow_swappable_with()
{
    if constexpr (is_same_v<T, U> and is_lvalue_reference_v<T>){
	if constexpr 
	    (requires{
		noexcept(STD::swap(STD::declval<T&>(), STD::declval<T&>()));}
		)
		return true;

	else
	    return false;
    }

    else{
	if constexpr 
	    (requires {
		noexcept(STD::swap(STD::declval<T>(), STD::declval<U>()));
		noexcept(STD::swap(STD::declval<U>(), STD::declval<T>())); }
		)
		return true;

	else
	    return false;

    }

}

}// impl_of
 
template<typename T, typename U>
struct is_nothrow_swappable_with
    : bool_constant<impl_of::is_nothrow_swappable_with<T,U>()>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "First template argument must be a complete class or an unbounded array");

    static_assert(private_::is_complete_or_unbounded(type_identity<U>{}),
    "Second template argument must be a complete class or an unbounded array");
};

template <typename T, typename U>
inline constexpr bool is_nothrow_swappable_with_v 
				=   is_nothrow_swappable_with<T,U>::value;


// is_nothrow_swappable
// --------------------
namespace impl_of{
template<typename T>
constexpr bool is_nothrow_swappable()
{
    if constexpr 
	    (requires{
		noexcept(STD::swap(STD::declval<T&>(), STD::declval<T&>()));}
		)
		return true;

	else
	    return false;
}

}// impl_of
template<typename T>
struct is_nothrow_swappable
    : bool_constant<impl_of::is_nothrow_swappable<T>()>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T>
inline constexpr bool is_nothrow_swappable_v 
				=   is_nothrow_swappable<T>::value;


// is_nothrow_destructible
// -----------------------
// Implementado más abajo.

// has_virtual_destructor
// ----------------------
template<typename T>
struct has_virtual_destructor
	: bool_constant< __has_virtual_destructor(T)>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T>
inline constexpr bool has_virtual_destructor_v 
				= has_virtual_destructor<T>::value;


// has_unique_object_representations
// ---------------------------------
template<typename T>
struct has_unique_object_representations
	: bool_constant< 
		    __has_unique_object_representations(
				remove_cv_t<remove_all_extents_t<T>>
			)>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T>
inline constexpr bool has_unique_object_representations_v 
				= has_unique_object_representations<T>::value;




// ---------------------
// Type property queries
// ---------------------
// alignment_of
// ------------
template<typename T>
struct alignment_of
	: integral_constant<STD::size_t, alignof(T)>
{
    static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

template <typename T>
inline constexpr size_t alignment_of_v =  alignment_of<T>::value;

// rank
// ----
template<typename>
struct rank
	: integral_constant<STD::size_t, 0> { };

template<typename T, STD::size_t _Size>
struct rank<T[_Size]>
	: integral_constant<STD::size_t, 1 + rank<T>::value> { };

template<typename T>
struct rank<T[]>
	: integral_constant<STD::size_t, 1 + rank<T>::value> { };

template <typename T>
inline constexpr size_t rank_v =  rank<T>::value;


// extent
// ------
// Implementado más arriba.

// ----------------------------
// Type relationship predicates
// ----------------------------
// is_same
// -------
// Implementado.

// is_base_of
// ----------
// Depende de gcc. __is_base_of!!!
// cppreference suministra una implementación que no depende de __is_base_of
template<typename Base, typename Derived>
struct is_base_of
: public integral_constant<bool, __is_base_of(Base, Derived)>
{ };

template <typename B, typename D>
inline constexpr bool is_base_of_v = is_base_of<B, D>::value;


// is_convertible
// --------------
namespace impl_of{

template <typename T>
void is_convertible_test(T);


template <typename From, typename To>
constexpr bool is_convertible(){
    if constexpr ( is_void_v<From>	or 
		   is_function_v<To>	or 
		   is_array_v<To>){
	if constexpr (is_void_v<To>)
	    return true;

	else 
	    return false;
    }

    else{
	if constexpr ( 
		requires { is_convertible_test<To>(STD::declval<From>()); }
		     )
	return true;
    else 
	return false;
    }
}

}// impl_of
 

template <typename From, typename To>
struct is_convertible 
	: bool_constant<impl_of::is_convertible<From, To>()> { };

template <typename From, typename To>
inline constexpr bool is_convertible_v = is_convertible<From, To>::value;



// is_nothrow_convertible
// ----------------------
namespace impl_of{

template <typename To>
void is_nothrow_convertible_test(To) noexcept;

// TODO: ¿por qué se complica tanto la implementación de
// is_nothrow_convertible()?
// La primera implementación que hice la hice como is_convertible: sencilla y
// clara, pero fallo el test_is_nothrow_convertible<char, D>. Cuando la clase
// tiene un constructor D(int) de tipo noexcept la parte 2 del test no
// funciona bien. Por eso he tenido que modificarla de esta forma.
template <typename From, typename To>
constexpr bool is_nothrow_convertible_part2()
{
    if constexpr ( 
	    requires { 
		noexcept(is_nothrow_convertible_test<To>
				(STD::declval<From>())); }
		 ){
	using test = 
	bool_constant<noexcept(
		is_nothrow_convertible_test<To> (STD::declval<From>()))>;
	if constexpr (test())
	    return true;
	else return false;

    } else
	return false;
}

template <typename From, typename To>
constexpr bool is_nothrow_convertible(){
    if constexpr ( is_void_v<From>	or 
		   is_function_v<To>	or 
		   is_array_v<To>){
	if constexpr (is_void_v<To>)
	    return true;

	else 
	    return false;
    }

    else
	 return is_nothrow_convertible_part2<From,To>();
}

}// impl_of
 

template <typename From, typename To>
struct is_nothrow_convertible 
	: bool_constant<impl_of::is_nothrow_convertible<From, To>()> { };

template <typename From, typename To>
inline constexpr bool is_nothrow_convertible_v 
				    = is_nothrow_convertible<From, To>::value;

// is_layout_compatible
// --------------------
// TODO

// invocable_result
// ----------------
// TODO: acabar
//namespace impl_of{
//
//template <typename Member_ptr, typename... Args>
//struct invoke_result_member_object;
//
//template <typename Res, typename Class, typename... Args>
//struct invoke_result_member_object<Res Class::*, Args>
//
//template <typename F, typename... Args>
//struct invoke_result { };
//
//
//template <typename F, typename Arg>
//    requires (is_member_object_pointer_v<remove_reference_t<F>>   == true and
//	      is_member_function_pointer_v<remove_reference_t<F>> == false)
//struct invoke_result<Member_ptr, Arg>
//	: invoke_result_member_object<decay_t<Member_ptr> { };
//
//
//
//}// impl_of
// 
//template <typename F, typename... Args>
//struct invoke_result
//	: impl_of::invoke_result<F, Args...>
//{
//    static_assert(private_::is_complete_or_unbounded(type_identity<F>{}),
//    "F must be a complete class or an unbounded array");
//
//    static_assert((private_::is_complete_or_unbounded(type_identity<Args>{}) 
//		  and  ...),
//    "Each argument type must be a complete class or an unbounded array");
//}
//
//
//template <typename F, typename... Args>
//inline constexpr bool invoke_result_t = invoke_result<F, Args...>::type;
//


// is_invocable
// ------------
//namespace impl_of{
//template <typename T, typename... Args>
//constexpr bool is_invocable
//{
//
//}
//
//
//}// impl_of
// 
//template <typename T, typename... Args>
//struct is_invocable
//	: bool_constant<impl_of::is_invocable<T, Args...>()> { };
//
//template <typename From, typename To>
//inline constexpr bool is_invocable_v  
//				    =  is_invocable<From, To>::value;

// is_nothrow_invocable
// --------------------
// TODO

// is_nothrow_invocable_r
// ----------------------
// TODO


// ----------------------------
// Const-volatile modifications
// ----------------------------
// Implementado.

// -------------------
// Array modifications
// -------------------
// if (T == typename(T)[] or T == typename(T)[sz]) return typename(T);
// else return T;
template <typename T>
struct remove_extent {
    using type = T;
};

template <typename T>
struct remove_extent<T[]> {
    using type = T;
};

template <typename T, size_t sz>
struct remove_extent<T[sz]> {
    using type = T;
};


template <typename T>
using remove_extent_t = typename remove_extent<T>::type;


// remove_all_extents
// ------------------
// Implementado más arriba.




// is_nothrow_destructible
// -----------------------
// La implementación de este trait es idéntico a is_destructible salvo que se
// mira que el destructor sea noexcept. ¿cómo fusionarlas las dos en una?
namespace impl_of{
template <typename T>
constexpr bool is_nothrow_destructible()
{
    if constexpr (is_void_v<T> or
		 private_::is_array_unknown_bounds_v<T> or
		  is_function_v<T>)
	return false;

    else if constexpr (is_reference_v<T> or
		       is_scalar_v<T>)
	return true;

    else {
	if constexpr (!is_object_v<T>)
	    return false;

	else {
	    using U = remove_all_extents_t<T>;
	    if constexpr ( requires { noexcept(STD::declval<U>().~U()) ;})

		return true;

	    else
		return false;
	}
    }
}

}// impl_of
 
template<typename T>
struct is_nothrow_destructible
	: bool_constant<impl_of::is_nothrow_destructible<T>()>
{
  static_assert(private_::is_complete_or_unbounded(type_identity<T>{}),
    "Template argument must be a complete class or an unbounded array");
};

// is_nothrow_destructible_v
template <typename T>
inline constexpr 
    bool is_nothrow_destructible_v = is_nothrow_destructible<T>::value;


// -----------------------
// Reference modifications
// -----------------------
// remove_reference
// ----------------
// Implementado en std_type_traits0.h

// add_lvalue_reference
// --------------------
// if (is_referenceable<T>()) return T&;
// else return T;
namespace impl_of{
template <typename T, bool = atd_::is_referenceable<T>>
struct add_lvalue_reference
{ using type = T;};

template <typename T>
struct add_lvalue_reference<T, true>
{ using type = T&;};

}// namespace 
 
template <typename T>
struct add_lvalue_reference
	: public impl_of::add_lvalue_reference<T>
{ };

template <typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;


// add_rvalue_reference
// --------------------
namespace impl_of{
template <typename T, bool = atd_::is_referenceable<T>>
struct add_rvalue_reference
{ using type = T;};

template <typename T>
struct add_rvalue_reference<T, true>
{ using type = T&&;};

}// namespace 
 
template <typename T>
struct add_rvalue_reference
	: public impl_of::add_rvalue_reference<T>
{ };

template <typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

// ------------------
// sign modifications
// ------------------
// make_signed
// -----------
// make_signed(T):
//	switch(T){
//	    case char: 
//	    case unsigned char: 
//		return signed char;
//	    ...
//	}
//  
// TODO: falta incluir char8_t, char16_t...
template <typename T>
struct make_signed { using type = T; };

template<>
struct make_signed<char> {using type = signed char;};

template<>
struct make_signed<unsigned char> {using type = signed char;};

template<>
struct make_signed<unsigned short> {using type = signed short;};

template<>
struct make_signed<unsigned int> {using type = signed int;};

template<>
struct make_signed<unsigned long> {using type = signed long;};

template<>
struct make_signed<unsigned long long> {using type = signed long long;};

template <typename T>
using make_signed_t = typename make_signed<T>::type;


// make_unsigned
// -------------
// TODO: falta incluir char8_t, char16_t...
template <typename T>
struct make_unsigned { using type = T; };

template<>
struct make_unsigned<char> {using type = unsigned char;};

template<>
struct make_unsigned<signed char> {using type = unsigned char;};

template<>
struct make_unsigned<signed short> {using type = unsigned short;};

template<>
struct make_unsigned<signed int> {using type = unsigned int;};

template<>
struct make_unsigned<signed long> {using type = unsigned long;};

template<>
struct make_unsigned<signed long long> {using type = unsigned long long;};


template <typename T>
using make_unsigned_t = typename make_unsigned<T>::type;


// ---------------------
// Pointer modifications
// ---------------------

// remove_pointer
// --------------
// if (T == typename(T)*) return typename(T);
// else if (T == const typename(T)*) return typename(T);
// else if (T == volatile typename(T)*) return typename(T);
// else if (T == const volatile typename(T)*) return typename(T);
// else return T;
//
// NOTA: gcc tiene una implementación diferente, "más smart", que reutiliza
// remove_cv. La mía es la obvia, aunque la de gcc tampoco es nada complicada
// de entender. ¿Cuál es mejor? ¿Cuál es más eficiente? ¿Otros criterios?
template <typename T>
struct remove_pointer{
    using type = T;
};

template <typename T>
struct remove_pointer<T*>{
    using type = T;
};


template <typename T>
struct remove_pointer<const T*>{
    using type = const T;
};


template <typename T>
struct remove_pointer<volatile T*>{
    using type = volatile T;
};


template <typename T>
struct remove_pointer<const volatile T*>{
    using type = const volatile T;
};


template <typename T>
using remove_pointer_t = typename remove_pointer<T>::type;


// add_pointer
// -----------
// typename add_pointer(typename T){
//
//  if (is_referenceable<T>())
//	return remove_reference_t<T>*;
//
//  else if (remove_cv_t<T> == void) 
//	return remove_reference_t<T>*;
//
//  else return T;
// }
//
// TODO: gcc implementa:
//
//  if (is_referenceable<T>() or is_void_v<T>)
//	return remove_reference_t<T>*;
//
//  else return T;
//
//  observar que omite el remove_cv_t<T> == void!!! Lo dejo como gcc para ser
//  consistente ya que estoy probando, pero habría que seguir el standar. Creo
//  que es un error de gcc.
//
template <typename T, 
	 bool = atd_::is_referenceable<T> or is_void_v<T>>
struct __add_pointer_switch;


template <typename T> 
struct __add_pointer_switch<T, false>
{
    using type = T;
};


template <typename T>
struct __add_pointer_switch<T, true>{
    using type = remove_reference_t<T>*;
};


template <typename T>
struct add_pointer
{ 
    using type = typename __add_pointer_switch<T>::type;
};


template <typename T>
using add_pointer_t = typename add_pointer<T>::type;


// ---------------------
// Other transformations
// ---------------------
// type_identity
// -------------
// Implementado.

// aligned_union
// -------------
// TODO

// remove_cvref
// ------------
template <typename T>
struct remove_cvref : remove_cv<T>
{};

template <typename T>
struct remove_cvref<T&> : remove_cv<T>
{};

template <typename T>
struct remove_cvref<T&&> : remove_cv<T>
{};

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;



// decay
// -----
// El standard indica que la función a implementar es:
//
// type decay<T>
// {
//	type U = remove_reference_t<T>;
//
//	if (is_array_v<U>)
//	    return remove_extent_t<U>*;
//
//	else if (is_function_v<U>)
//	    return add_pointer_t<U>;
//
//	else
//	    return remove_cv_t<U>;
//  }
namespace impl_of{
template <typename U,
	    bool = is_array_v<U>,
	    bool = is_function_v<U>>
struct decay;

template <typename U>
struct decay<U, false, false>
{
    using type = remove_cv_t<U>;
};


template <typename U> 
struct decay<U, true, false>{
    using type = remove_extent_t<U>*;
};


template <typename U> 
struct decay<U, false, true>{
    using type = add_pointer_t<U>;
};

// tiene prioridad is_array antes que is_function.
template <typename U> 
struct decay<U, true, true>{
    using type = remove_extent_t<U>*;
};
}// impl_of

template <typename T>
struct decay {
    using U    = remove_reference_t<T>;
    using type = typename impl_of::decay<U>::type;
};


template <typename T>
using decay_t = typename decay<T>::type;


// enable_if
// ---------
// type enable_if(bool b, type T)
// {
//	if (b == true) return T;
//	else no_type; // or compile_error("no type");
// }
template <bool b, typename T>
struct enable_if;

template <typename T>
struct enable_if<true, T>{ using type = T; };

template <bool b, typename T>
using enable_if_t = typename enable_if<b, T>::type;


// conditional
// -----------
template <bool B, typename True, typename False>
struct conditional
{ using type = True; };


template <typename True, typename False>
struct conditional<false, True, False>
{ using type = False; };


template <bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;




// common_type
// -----------
//  type common_type<type T1, type T2, ..., type Tn>
//  {
//	if (sizeof...(T) == 0) 
//	    return no_type; 
//
//	if (sizeof...(T) == 1) 
//	    return common_type_t<T, T>; // definido o no
//
//	if (sizeof...(T) == 2){
//	    type D1 = decay_t<T1>
//	    type D2 = decay_t<T2>;
//
//	    // equivalente a: if (!(is_same_v<T1,D1> and is_same_v<T2,D2>)
//	    if (!is_same_v<T1, D1> or !is_same_v<T2, D2>)
//		return common_type_t<D1, D2>;
//
//	    // case: is_same_v<T1, D1> and is_same_v<T2, D2>
//	    else if (!there_is_specialization_of(common_type_t<D1, D2>)){
//		// part2:
//		type C = decay_t<decltype(false ? declval<D1>(): declval<D2>())>;
//		if (C != no_type)
//		    return C;
//
//		type D = COND_RES(CREF<D1>, CREF<D2>);
//		if (D != no_type)
//		    return decay_t<D>;
//	    }
//	}
//
// 
//	return common_type_t<common_type_t<T1, T2>, Tail...>;
// }
//
template <typename... T>
struct common_type {};

template <typename... T> 
using common_type_t = typename common_type<T...>::type;


// sizeof...(T) == 0
template<>
struct common_type<> { };

// sizeof...(T) == 1
template <typename T>
struct common_type<T> : common_type<T,T> { };


// sizeof...(T) == 2
namespace impl_of{

// cond_res
// --------
// FUNDAMENTAL: el STD:: delante del declval para que funcionen los test!!!
template<typename X, typename Y>
using cond_res = 
	    decltype(false ? STD::declval<X(&)()>()() : STD::declval<Y(&)()>()());

namespace priv_{
// La siguiente expresión compila solo si T1 y T2 tienen un tipo en común, en
// cuyo caso es el que devuelve. Si no tienen tipo en común no compila.
// Precondition: T1 == D1, T2 == D2
// CUIDADO!!! si no llamo a STD::declval, y se llama directamente a declval el
// test falla en void, diciendo que no está definido declval<void>()!!!
// Por eso es fundamental llamar a STD::declval. ¿por qué ocurre esto? (por el
// contexto las templates se buscan en el namespace correspondiente. ¿tendrá
// que ver con eso?)
template<typename T1, typename T2>
using C_type = 
	    decay_t<decltype(false ? STD::declval<T1>() : STD::declval<T2>())>;

template <typename T>
using cref = add_lvalue_reference_t<const remove_reference_t<T>>;

// Precondition: T1 == D1, T2 == D2
template<typename T1, typename T2>
using D_type = cond_res<cref<T1>, cref<T2>>;

}

// part2: es un switch dependiendo de si existen tipos o no.
template <typename T1, typename T2, int case_number = 1, typename = void>
struct common_type_part2 
	: common_type_part2<T1, T2, case_number + 1> 
{ };

template <typename T1, typename T2>
struct common_type_part2 <T1, T2, 1, void_t<priv_::C_type<T1,T2>> >
{ 
    static constexpr int debug_number_part2 = 1;
    using type = priv_::C_type<T1, T2>; 
};

template <typename T1, typename T2>
struct common_type_part2 <T1, T2, 2, void_t<priv_::D_type<T1,T2>> >
{ 
    static constexpr int debug_number_part2 = 2;
    using type = decay_t<priv_::D_type<T1, T2>>; 
};

template <typename T1, typename T2>
struct common_type_part2 <T1, T2, 3>
{ 
    static constexpr int debug_number_part2 = 3;
};

// NOTA: se podía implementar usando un flag `bool are_both_equal` que indique
// si son iguales a su decay o no. Queda más sencillo de leer, pero involucra
// más llamadas a funciones con lo que el compilador tardará más en
// compilarlo. Por ello dejo esta implementación, que no muestra bien la
// intención del código.
// if (!(is_same_v<T1, D1> and is_same_v<T2,D2>))
//	return common_type_t<D1, D2>;
template <typename T1, typename T2,
	  typename D1 = decay_t<T1>, typename D2 = decay_t<T2> >
struct common_type : common_type<D1, D2>
{
    static constexpr int debug_decay_true = 0;
};

// else
template <typename T1, typename T2>
struct common_type<T1, T2, T1, T2>
    : common_type_part2<T1, T2> 
{ 
    static constexpr int debug_decay_true = 1;
};



// common_type_fold
// ----------------
// type common_type_fold<T1, ..., Tn>{
//	if (T1::type != no_type)
//	    return common_type_t<T1,...,Tn>;
//
//	else
//	    return no_type;
// }
template <typename T, typename... Tail>
struct common_type_fold { };

template <typename T, typename... Tail>
    requires (atd_::type_member<T>)
struct common_type_fold <T, Tail...>
	    : common_type<typename T::type, Tail...> {};


}// impl_of
 
template <typename T1, typename T2>
struct common_type<T1, T2> 
	    : impl_of::common_type<T1, T2> 
{ 
    static constexpr int debug_common_type2 = 102; // el valor da lo mismo
};


// sizeof...(T) > 2
// C(t1, t2, t3, ..., tn) = C( C(t1, t2), t3, ..., tn);
template <typename T1, typename T2, typename... Tail>
struct common_type <T1, T2, Tail...>
	: impl_of::common_type_fold<common_type<T1, T2>, Tail...>
{ };




// basic_common_reference
// ----------------------
// Según el standard: 
//	Unless this trait is specialized there shall be no member type.
// 
template<typename T, typename U,
	 template<typename> typename TQ, template<typename> typename UQ>
struct basic_common_reference { };


// common_reference
// ----------------
// El standard define las siguientes funciones previas:
//
// template <typename A>
// using CREF = add_lvalue_reference_t<const_ remove_reference_t<A>>;
//
// type XREF<type A, type U>	// = copy_cvref!!!
// {
//	if (is_lvalue_reference_v<A>)
//	    return copy_cv_t<A, U>&;
//
//	if (is_rvalue_reference_v<A>)
//	    return copy_cv_t<A, U>&&;
//
//	return copy_cv_t<A>;
// }
//
// type COPYCV<type From, type To>  // == copy_cv_t<from, to>
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
// template <typename X, typename Y>
// using COND_RES = decltype(false ? declval<X(&)()>()() : declval<Y(&)()>()());
//
// type COMMON_REF<type A, type B>
// {
//	type X = remove_reference_t<A>;
//	type Y = remove_reference_t<B>;
//	
//	if (is_lvalue_reference_v<A> and is_lvalue_reference_v<B>){
//	    type res = COND_RES(COPYCV(X,Y) &, COPYCV(Y,X) &);
//
//	    if (res != no_type and is_reference_v<res>)
//		return res;
//
//	    // return no_type; <-- no queda del todo claro el "otherwise"
//				  del standard
//	}
//
//	type C = remove_reference_t<COMMON_REF<X&, Y&>&&;
//	if (is_rvalue_reference_v<A> and is_rvalue_reference_v<B>
//	    and is_convertible_v<X&&, C>
//	    and is_convertible_v<Y&&, C>)
//	    return C;  
//
//	type D = COMMON_REF<const X&, Y&>;
//	if (is_rvalue_reference_v<A> and is_lvalue_reference_v<B>   
//	    and D != no_type)
//	    return D;
//
//	if (is_lvalue_reference_v<A> and is_rvalue_reference_v<B>)
//	    return COMMON_REF<B, A>;
//
//	return no_type;
// }
//
//
// type common_reference<T1, T2, T3, ..., Tn>
// {
//	if (n == 0)
//	    return no_type;
//
//	if (n == 1)
//	    return T1;
//
//	if (n == 2){
//	    type C = COMMON_REF<T1, T2>;
//	    if (is_reference_type<T1> and is_reference_type<T2>
//		and C != no_type)
//		return C;
//
//	    type D = basic_common_reference<remove_cvref_t<T1>, 
//				       remove_cvref_t<T2>,
//				       XREF<T1>, XREF<T2>;
//	    if (D != no_type)
//		return D;
//
//	    type E = COND_RES<T1, T2>;
//	    if (E != no_type)
//		return E;
//
//	    type F = common_type_t<T1, T2>;
//	    if (F != no_type)
//		return F;
//
//	    return no_type;
//	}
//
//	// if (n > 2)
//	type C = common_reference_t<T1, T2>;
//	if (is_type<C>) // == if(C != no_type)
//	    return common_reference_t<C, T3, ..., Tn>;
//
//	return no_type;
// }
//  

namespace private_{
// xref  (== copy_cvref)
// ----
// pero el nombre real es `copy_cvref` ya que lo que hace es copiar el const,
// volatile o ref de From, en To.
template <typename T>
struct xref{
    template <typename U>
    using type = atd_::copy_cv_t<T, U>;
};

template <typename T>
struct xref<T&>{
    template <typename U>
    using type = atd_::copy_cv_t<T, U>&;
};

template <typename T>
struct xref<T&&>{
    template <typename U>
    using type = atd_::copy_cv_t<T, U>&&;
};


} // namespace private_

namespace impl_of{
using atd_::copy_cv;
using atd_::copy_cv_t;


// COND_RES(COPYCV(X,Y)&, COPYCV(Y,X)&)
// ------------------------------------
template <typename X, typename Y>
using cond_res_cvref = cond_res<copy_cv_t<X,Y>&, copy_cv_t<Y,X>&>;

// common_ref (COMMON_REF)
// -----------------------
template <typename A, typename B>//, typename = void>
struct common_ref 
{ static constexpr int debug_number = 0; };

template <typename A, typename B>
using common_ref_t = typename common_ref<A,B>::type;

// case: is_lvalue_reference_v<A> and is_lvalue_reference_v<B>
template <typename A, typename B>
    requires (atd_::is_a_type<cond_res_cvref<A,B>> and
	      is_reference_v<cond_res_cvref<A, B>>)
struct common_ref <A&, B&>
{ 
    using type = cond_res_cvref<A, B>;
    static constexpr int debug_number = 1; 
};


template <typename A, typename B>
    requires (atd_::is_a_type<cond_res_cvref<A,B>> and
	      !is_reference_v<cond_res_cvref<A, B>>)
struct common_ref <A&, B&>
{ static constexpr int debug_number = 2; };

//  type C = remove_reference_t<COMMON_REF<A&, B&>&&;
template <typename A>
using X_t = remove_reference_t<A>;

template <typename B>
using Y_t = remove_reference_t<B>;

template <typename A, typename B>
using common_ref_C = 
	remove_reference_t<common_ref_t<X_t<A>&, 
				        Y_t<B>&>>&&;

// case: is_rvalue_reference_v<A> and is_rvalue_reference_v<B>
template <typename A, typename B>
    requires(atd_::is_a_type<common_ref_C<A, B>> and
	     is_convertible_v<X_t<A>&&, common_ref_C<A, B>> and
	     is_convertible_v<Y_t<B>&&, common_ref_C<A, B>>)
struct common_ref<A&&, B&&> //, void_t<common_ref_C<A,B>>>
{ 
    // DUDA: using type = common_ref_C<A&&, B&&>;
    using type = common_ref_C<A, B>;
    static constexpr int debug_number = 3; 
};


//  type D = COMMON_REF<const A&, B&>;
template <typename A, typename B>
using common_ref_D = common_ref_t<const X_t<A>&, B&>;

// if (is_rvalue_reference_v<A> and is_lvalue_reference_v<B> ...
template <typename A, typename B>
    requires (atd_::is_a_type<common_ref_D<A&&, B&>> and
	      is_convertible_v<A&&, common_ref_D<A, B>>)
struct common_ref<A&&, B&>//, void_t<common_ref_D<A,B>> >
{ 
    using type = common_ref_D<A, B>;
    static constexpr int debug_number = 4; 
};

// if (is_lvalue_reference_v<A> and is_rvalue_reference_v<B>)
template <typename A, typename B>
struct common_ref<A&, B&&>
    : common_ref<B&&, A&> 
{ 
    static constexpr int debug_number = 5; 
};


// common_reference (2 elementos)
// ------------------------------
// (impl) En este caso queremos implementar el siguiente flujo:
//	type X1 =...
//	if (X1 != no_type)
//	    return ...
//
//	type X2 = ...
//	if (X2 != no_type)
//	    return ...
//	...
//	return no_type;
//
//  Si un tipo no está definido, pasamos al siguiente condicional.
//  Los de gcc usan el siguiente truco: 
//	Como es habitual para saber si un tipo está definido o no se usa
//	void_t<T>. Cuando el tipo está definido, void_t<T> está definido y la
//	especialización de template es la que se instancia.
//	Pero ¿qué pasa si no está definido? Que entonces vamos a la definición
//	general que es herencia de common_reference con un case_number
//	superior, con lo que pasamos al siguiente case. Muy ingenioso.
//
template <typename T1, typename T2, int case_number = 1, typename = void>
struct common_reference
    : common_reference<T1, T2, case_number + 1>
{ };

template <typename T1, typename T2>
using common_reference_t = typename common_reference<T1, T2>::type;

// case C: is_reference_type... != no_type
template <typename T1, typename T2>
struct common_reference <T1&, T2&, 1,
			void_t<common_ref_t<T1&, T2&>>
			>
{ using type = common_ref_t<T1&, T2&>; };

template <typename T1, typename T2>
struct common_reference <T1&&, T2&&, 1,
			void_t<common_ref_t<T1&&, T2&&>>
			>
{ using type = common_ref_t<T1&&, T2&&>; };

template <typename T1, typename T2>
struct common_reference <T1&, T2&&, 1,
			void_t<common_ref_t<T1&, T2&&>>
			>
{ using type = common_ref_t<T1&, T2&&>; };

template <typename T1, typename T2>
struct common_reference <T1&&, T2&, 1,
			void_t<common_ref_t<T1&&, T2&>>
			>
{ using type = common_ref_t<T1&&, T2&>; };


// case D: basic_common_reference<...> != no_type
template <typename T1, typename T2>
using basic_common_ref =
    typename basic_common_reference<remove_cvref_t<T1>,
				    remove_cvref_t<T2>,
				    private_::xref<T1>::template type,
				    private_::xref<T2>::template type>::type;

template <typename T1, typename T2>
struct common_reference <T1, T2, 2,
			void_t<basic_common_ref<T1, T2>>
			>
{ using type = basic_common_ref<T1, T2>; };


// case E: COND_RES<T1, T2> != no_type
template <typename T1, typename T2>
struct common_reference <T1, T2, 3,
			void_t<cond_res<T1, T2>>
			>
{ using type = cond_res<T1, T2>; };

// case F: common_type_t<T1, T2> != no_type
template <typename T1, typename T2>
struct common_reference <T1, T2, 4,
			void_t<common_type_t<T1, T2>>
			>
{ using type = common_type_t<T1, T2>; };

// default
template <typename T1, typename T2>
struct common_reference <T1, T2, 5, void>
{ };

}// namespace impl_of
 
template <typename... T>
struct common_reference;

template <typename... T>
using common_reference_t = typename common_reference<T...>::type;

// if (n == 0) return no_type;
template<>
struct common_reference<>
{ };

// if (n == 1) return T1;
template <typename T1>
struct common_reference<T1>
{ using type = T1;};


// if (n == 2) ...
template <typename T1, typename T2>
struct common_reference<T1, T2>
    : impl_of::common_reference<T1, T2>
{ };


// if (n > 2) recursivo
template <typename T1, typename T2, typename... Tail>
struct common_reference<T1, T2, Tail...>{
    using type = common_reference_t<common_reference_t<T1, T2>, Tail...>;
};


// underlying_type
// ---------------
namespace impl_of{
template<typename T, bool = is_enum_v<T>>
struct underlying_type{
  using type = __underlying_type(T);
};

template<typename T>
struct underlying_type<T, false> { };
}// impl_of
 
template<typename T>
struct underlying_type : impl_of::underlying_type<T> { };

template<typename T>
using underlying_type_t = typename underlying_type<T>::type;

// invoke_result
// -------------
// Implementado más arriba (TODO)

// unwrap_reference
// ----------------
// TODO

// unwrap_ref_decay
// ----------------
// TODO



}// namespace

#endif


