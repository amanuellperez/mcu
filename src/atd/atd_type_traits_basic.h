// Copyright (C) 2019-2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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
 *   - DESCRIPCION: Ampliación de type_traits.h
 *
 *
 *   - HISTORIA:
 *    A.Manuel L.Perez
 *    27/08/2019 Less_than, Pertenece_al_intervalo_cerrado
 *    12/02/2021 has_same_sign, same_type_with_double_bits
 *    14/03/2021 same_type_at_least32
 *    18/03/2021 make_type<A>::template same_sign_as<B>
 *
 ****************************************************************************/
#include <type_traits>
#include <stdint.h> // uint8_t ...

namespace atd{


/// ¿x es menor que y?
template <typename Int, Int x, Int y>
struct Less_than: public std::bool_constant<x < y>{};


// ¿x pertenece al intervalo cerrado [a,b]?
// TODO: cambiar nombre. Lo usa UART_baud_rate
template <typename Int, Int x, Int a, Int b>
struct Pertenece_al_intervalo_cerrado
    : public std::bool_constant< 
				a <= x and x <= b
				> {};



// Tipo que permite hacer:
// static_assert(always_false<T>::value, "aqui no debe de llegar");
template <typename T>
struct always_false_type : std::false_type { };

template <typename T>
inline constexpr bool always_false_v = always_false_type<T>::value;




/*!
 *  bool has_same_sign(Integer T, Integer U)
 *  {
 *     if (sign(T) == sign(U))
 *          return true;
 *     else
 *          return false;
 *  }
 *
 */
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
struct same_type_with_double_bits_{
    using type = T;
};

template<>
struct same_type_with_double_bits_<uint8_t>{
    using type = uint16_t;
};

template<>
struct same_type_with_double_bits_<uint16_t>{
    using type = uint32_t;
};

template<>
struct same_type_with_double_bits_<uint32_t>{
    using type = uint64_t;
};


template<>
struct same_type_with_double_bits_<uint64_t>{
    using type = uint64_t;
};

template<>
struct same_type_with_double_bits_<int8_t>{
    using type = int16_t;
};

template<>
struct same_type_with_double_bits_<int16_t>{
    using type = int32_t;
};

template<>
struct same_type_with_double_bits_<int32_t>{
    using type = int64_t;
};


template<>
struct same_type_with_double_bits_<int64_t>{
    using type = int64_t;
};


template <typename T>
using same_type_with_double_bits =
    typename same_type_with_double_bits_<T>::type;


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
struct same_type_at_least32_{
    using type = T;
};

template<>
struct same_type_at_least32_<uint8_t>{
    using type = uint32_t;
};

template<>
struct same_type_at_least32_<uint16_t>{
    using type = uint32_t;
};

template<>
struct same_type_at_least32_<int8_t>{
    using type = int32_t;
};

template<>
struct same_type_at_least32_<int16_t>{
    using type = int32_t;
};

template <typename T>
using same_type_at_least32 =
    typename same_type_at_least32_<T>::type;


}// namespace

#endif

