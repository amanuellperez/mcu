// Copyright (C) 2019-2022 Manuel Perez 
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

#ifndef __MCU_STD_ALGORITHM_H__
#define __MCU_STD_ALGORITHM_H__
/****************************************************************************
 *
 *  - DESCRIPCION: El equivalente a <algorithm>
 *
 *  - HISTORIA:
 *    Manuel Perez 
 *	08/02/2019: for_each, for_each_n
 *	16/07/2019: min/max
 *	19/09/2019: copy, copy_n, fill, fill_n
 *	03/11/2019: find, find_if, find_if_not
 *	16/11/2019: count, count_if
 *	04/02/2020: shift_left
 *	28/07/2021: swap, reverse
 *	31/01/2022: max_element/min_element
 *
 *   - TODO:
 *	- fill_n: especializarla a memset cuando se trate de arrays.
 *
 ****************************************************************************/
#include "std_config.h"

#include "std_iterator.h"
#include "std_utility.h"    // std::move

namespace STD{
// ---------------------------------
// non-modifying sequence operations
// ---------------------------------


// for_each
// --------
template <typename Input_it, typename Unary_function>
constexpr Unary_function
for_each(Input_it begin, Input_it end, Unary_function f)
{
    for(; begin != end; ++begin)
	f(*begin);

    return f;
}


template <typename Input_it, typename Size, typename Unary_function>
constexpr Unary_function for_each_n(Input_it begin, Size n, Unary_function f)
{
    for(; n != Size{0}; ++begin, --n)
	f(*begin);

    return f;
}


// find
// ----
template <typename Input_it, typename T>
inline constexpr Input_it find(Input_it p0, Input_it pe, const T& value)
{
    while (p0 != pe and *p0 != value)
	++p0;

    return p0;
}

template <typename Input_it, typename Unary_predicate>
inline constexpr Input_it find_if(Input_it p0, Input_it pe, Unary_predicate f)
{
    while (p0 != pe and !f(*p0))
	++p0;

    return p0;
}

template <typename Input_it, typename Unary_predicate>
inline constexpr Input_it find_if_not(Input_it p0, Input_it pe, Unary_predicate f)
{
    while (p0 != pe and f(*p0))
	++p0;

    return p0;
}


// count
// -----
template <typename Input_it, typename T>
constexpr typename iterator_traits<Input_it>::difference_type
    count (Input_it p0, Input_it pe, const T& value)
{
    typename iterator_traits<Input_it>::difference_type n{0};

    for (; p0 != pe; ++p0){
	if (*p0 == value)
	    ++n;
    }

    return n;
}


template <typename Input_it, typename Unary_predicate>
constexpr typename iterator_traits<Input_it>::difference_type
    count_if (Input_it p0, Input_it pe, Unary_predicate pred)
{
    typename iterator_traits<Input_it>::difference_type n{0};

    for (; p0 != pe; ++p0){
	if (pred(*p0))
	    ++n;
    }

    return n;
}



// -----------------------------
// modifying sequence operations
// -----------------------------

// fill
// ----
template <typename Output_it, typename T>
inline constexpr Output_it fill(Output_it q, Output_it qe, const T& value)
{
    for (; q != qe; ++q)
	*q = value;

    return q;
}



template <typename Output_it, typename Size, typename T>
inline constexpr Output_it fill_n(Output_it q, Size n, const T& value)
{
    for (; n > 0; ++q, --n)
	*q = value;

    return q;
}



// minimum and maximum
// -------------------
template <typename T>
inline constexpr const T& min(const T& a, const T& b)
{ return (a < b)? a: b; }


template <typename T>
inline constexpr const T& max(const T& a, const T& b)
{ return (a < b)? b: a; }


template <typename T, typename Compare>
inline constexpr const T& min(const T& a, const T& b, Compare cmp)
{ return cmp(a, b)? a: b; }


template <typename T, typename Compare>
inline constexpr const T& max(const T& a, const T& b, Compare cmp)
{ return cmp(a, b)? b: a; }




// max_element
// -----------
// ¿Se pueden fusionar las 4 implementaciones en una sola? El problema esta en
// que a max_element se le pasa std::less y no std::greater.
template<typename Forward_it>
constexpr Forward_it min_element(Forward_it p, Forward_it pe)
{
    if (p == pe) return pe;

    Forward_it res =  p;
    ++p;

    for (; p != pe; ++p){
	if (*p < *res)
	    res = p;
    }

    return res;
}


template<typename Forward_it, typename Cmp>
constexpr Forward_it min_element(Forward_it p, Forward_it pe, Cmp cmp)
{
    if (p == pe) return pe;

    Forward_it res =  p;
    ++p;

    for (; p != pe; ++p){
	if (cmp(*p, *res))
	    res = p;
    }

    return res;
}



template<typename Forward_it>
constexpr Forward_it max_element(Forward_it p, Forward_it pe)
{
    if (p == pe) return pe;

    Forward_it res =  p;
    ++p;

    for (; p != pe; ++p){
	if (*p > *res)
	    res = p;
    }

    return res;
}


// Cmp = returns true if the first argument is less (!!!) than the second
template<typename Forward_it, typename Cmp>
constexpr Forward_it max_element(Forward_it p, Forward_it pe, Cmp cmp)
{
    if (p == pe) return pe;

    Forward_it res =  p;
    ++p;

    for (; p != pe; ++p){
	if (cmp(*res, *p)) // cuidado: orden!! cmp es "menor que"
	    res = p;
    }

    return res;
}




// copy
// ----
template <typename Input_it, typename Output_it>
constexpr Output_it copy(Input_it p, Input_it pe, Output_it q)
{
    for (; p != pe; ++p, ++q)
	*q = *p;

    return q;
}


template <typename Input_it, typename Size, typename Output_it>
inline constexpr Output_it copy_n(Input_it p, Size n, Output_it q)
{
    for (; n > 0; ++p, ++q, --n)
	*q = *p;

    return q;
}

// shift_left
// ----------
template <typename Forward_it>
constexpr Forward_it shift_left(Forward_it p0, Forward_it pe, 
		typename iterator_traits<Forward_it>::difference_type n)
{
    if (n <= 0)
	return pe;

    if (n >= distance(p0, pe))
	return p0;

    Forward_it q0 = p0;
    advance(p0, n);

    for (; p0 != pe; advance(p0, 1), advance(q0, 1))
    {
	*q0 = *p0;
    }

    return q0;
}

// swap
// ----
// Implementado en std_type_traits0.h

// reverse
// -------
template <typename Bidirectional_it>
constexpr void reverse(Bidirectional_it p0, Bidirectional_it pe)
{
    while (true){
	if (p0 == pe) return;

	--pe;
	if (p0 == pe) return;

	swap(*p0, *pe);
	++p0;
    }

// Si fuera Random_it se podría implementar:
//    if (p0 == pe)
//	return;
//
//    --pe; // aquí pe hace las veces de p1
//
//    while (p0 < pe){ <--- pero esto no lo tienen los Bidirectional_it!!!
//	swap(p0, pe);
//	++p0;
//	--pe;
//    }
}


}// namespace

#endif


