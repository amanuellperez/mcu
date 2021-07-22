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

#ifndef __ATD_BUFFER_H__
#define __ATD_BUFFER_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Tenemos los siguientes tipos de arrays:
 *	std::array : array de longitud fija.
 *	std::vector: array de longitud variable, memoria redimensionable.
 *
 *    Necesito un intermedio, un array de longitud variable pero cuya
 *    capacidad sea fija. ¿Cómo llamarlo? De momento lo voy a llamar `Buffer`
 *    (a fin de cuentas un buffer encaja dentro de esta idea). 
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    17/07/2021 v0.0
 *
 ****************************************************************************/
#include <stdlib.h>   // size_t
#include <cstddef> // std::ptrdiff_t
#include <type_traits>
#include <initializer_list>
#include <algorithm>

namespace atd{

template<typename T, size_t N>
class Buffer {
public:
// Types
    using value_type	    = T;
    using pointer	    = T*;
    using const_pointer	    = const T*;
    using reference	    = T&;
    using const_reference   = const T&;
    using size_type	    = size_t;
    using difference_type   = std::ptrdiff_t;
    using iterator	    = T*;
    using const_iterator    = const T*;
 
    // Construction
    constexpr Buffer() : i_{0} {}
    constexpr Buffer(std::initializer_list<T> v)
    {
	i_ = std::min(v.size(), N);

	std::copy_n(v.begin(), i_, a);
    }


    /// Appends the given element to the end of the container.
    /// Es responsabilidad del usuario garantizar que size() != max_size()
    constexpr void push_back(const T& x);

    /// Removes the last element of the container.
    /// Es responsabilidad del usuario garantizar que size() > 0
    constexpr void pop_back() {--i_;}

    /// Erases all elements from the container.
    constexpr void clear() {i_ = 0;}

    // iterators
    constexpr iterator begin() noexcept {return &a[size_type(0)];}
    constexpr iterator end() noexcept {return &a[i_];}

    constexpr const_iterator begin() const noexcept 
    {return &a[size_type(0)];}

    constexpr const_iterator end() const noexcept 
    {return &a[i_];}


    
    // capacity:
    constexpr bool empty() const noexcept
    { return size() == size_type(0); }

    constexpr size_type size() const noexcept 
    {return i_;}

    constexpr size_type max_size() const noexcept 
    {return N;}

    // elemental access:
    constexpr reference operator[](size_type n)
    {return a[n];}

    constexpr const_reference operator[](size_type n) const
    {return a[n];}



    // Los datos válidos serán de [0, size()) = [begin, end)
    constexpr T* data() noexcept
    {return &a[0];}

    constexpr const T* data() const noexcept
    {return &a[0];}


private:
    T a[N];
    size_t i_;	// index of the last element
};


template<typename T, size_t N>
inline constexpr void Buffer<T,N>::push_back(const T& x)
{
    a[i_] = x;
    ++i_;
}

//// deduction guides
//template <typename T, typename... U>
//Buffer(T, U...) -> Buffer<std::enable_if_t<(std::is_same_v<T, U> && ...), T>,
//			1 + sizeof...(U)>;

}// namespace




#endif


