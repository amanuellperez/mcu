// Copyright (C) 2022 Manuel Perez 
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

#ifndef __MCU_STD_SPAN_H__
#define __MCU_STD_SPAN_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	El correspondiente a <span>
 *
 * HISTORIA
 *    Manuel Perez
 *    23/12/2022 Versión mínima
 *
 ****************************************************************************/
#include "std_config.h"
#include "std_type_traits.h"
#include "std_array.h"

#include <cstddef>

namespace STD{

// span
// ----
//  std::span es una view de un array. Tenemos dos posibles span:
//  
//  (1) span<int, 10>(x);
//	Se trata de un array de 10 elementos, donde el 10 lo conocemos en
//	tiempo de compilación.
//
//  (2) span<int>(x, 10);
//	Idem que (1) pero el 10 lo conocemos en tiempo de ejecución.
//
//  El '10' es el size() del array. Desconozco por qué en lugar de llamarlo
//  size lo llaman extent.
//
inline constexpr size_t dynamic_extent = static_cast<size_t>(-1);

namespace __std{

// Number que podemos conocer en tiempo de compilación o en tiempo de
// ejecución. ¿Llamarlo mejor Number?
//
// if (x0 == -1)
//	Number	
//	{ 
//	    value() {return x_;}   
//	    contiene variable x_; 
//	}
//  else
//	Number
//	{ 
//	    value() { return x0; }
//	}
template <size_t x0>
class Number{
public: 
    constexpr Number(size_t) noexcept { }

    static constexpr size_t value() noexcept
    { return x0;}
};


template <>
class Number<dynamic_extent>{
public:
    constexpr Number(size_t x0) noexcept 
    : x_{x0}
    { }

    constexpr size_t value() const noexcept
    { return x_;}

private:
    size_t x_;
};



} // namespace __std
  


template <typename T, size_t size0 = dynamic_extent>
class span{
public:
// Types
    using element_type	= T;
    using value_type	= remove_cv_t<T>;
    using size_type	= size_t;
    using difference_type   = ptrdiff_t;
    using pointer	= T*;
    using const_pointer	= const T*;
    using reference	= T&;
    using const_reference   = const T&;
    using iterator	= T*;
//  using reverse_iterator

// Member constant
    static constexpr size_t extent = size0;

// Constructor
    constexpr span() noexcept
	requires (extent == 0 || extent == dynamic_extent)
	: ptr_{nullptr}, size_{0} { }


    template <size_type N>
	requires (extent == dynamic_extent || N == extent)
	// TODO: require remove_pointer_t <...>
    constexpr span(type_identity_t<element_type> (&x)[N]) noexcept
		: ptr_{static_cast<pointer>(x)}, size_{N} { }

    template <typename U, size_t N>
	requires (extent == dynamic_extent || N == extent)
	// TODO: require remove_pointer_t <...>
    constexpr span(array<U, N>& x) noexcept
	: ptr_{x.data()}, size_{x.size()} 
    { }

// Iterators
    constexpr iterator begin() const noexcept {return ptr_; }
    constexpr iterator end() const noexcept { return ptr_ + size(); }
    
// Element access
    constexpr reference operator[](size_type i) const { return ptr_[i]; }
    constexpr pointer data() const noexcept { return ptr_; }

// Observers
    constexpr size_type size() const noexcept {return size_.value(); }
    constexpr size_type size_bytes() const noexcept
    { 
	return size() * sizeof(element_type); 
    }

    [[nodiscard]] constexpr bool empty() const noexcept{ return size() == 0; }

private:
   pointer ptr_;
   __std::Number<size0> size_;
};


// Deduction guides
// ----------------
template <typename T, size_t N>
span(T (&)[N]) -> span<T, N>;

}// namespace

#endif


