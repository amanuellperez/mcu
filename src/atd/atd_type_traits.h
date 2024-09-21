// Copyright (C) 2019-2024 Manuel Perez 
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

#ifndef __ATD_TYPE_TRAITS_BASIC_H__
#define __ATD_TYPE_TRAITS_BASIC_H__
/****************************************************************************
 *
 *  DESCRIPCION
 *	Ampliación de type_traits.h
 *
 *
 *  HISTORIA
 *    Manuel Perez
 *    27/08/2019 Less_than, Pertenece_al_intervalo_cerrado
 *    12/02/2021 has_same_sign, same_type_with_double_bits
 *    14/03/2021 same_type_at_least32
 *    18/03/2021 make_type<A>::template same_sign_as<B>
 *    13/08/2021 common_type_if_convertible_t
 *    23/12/2022 value_type_t<T>/size_type<T>/iterator_type<T>
 *    25/02/2023 sizeof_in_bits<T>/size_of_in_bytes<T>
 *    12/10/2023 is_integer, is_decimal
 *    15/06/2024 least_uint_t_to_represent
 *    21/07/2024 identity
 *
 ****************************************************************************/
#include <type_traits>
#include <stdint.h> // uint8_t ...
#include <cstddef>
#include <limits>
#include <functional>	// identity
			
namespace atd{


/// ¿x es menor que y?
template <typename Int, Int x, Int y>
struct Less_than: public std::bool_constant<x < y>{};


// Tipo que permite hacer:
// static_assert(always_false<T>::value, "aqui no debe de llegar");
template <typename T>
struct always_false_type : std::false_type { };

template <typename T>
inline constexpr bool always_false_v = always_false_type<T>::value;




// has_same_sign
// -------------
// bool has_same_sign(Integer T, Integer U)
// {
//    if (sign(T) == sign(U))
//       return true;
//    else
//       return false;
// }
template <typename T, typename U>
inline constexpr bool has_same_sign()
{
    if constexpr (std::is_signed_v<T> and std::is_signed_v<U>)
	return true;
    else if constexpr (!std::is_signed_v<T> and !std::is_signed_v<U>)
	return true;
    else
	return false;
}

// Todas las funciones con tipo son _t ó _v. 
template <typename T, typename U>
inline static constexpr bool has_same_sign_v = has_same_sign<T,U>();

/*!
 *
 * make_type<A>::same_sign_as<B>:
 *  if (is_signed<B>) return make_signed<A>;
 *  else return make_unsigned<A>;
 *
 */
template <typename A, typename B, bool = std::is_signed_v<B>>
struct __same_sign_as{
    using type = std::make_signed_t<A>;
};

template <typename A, typename B>
struct __same_sign_as<A, B, false>{
    using type = std::make_unsigned_t<A>;
};

// TODO: para usarlo llamar:
//		using R = make_type<A>::template same_sign_as<B>;
//	¿cómo puedo eliminar el template? Queda la mar de feo.
template <typename A>
struct make_type{
    template <typename B>
    struct same_sign_as_
    { using type = typename __same_sign_as<A, B>::type; };

