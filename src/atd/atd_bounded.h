// Copyright (C) 2021 Manuel Perez 
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

#ifndef __ATD_TYPES_H__
#define __ATD_TYPES_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Modificaciones que hacemos sobre un tipo.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    21/03/2021 v0.0 Bounded
 *
 ****************************************************************************/

namespace atd{
/*!
 *  \brief  Bounded integer.
 *
 *  Se trata de un número restringido al intervalo [min, max].
 *  Las operaciones garantizan que el número nunca se sale del intervalo.
 *  Como en microcontroladores no lanzamos excepciones, en caso de algún
 *  error se pondrá el número en min o max.
 *
 */
template <typename Int, Int min0, Int max0>
class Bounded{
public:
    static_assert(min0 <= max0, "min has to be less than max");

// constructor
    constexpr Bounded()  = default;

    // conversión de Int <--> Bounded
    constexpr Bounded(const Int& x);
    constexpr operator Int() const {return x_;}

// algebra
    constexpr Bounded& operator+=(const Bounded& b);
    constexpr Bounded& operator-=(const Bounded& b);
    constexpr Bounded& operator*=(const Bounded& b);
    constexpr Bounded& operator/=(const Bounded& b);

    constexpr Bounded& operator++();
    constexpr Bounded operator++(int);

    constexpr Bounded& operator--();
    constexpr Bounded operator--(int);



// info
    constexpr Int min() const {return min0;}
    constexpr Int max() const {return max0;}

private:
    Int x_;   
};

// constructor
template <typename Int, Int m, Int M>
inline constexpr Bounded<Int,m,M>::Bounded(const Int& x)
{
    if (x > max())
	x_ = max();
    else if(x < min())
	x_ = min();
    else
	x_ = x;
}

// algebra
template <typename I, I m, I M>
inline constexpr Bounded<I,m,M>& Bounded<I,m,M>::operator+=(const Bounded& b)
{
    if (x_ + b.x_ <= max())
	x_ += b.x_;
    else
	x_ = max();

    return *this;
}

template <typename I, I m, I M>
inline constexpr Bounded<I, m, M>& Bounded<I, m, M>::
operator-=(const Bounded& b)
{
    if (x_ - b.x_ >= min())
	x_ -= b.x_;
    else
	x_ = min();

    return *this;
}

template <typename Int, Int m, Int M>
inline constexpr Bounded<Int, m, M>& Bounded<Int, m, M>::
operator*=(const Bounded& b)
{
    Int res = x_ * b.x_;
    if (res > max())
	x_ = max();
    else if (res < min())
	x_ = min();
    else
	x_ = res;

    return *this;
}


template <typename Int, Int m, Int M>
inline constexpr Bounded<Int,m,M>& Bounded<Int,m,M>::operator++()
{
    return (*this) += Int{1};
}

template <typename Int, Int m, Int M>
inline constexpr Bounded<Int,m,M> Bounded<Int,m,M>::operator++(int)
{
    Bounded old{*this};
    ++(*this);
    return old;
}


template <typename Int, Int m, Int M>
inline constexpr Bounded<Int,m,M>& Bounded<Int,m,M>::operator--()
{
    return (*this) -= Int{1};
}

template <typename Int, Int m, Int M>
inline constexpr Bounded<Int,m,M> Bounded<Int,m,M>::operator--(int)
{
    Bounded old{*this};
    --(*this);
    return old;
}


}// namespace

#endif


