// Copyright (C) 2019-2020 Manuel Perez 
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
 *  - DESCRIPCION: Corresponde con el type_traits del estandar
 *
 *  - HISTORIA:
 *    A. Manuel Lopez 
 *    07/02/2019 v0.0
 *    22/10/2019 is_const_v, is_volatile_v, conditional_t
 *    03/11/2019 void_t
 *    08/12/2019 add_rvalue_reference/add_lvalue_reference/is_function
 *		 add_pointer/remove_pointer, ...
 *    18/03/2021 make_signed/make_unsigned
 *    14/08/2021 Completando implementación de common_type.
 *               Es copia de cppreference. TODO: reescribirla.
 *    23/12/2022 type_identity
 *
 ****************************************************************************/
#include "std_config.h"

#include "std_cstddef.h"    // size_t

namespace STD{
// Helper class: integral_constant
// ------------------------------------
template <typename T, T v>
struct integral_constant{
    using value_type = T;
    using type = integral_constant<T,v>;

    static constexpr T value = v;

    constexpr operator value_type() const noexcept {return  value;}
    constexpr value_type operator()() const noexcept {return value;}
};


/// bool_constant
template <bool B>
using bool_constant = integral_constant<bool, B>;

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;



/// is_same
template <typename X, typename Y>
struct is_same : public false_type {};

template <typename X>
struct is_same<X, X> : public true_type {};

/// is_same_v
template <typename X, typename Y>
inline constexpr bool is_same_v = is_same<X, Y>::value;

// declval: copiado de gcc. Es propuesta de Eric Niebler:
// https://bugs.llvm.org/show_bug.cgi?id=27798. Ver discusión en:
// https://stackoverflow.com/questions/56212117/understanding-declval-optimized-implementation
template<typename T, typename Up = T&&>
Up __declval(int); 

template<typename T>
T __declval(long);

template<typename T>
auto declval() noexcept -> decltype(__declval<T>(0));





// ----------------------------
// const-volatile modifications
// ----------------------------
template <typename T>
struct remove_const {
    using type = T;
};

template <typename T>
struct remove_const <const T> { using type = T; };

/// remove_const_v
template <typename T>
using remove_const_t = typename remove_const<T>::type;


/// remove_volatile
template <typename T>
struct remove_volatile {using type = T; };

template <typename T>
struct remove_volatile <volatile T> {using type = T;};

/// remove_volatile_t
template <typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

/// remove_cv
template <typename T>
struct remove_cv {
    using type = 
	typename remove_volatile<typename remove_const<T>::type>::type;
};


/// remove_cv_t
template <typename T>
using remove_cv_t = typename remove_cv<T>::type;


// -----------------------
// primary type categories
// -----------------------

// is_void
// -------
template <typename T>
constexpr inline bool __is_void()
{
    using U = remove_cv_t<T>;

    if (is_same_v<U, void>)
	return true;
    else
	return false;
}

template <typename T>
struct is_void : public bool_constant<__is_void<T>()> {};

template <typename T>
inline constexpr bool is_void_v = is_void<T>::value;


// is_integral
// -----------
// Esta función asocia a cada tipo (sin const ni volatile) true or false
// dependiendo de si es o no integral type
// Observar que se incluye char como tipo integral, pero... ¡es un caracter,
// no un número!!! (legacy code???)
template <typename>
struct _is_integral_without_cv: public false_type {};

template <>
struct _is_integral_without_cv<bool> : public true_type {};

template <>
struct _is_integral_without_cv<char> : public true_type {};

template <>
struct _is_integral_without_cv<signed char> : public true_type {};

template <>
struct _is_integral_without_cv<unsigned char> : public true_type {};

template <>
struct _is_integral_without_cv<char16_t> : public true_type {};

template <>
struct _is_integral_without_cv<char32_t> : public true_type {};

template <>
struct _is_integral_without_cv<short> : public true_type {};

template <>
struct _is_integral_without_cv<unsigned short> : public true_type {};

template <>
struct _is_integral_without_cv<int> : public true_type {};

template <>
struct _is_integral_without_cv<unsigned int> : public true_type {};

template <>
struct _is_integral_without_cv<long> : public true_type {};

template <>
struct _is_integral_without_cv<unsigned long> : public true_type {};

template <>
struct _is_integral_without_cv<long long> : public true_type {};

template <>
struct _is_integral_without_cv<unsigned long long> : public true_type {};

/// is_integral
// Observar la forma tan curiosa de cómo hacer la composición de dos funciones
template <typename T>
struct is_integral: 
    public _is_integral_without_cv<remove_cv_t<T>>::type
{};


/// is_integral_v
template <typename T>
inline constexpr bool is_integral_v = is_integral<T>::value;



// is_floating_point
// -----------------
template <typename T>
struct _is_floating_point_without_cv: public false_type {};

template <>
struct _is_floating_point_without_cv<float> : public true_type {};

template <>
struct _is_floating_point_without_cv<double> : public true_type {};

template <>
struct _is_floating_point_without_cv<long double> : public true_type {};


// is_floating_point
// -----------------
// Observar la forma tan curiosa de cómo hacer la composición de dos funciones
template <typename T>
struct is_floating_point: 
    public _is_floating_point_without_cv<remove_cv_t<T>>::type
{};


template <typename T>
inline constexpr bool is_floating_point_v = is_floating_point<T>::value;


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


// is_function: copiado de cpp_reference (es un monstruo!!!)
// ---------------------------------------------------------
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


// -------------------------
// composite type categories
// -------------------------
// is_reference
// ------------
template <typename T>
constexpr inline bool __is_reference()
{
    if (is_rvalue_reference_v<T> or is_lvalue_reference_v<T>)
	return true;

    return false;
}

template <typename T>
struct is_reference : public bool_constant<__is_reference<T>()>{ };

template <typename T>
inline constexpr bool is_reference_v = is_reference<T>::value;



// is_arithmetic
// -------------
template <typename T>
struct is_arithmetic 
    : public integral_constant<bool,
				is_integral_v<T> || is_floating_point_v<T>>
{};
		

// is_arithmetic_v
template <typename T>
inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;


// is_object
// ---------
// Según cppreference: If T is an object type (that is any possibly cv-qualified
// type other than function, reference, or void types), provides the member
// constant value equal true. For any other type, value is false.
template <typename T>
constexpr inline bool __is_object()
{
    if (!(is_function_v<T> or is_reference_v<T> or is_void_v<T>))
	return true;

    else 
	return false;
}

template <typename T>
struct is_object : public bool_constant<__is_object<T>()> { };


template <typename T>
inline constexpr bool is_object_v = is_object<T>::value;

// ---------------
// Type properties
// ---------------

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



// --------------
// is_signed
// --------------
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


/// is_unsigned
template <typename T>
struct is_unsigned 
    : public integral_constant<bool, is_arithmetic_v<T> and !is_signed_v<T>>
{};


/// is_unsigned_v
template <typename T>
inline constexpr bool is_unsigned_v = is_unsigned<T>::value;







// -----------------------
// Reference modifications
// -----------------------
// remove_reference
template <typename T>
struct remove_reference{ using type = T; };

template <typename T>
struct remove_reference<T&> { using type = T;};

template <typename T>
struct remove_reference<T&&> { using type = T;};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;


// add_lvalue_reference
// --------------------
// if (is_referenceable<T>()) return T&;
// else return T;
// ---
//
// TODO: gcc implementa también el caso en que se le pase una función. Aquí no
// lo estoy implementando.
template <typename T>
constexpr inline bool __is_referenceable()
{
    if (is_object_v<T> or is_reference_v<T>)
	return true;

    else
	return false;
}

template <typename T, bool = __is_referenceable<T>()>
struct __add_lvalue_reference_switch;

template <typename T>
struct __add_lvalue_reference_switch<T, false> {
    using type = T;
};

template <typename T>
struct __add_lvalue_reference_switch<T, true> {
    using type = T&;
};

template <typename T>
struct add_lvalue_reference{
    using type = typename __add_lvalue_reference_switch<T>::type;
};


template <typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;


//// add_rvalue_reference
//
//template <typename T>
//using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

// ------------------
// sign modifications
// ------------------
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


// --------------
// type relations
// --------------

template <typename From, typename To>
constexpr inline bool __is_convertible_req()
{
    return  is_void_v<From>	or 
	    is_function_v<To>	or 
	    is_array_v<To>;
}

template <typename From, typename To, bool = __is_convertible_req<From, To>()>
struct __is_convertible_helper{
    using type = typename is_void<To>::type;
};


template <typename From, typename To>
class __is_convertible_helper<From, To, false> {
    template <typename To1>
    static void test_aux(To1);

