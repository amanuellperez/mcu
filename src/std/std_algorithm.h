// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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
 *    A.Manuel L.Perez 
 *	08/02/2019: for_each, for_each_n
 *	16/07/2019: min/max
 *	19/09/2019: copy, copy_n, fill, fill_n
 *	03/11/2019: find, find_if, find_if_not
 *	16/11/2019: count, count_if
 *	04/02/2020: shift_left
 *
 *   - TODO:
 *	- fill_n: especializarla a memset cuando se trate de arrays.
 *
 ****************************************************************************/
#include "std_config.h"

#include "std_iterator.h"

namespace STD{
// ---------------------------------
// non-modifying sequence operations
// ---------------------------------


// for_each
// --------
template <typename InputIt, typename UnaryFunction>
constexpr UnaryFunction for_each(InputIt begin, InputIt end, UnaryFunction f)
{
    for(; begin != end; ++begin)
	f(*begin);

    return f;
}


template <typename InputIt, typename Size, typename UnaryFunction>
constexpr UnaryFunction for_each_n(InputIt begin, Size n, UnaryFunction f)
{
    for(; n != Size{0}; ++begin, --n)
	f(*begin);

    return f;
}


// find
// ----
template <typename InputIt, typename T>
inline constexpr InputIt find(InputIt p0, InputIt pe, const T& value)
{
    while (p0 != pe and *p0 != value)
	++p0;

    return p0;
}

template <typename InputIt, typename UnaryPredicate>
inline constexpr InputIt find_if(InputIt p0, InputIt pe, UnaryPredicate f)
{
    while (p0 != pe and !f(*p0))
	++p0;

    return p0;
}

template <typename InputIt, typename UnaryPredicate>
inline constexpr InputIt find_if_not(InputIt p0, InputIt pe, UnaryPredicate f)
{
    while (p0 != pe and f(*p0))
	++p0;

    return p0;
}


// count
// -----
template <typename InputIt, typename T>
constexpr typename iterator_traits<InputIt>::difference_type
    count (InputIt p0, InputIt pe, const T& value)
{
    typename iterator_traits<InputIt>::difference_type n{0};

    for (; p0 != pe; ++p0){
	if (*p0 == value)
	    ++n;
    }

    return n;
}


template <typename InputIt, typename UnaryPredicate>
constexpr typename iterator_traits<InputIt>::difference_type
    count_if (InputIt p0, InputIt pe, UnaryPredicate pred)
{
    typename iterator_traits<InputIt>::difference_type n{0};

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
template <typename OutputIt, typename T>
inline constexpr OutputIt fill(OutputIt q, OutputIt qe, const T& value)
{
    for (; q != qe; ++q)
	*q = value;

    return q;
}



template <typename OutputIt, typename Size, typename T>
inline constexpr OutputIt fill_n(OutputIt q, Size n, const T& value)
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


template <typename InputIt, typename OutputIt>
constexpr OutputIt copy(InputIt p, InputIt pe, OutputIt q)
{
    for (; p != pe; ++p, ++q)
	*p = *q;

    return q;
}


template <typename InputIt, typename Size, typename OutputIt>
inline constexpr OutputIt copy_n(InputIt p, Size n, OutputIt q)
{
    for (; n > 0; ++p, ++q, --n)
	*q = *p;

    return q;
}


template <typename ForwardIt>
constexpr ForwardIt shift_left(ForwardIt p0, ForwardIt pe, 
		typename iterator_traits<ForwardIt>::difference_type n)
{
    if (n <= 0)
	return pe;

    if (n >= distance(p0, pe))
	return p0;

    ForwardIt q0 = p0;
    advance(p0, n);

    for (; p0 != pe; advance(p0, 1), advance(q0, 1))
    {
	*q0 = *p0;
    }

    return q0;
}



}// namespace

#endif