    template <typename B>
    using same_sign_as = typename same_sign_as_<B>::type;
};



/*!
 *  \brief  Devuelve el mismo type pero con el doble de bits.
 *
 *  Si no existe, devuelve el mismo tipo.
 *  
 *  Integer same_type_with_double_bits(Integer T)
 *  {
 *	switch(T){
 *	    case uint8_t : return uint16_t;
 *	    case uint16_t: return uint32_t;
 *	    case uint32_t: return uint64_t;
 *	    // idem para signed
 *	} 
 *
 *	return T;
 *  }
 */
template <typename T>
struct same_type_with_double_bits{
    using type = T;
};

template<>
struct same_type_with_double_bits<uint8_t>{
    using type = uint16_t;
};

template<>
struct same_type_with_double_bits<uint16_t>{
    using type = uint32_t;
};

template<>
struct same_type_with_double_bits<uint32_t>{
    using type = uint64_t;
};


template<>
struct same_type_with_double_bits<uint64_t>{
    using type = uint64_t;
};

template<>
struct same_type_with_double_bits<int8_t>{
    using type = int16_t;
};

template<>
struct same_type_with_double_bits<int16_t>{
    using type = int32_t;
};

template<>
struct same_type_with_double_bits<int32_t>{
    using type = int64_t;
};


template<>
struct same_type_with_double_bits<int64_t>{
    using type = int64_t;
};


template <typename T>
using same_type_with_double_bits_t =
    typename same_type_with_double_bits<T>::type;


/*!
 *  \brief  Devuelve el mismo type pero mínimo de 32 bits
 *
 *  Integer same_type_at_least32(Integer T)
 *  {
 *	switch(T){
 *	    case uint8_t :
 *	    case uint16_t:
 *		return uint32_t;
 *
 *	    case int8_t:
 *	    case int16_t:
 *		return int32_t;
 *
 *	    default:
 *		return T;
 *	} 
 *  }
 */
template <typename T>
struct same_type_at_least32{
    using type = T;
};

template<>
struct same_type_at_least32<uint8_t>{
    using type = uint32_t;
};

template<>
struct same_type_at_least32<uint16_t>{
    using type = uint32_t;
};

template<>
struct same_type_at_least32<int8_t>{
    using type = int32_t;
};

template<>
struct same_type_at_least32<int16_t>{
    using type = int32_t;
};

template <typename T>
using same_type_at_least32_t =
    typename same_type_at_least32<T>::type;



// SFINAE helper to obtain common_type<Rep1, Rep2> only if Rep2
// is implicitly convertible to it.
// if (is_convertible_v<const Rep2&, common_type_t<Rep1, Rep2>>)
//	return common_type_t<Rep1, Rep2>;
// else
//	compile_error();
template <typename Rep1,
          typename Rep2,
          typename CRep = std::common_type_t<Rep1, Rep2>>
using common_type_if_convertible_t =
    std::enable_if_t<std::is_convertible_v<const Rep2&, CRep>, CRep>;


// value_type_t
// ------------
// type value_type<typename T>
// {
//	if (is_array(T == T[N]))
//	    return T;
//
//	else 
//	    return T::value_type;
// }
template <typename T>
struct value_type{
    using type = typename T::value_type;
};

template <typename T, size_t N>
struct value_type<T[N]>{
    using type = T;
};

template <typename T>
using value_type_t = typename value_type<T>::type;


// size_type_t
// ------------
// type size_type<typename T>
// {
//	if (is_array(T == T[N]))
//	    return size_t;
//
//	else 
//	    return T::size_type;
// }
template <typename T>
struct size_type{
    using type = typename T::size_type;
};

template <typename T, size_t N>
struct size_type<T[N]>{
    using type = T;
};

template <typename T>
using size_type_t = typename size_type<T>::type;


// iterator_type_t
// ---------------
// type iterator_type<typename T>
// {
//	if (is_array(T == T[N]))
//	    return T*;
//
//	else 
//	    return T::iterator_type;
// }
template <typename T>
struct iterator_type{
    using type = typename T::iterator;
};

template <typename T, size_t N>
struct iterator_type<T[N]>{
    using type = T*;
};

template <typename T>
using iterator_type_t = typename iterator_type<T>::type;


// sizeof
// ------
// Uno de los problemas con sizeof es que devuelve el tamaño en chars, que no
// se garantiza que sean bytes.
// sizeof devuelve size_t ==> sizeof_in_bits devuelve size_t
template <typename T>
inline constexpr size_t sizeof_in_bytes()
{
    // garantizamos que sizeof devuelva el número de bytes = 8 bits
    static_assert(sizeof(uint8_t) == sizeof(char));

    return sizeof(T);
}

// sizeof devuelve size_t ==> sizeof_in_bits devuelve size_t
template <typename T>
inline constexpr size_t sizeof_in_bits()
{
    // garantizamos que sizeof devuelva el número de bytes = 8 bits
    static_assert(sizeof(uint8_t) == sizeof(char));

    return (sizeof(T) * 8) / sizeof(char);
}


// is_integer
// ------------------
// Uno de los problemas con el standar de C++ es que considera `char` que es
// un CARACTER como un integral type. Pero un carácter NO es un entero. Por
// ello defino is_integer que excluye los char de los enteros
// (mentira, ya que uint8_t que es un entero el compilador realmente lo
// concibe como tipo char, lo cual es un error heredado, creo, de C)
namespace impl_of{
template <typename>
struct is_integer:  std::false_type {};

// `bool` no es un entero!!! bool es true/false. NO ES UN NÚMERO!!!
//template <>
//struct is_integer<bool> :  true_type {};

// uint8_t/int8_t internamente son char!!! :(
template <>
struct is_integer<uint8_t> :  std::true_type {};

template <>
struct is_integer<int8_t> :  std::true_type {};

template <>
struct is_integer<short> :  std::true_type {};

template <>
struct is_integer<unsigned short> :  std::true_type {};

template <>
struct is_integer<int> :  std::true_type {};

template <>
struct is_integer<unsigned int> :  std::true_type {};

template <>
struct is_integer<long> :  std::true_type {};

template <>
struct is_integer<unsigned long> :  std::true_type {};

template <>
struct is_integer<long long> :  std::true_type {};

template <>
struct is_integer<unsigned long long> :  std::true_type {};
}// impl_of

/// is_integer
template <typename T>
struct is_integer: 
     impl_of::is_integer<std::remove_cv_t<T>>::type
{};


/// is_integer_v
template <typename T>
inline constexpr bool is_integer_v = is_integer<T>::value;


// is_decimal
// ----------
// (RRR) Si esta clase es sinónima de `std:is_floating_point`¿para qué
//       definirla?
//
//       cppreference indica que std::is_floating_point NO se puede
//       especializar, pero yo quiero especializarla para atd::Decimal, ya que
//       es de tipo floating_point. Como no puedo (=debo) violar el estandar
//       me veo obligado a definir mi propio atd::is_floating_point. Opto por
//       no llamarlo `atd::is_floating_point` por el lío que generaría el
//       nombre con el del estandard.
//
//       Y esta clase se puede especializar todo lo que quieras!!!
template <typename T>
struct is_decimal: std::is_floating_point<T> {};

template <typename T>
inline constexpr bool is_decimal_v = is_decimal<T>::value;


// least_uint_t_to_represent
// -------------------------
// Devuelve el menor tipo uintxx_t capaz de albergar el número N
// TODO: nombre??? cómo llamo a esto??? @_@

namespace impl_of{

template <uint64_t N>
constexpr 
int least_uint_t_to_represent_()
{
    if constexpr (N <= std::numeric_limits<uint8_t>::max())
	return 8;

    else if (N <= std::numeric_limits<uint16_t>::max())
	return 16;

    else if (N <= std::numeric_limits<uint32_t>::max())
	return 32;

    else 
	return 64;
}


template <uint64_t N, int type = impl_of::least_uint_t_to_represent_<N>()>  
struct least_uint_t_to_represent;

template <uint64_t N> 
struct least_uint_t_to_represent<N, 8>
{ using type = uint8_t; };

template <uint64_t N> 
struct least_uint_t_to_represent<N, 16>
{ using type = uint16_t; };

template <uint64_t N> 
struct least_uint_t_to_represent<N, 32>
{ using type = uint32_t; };

template <uint64_t N> 
struct least_uint_t_to_represent<N, 64>
{ using type = uint64_t; };

}// impl_of


template <uint64_t N>
using least_uint_t_to_represent_t
	= typename impl_of::least_uint_t_to_represent<N>::type;


// identity
// --------
// La defino para simplificar su uso
template <typename T>
constexpr T&& identity(T&& t) noexcept 
{ return std::identity{}(t); }

}// namespace

#endif

