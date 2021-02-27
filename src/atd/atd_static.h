// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __STATIC_H__
#define __STATIC_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Algoritmos en tiempo de compilación.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    18/01/2020 static_array
 *    26/02/2021 Reestructurado. 
 *		 static_variadic_element
 *		 static_array_push_back/front
 *		 static_find_subset_if
 *
 ****************************************************************************/

#include <cstddef>  // size_t
#include <type_traits>

namespace atd{
/*!
 *  \brief  Acceso a un elemento de T...args en tiempo de compilación.
 */
template<int i, typename T, T first, T... args>
struct _static_variadic_element{
    static constexpr T value = _static_variadic_element<i-1, T, args...>::value;
};

template<typename T, T first, T... args>
struct _static_variadic_element<0, T, first, args...>{
    static constexpr T value = first;
};

template <int i, typename T, T first, T... args>
static constexpr T static_variadic_element =
    _static_variadic_element<i, T, first, args...>::value;




/*!
 *  \brief  Static array.
 *
 */
template <typename T, T... args>
struct static_array{
    using size_type = std::size_t;

    /// Devuelve el elemento i del array.
    template <size_type i>
    static constexpr T at = static_variadic_element<i, T, args...>;

    static constexpr size_type size = sizeof...(args);
    static constexpr T data[size] = {args...};

    // Este operator no puede ser static como 'at'. Confio en que el
    // compilador optimice todo y no genere nada de código con esta función.
    // Si se quiere garantiar dicha optimización usar 'at'.
    constexpr T operator[](size_type i) const {return data[i];}
};


// caso degenerado: size == 0
template <typename T>
struct static_array<T>{
    using size_type = std::size_t;

    static constexpr size_type size = 0;
};



// static_array_push_back
// ----------------------
template <typename T, typename SA, T last>
struct _static_array_push_back;

template <typename T, T last, T... args>
struct _static_array_push_back<T, static_array<T, args...>, last>{
    using type = static_array<T, args..., last>;   
};

template <typename T, typename SA, T last>
using static_array_push_back = typename _static_array_push_back<T, SA, last>::type;


// static_array_push_front
// ----------------------
template <typename T, typename SA, T first>
struct _static_array_push_front;

template <typename T, T first, T... args>
struct _static_array_push_front<T, static_array<T, args...>, first>{
    using type = static_array<T, first, args...>;   
};

template <typename T, typename SA, T first>
using static_array_push_front =
    typename _static_array_push_front<T, SA, first>::type;

/*!
 *  \brief  static_find_subset_if
 *
 *  Extrae, en tiempo de compilación, el subset que cumple la condición F.
 *
// */
//static_array find_subset_if(static_array& x, Function f)
//{
//    static_array y{};
//    while (!empty()){
//	if (f(x[i])
//	    y.add(x[i]);
//    }
//    return y;
//}

template <typename Function, typename Static_array>
struct static_find_subset_if;


template <typename F, typename T, T first, T... args>
struct static_find_subset_if<F, static_array<T, first, args...>>{

    static constexpr F func{};

    using next_static_array = static_array<T, args...>;

    using T_true = static_array_push_front<
        T,
        typename static_find_subset_if<F, next_static_array>::type,
        first>;

    using T_false = static_find_subset_if<F, next_static_array>::type;

    using type = std::conditional_t<func(first), T_true, T_false>;

};


template <typename F, typename T>
struct static_find_subset_if<F, static_array<T>>{
    using type = static_array<T>;

};




}// namespace

#endif




