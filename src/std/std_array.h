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

#ifndef __MCU_STD_ARRAY_H__
#define __MCU_STD_ARRAY_H__
/****************************************************************************
 *
 *  - DESCRIPCION: El correspondiente a <array>
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *	07/02/2019 v0.0
 *
 ****************************************************************************/


#include "std_config.h"
#include <stdlib.h>   // size_t
#include <cstddef> // std::ptrdiff_t
#include "std_type_traits.h"

namespace STD{

// TODO: faltaría definir una especialización de array<T,0>.
template<typename T, size_t N>
struct array{
    T a[N];

    // tipos
    using value_type	    = T;
    using pointer	    = T*;
    using const_pointer	    = const T*;
    using reference	    = T&;
    using const_reference   = const T&;
    using size_type	    = size_t;
    using difference_type   = std::ptrdiff_t;
    using iterator	    = T*;
    using const_iterator    = const T*;
    // TODO: definir reverse_iterator
//    using reverse_iterator  = reverse_iterator<iterator>;
//    using const_reverse_iterator  = reverse_iterator<const_iterator>;

    void fill(const T& u)
    {
	for (size_type i = 0; i < N; ++i)
	    a[i] = u;
    }


    // TODO: swap

    // iterators
    constexpr iterator begin() noexcept {return &a[size_type(0)];}
    constexpr iterator end() noexcept {return &a[N];}

    constexpr const_iterator begin() const noexcept 
    {return &a[size_type(0)];}

    constexpr const_iterator end() const noexcept 
    {return &a[N];}

    // TODO: rbegin/rend
    // TODO: cbegin/cend

    
    // capacity:
    constexpr bool empty() const noexcept
    { return size() == size_type(0); }

    constexpr size_type size() const noexcept 
    {return N;}

    constexpr size_type max_size() const noexcept 
    {return size();}

    // elemental access:
    constexpr reference operator[](size_type n)
    {return a[n];}

    constexpr const_reference operator[](size_type n) const
    {return a[n];}

    // No defino at(), ya que en un micro no se pueden lanzar excepciones

    constexpr reference front()
    {return a[size_type(0)];}

    constexpr const_reference front() const
    {return a[size_type(0)];}

    constexpr reference back()
    {return a[N - size_type(1)];}

    constexpr const_reference back() const
    {return a[N - size_type(1)];}


    constexpr T* data() noexcept
    {return &a[0];}

    constexpr const T* data() const noexcept
    {return &a[0];}

};


// deduction guides
template <typename T, typename... U>
array(T, U...) -> array<enable_if_t<(is_same_v<T, U> && ...), T>,
			1 + sizeof...(U)>;

}// namespace

#endif