    template <typename From1,
              typename To1,
              typename = decltype(test_aux<To1>(declval<From1>()))>
    static true_type test(int);

    template <typename, typename>
    static false_type test(...);

public:
    using type = decltype(test<From, To>(0));
};

template <typename From, typename To>
struct is_convertible : __is_convertible_helper<From, To>::type { };


template <typename From, typename To>
inline constexpr bool is_convertible_v = is_convertible<From, To>::value;



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


// ---------------------
// Pointer modifications
// ---------------------

// remove_pointer
//
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
	 bool = __is_referenceable<T>() or is_void_v<T>>
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


// decay
// -----
// El standard indica que la función a implementar es:
//
// typename U = remove_reference_t<T>;
//
// if (is_array_v<U>)
//	return remove_extent_t<U>*;
//
// else if (is_function_v<U>)
//	return add_pointer_t<U>;
//
// else
//	return remove_cv_t<U>;
template <typename U,
	    bool = is_array_v<U>,
	    bool = is_function_v<U>>
struct __decay_switch;

template <typename U>
struct __decay_switch<U, false, false>
{
    using type = remove_cv_t<U>;
};


template <typename U> 
struct __decay_switch<U, true, false>{
    using type = remove_extent_t<U>*;
};


template <typename U> 
struct __decay_switch<U, false, true>{
    using type = add_pointer_t<U>;
};

// tiene prioridad is_array antes que is_function.
template <typename U> 
struct __decay_switch<U, true, true>{
    using type = remove_extent_t<U>*;
};


template <typename T>
struct decay {
    using U    = remove_reference_t<T>;
    using type = typename __decay_switch<U>::type;
};


template <typename T>
using decay_t = typename decay<T>::type;


// enable_if
// ---------
// typename enable_if(bool b, typename T)
// {
//	if (b == true) return T;
//	else compile_error("no type");
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

template <typename...>
using void_t = void;



// common_type
// -----------
// if (sizeof...(T) == 0) 
//	compile_error("undefined");
//
// else if (sizeof...(T) == 1) 
//	return common_type_t<T, T>; // definido o no
//
// else if (sizeof...(T) == 2){
//	typename D1 = decay_t<T1>;
//	typename D2 = decay_t<T2>;
//
//	// condicion == !(is_same_v<T1, D1> and is_same_v<T2, D2>)
//	if (!is_same_v<T1, D1> or !is_same_v<T2, D2>)
//	    return common_type_t<D1, D2>;
//
//	else if (!there_is_specialization_of(common_type_t<D1, D2>))
//	    return decay_t<decltype(false ? declval<D1>(): declval<D2>())>;
// }
//
// else 
//	return common_type_t<common_type_t<T1, T2>, Tail...>;
//	
// TODO: la impresión al hacer las pruebas es que al introducir common_type en
// los test tarda bastante más en compilar. ¿Merecerá la pena mejorar la
// implementación? Esta es muy sencilla de entender.
// gcc tiene una implementación diferente.
template <typename... T>
struct common_type {};

template <typename... T> 
using common_type_t = typename common_type<T...>::type;



// sizeof...(T) == 0
template<>
struct common_type<> { };

// sizeof...(T) == 1
template <typename T>
struct common_type<T> {
    using type = common_type_t<T, T>;
};

// sizeof...(T) == 2
// TODO: esta implementación es copia de la que hay en cppreference.
// Preferiría hacerla con un switch pero el problema es cómo hacerlo cuando no
// `type` no queda definido. Por eso se usa la herencia.
namespace detail {

// La expresión "false? decl..." no compila si T1 y T2 no son del mismo tipo
// (?)
template<typename T1, typename T2>
using common_declval_T1_T2 = decltype(false ? declval<T1>() : declval<T2>());
 
template<typename, typename, typename = void>
struct decay_conditional_result {};

template<typename T1, typename T2>
struct decay_conditional_result<T1, T2, void_t<common_declval_T1_T2 <T1, T2>>>
{
    using type = decay_t<common_declval_T1_T2<T1, T2>>;
};
 
template<typename T1, typename T2, typename = void>
struct common_type_2_impl : decay_conditional_result<const T1&, const T2&> {};
 
 
template<typename T1, typename T2>
struct common_type_2_impl<T1, T2, void_t<common_declval_T1_T2<T1, T2>>>
    : decay_conditional_result<T1, T2> {};
}
 
template <typename T1, typename T2>
struct common_type<T1, T2> 
    : conditional<is_same<T1, typename decay<T1>::type>::value &&
                       is_same<T2, typename decay<T2>::type>::value,
                       detail::common_type_2_impl<T1, T2>,
                       common_type<typename decay<T2>::type,
                                   typename decay<T2>::type>>::type {};




// sizeof...(T) > 2
// C(t1, t2, t3, ..., tn) = C( C(t1, t2), t3, ..., tn);
template <typename T1, typename T2, typename... Tail>
struct common_type <T1, T2, Tail...>{
    using type = common_type_t<common_type_t<T1, T2>, Tail...>;
};


// type_identity
// -------------
template <typename T>
struct type_identity {
    using type = T;
};

template <typename T>
using type_identity_t = typename type_identity<T>::type;


}// namespace

#endif


