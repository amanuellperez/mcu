// Copyright (C) 2020 Manuel Perez 
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

#ifndef __MCU_STD_INITIALIZER_LIST_H__
#define __MCU_STD_INITIALIZER_LIST_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Equivalent to <initializer_list>
 *
 *  - WARNING: Right now it's not tested!!! (I don't know how to test this 
 *	class using gcc)
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    25/01/2020 v0.0
 *
 ****************************************************************************/
#include "std_config.h"

#include "std_cstddef.h"    // size_t

namespace STD{

template <typename T>
class initializer_list{
public:
    using value_type      = T;
    using reference       = const T&;
    using const_reference = const T&;
    using size_type       = size_t;

    using iterator       = const T*;
    using const_iterator = const T*;

    constexpr initializer_list() noexcept
	    : p_{0}, size_{0} {}

    constexpr size_type size() const noexcept { return size_; }

    constexpr const_iterator begin() const noexcept { return p_; }

    constexpr const_iterator end() const noexcept { return begin() + size(); }

private:
    const_iterator p_;
    size_type size_;

    // ¿Por qué funciona esta clase? Magia!!! Parece ser que esta clase
    // realmente pertenece al "lenguaje" de C++ y por tanto el compilador 
    // la reconoce como especial. De hecho no funciona si se intenta compilar
    // dentro del namespace mtd ya que no lo reconoce el compilador gcc. 
    // The compiler can call a private constructor.
    constexpr initializer_list(const_iterator p, size_type sz) noexcept
    : p_(p), size_(sz) { }
};


// Initializer list rang access
// ----------------------------
template <typename T>
constexpr const T* begin(initializer_list<T> il) noexcept
{
    return il.begin();
}

template <typename T>
constexpr const T* end(initializer_list<T> il) noexcept
{
    return il.end();
}



}// namespace

#endif


