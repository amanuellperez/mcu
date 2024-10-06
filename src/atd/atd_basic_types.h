// Copyright (C) 2024 Manuel Perez 
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

#ifndef __ATD_BASIC_TYPES_H__
#define __ATD_BASIC_TYPES_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Veamos los builtin types como clases
 *
 * HISTORIA
 *    Manuel Perez
 *    06/10/2024 view_of(int)
 *
 ****************************************************************************/
#include <limits>

#include "atd_cmath.h"	// atd::abs
#include "atd_concepts.h"

namespace atd{

// View_of_int
// -----------
namespace private_{
template <Type::Integer Int0>
class View_of_int_digit_iterator{
public:
    using Int = std::remove_const_t<Int0>;
    using Digit_type = Int; // TODO: devolver mejor uint16_t o int16_t ?
			    //	    (uint8_t es un char, no un didit :(
			    
    View_of_int_digit_iterator(const Int0& x) : x_{atd::abs(x)} { }
    
    Digit_type operator*();

    void next();
    void operator++() {next();}
    void operator++(int) {next();}

    template <Type::Integer Int2>
    friend bool operator==(const View_of_int_digit_iterator<Int2>&,
	                   const View_of_int_digit_iterator<Int2>&);
private:
    Int x_;   

// Helpers
    static bool is_end(const Int& x)
    {return (x == std::numeric_limits<Int>::max());}

    static void write_end(Int& x)
    {x = std::numeric_limits<Int>::max();}
};


template <Type::Integer Int>
auto View_of_int_digit_iterator<Int>::operator*() -> Digit_type
{
    if (is_end(x_))
	return 0;

    return x_ % Int{10};
}

template <Type::Integer Int>
void View_of_int_digit_iterator<Int>::next()
{
    if (x_ < Int{10})
	write_end(x_);

    else 
	x_ /= Int{10};
}


template <Type::Integer Int>
inline 
bool operator==(const View_of_int_digit_iterator<Int>& a,
	                   const View_of_int_digit_iterator<Int>& b)
{ return a.x_ == b.x_; }

template <Type::Integer Int>
inline 
bool operator!=(const View_of_int_digit_iterator<Int>& a,
	                   const View_of_int_digit_iterator<Int>& b)
{ return !(a == b);}


template <Type::Integer Int>
inline 
View_of_int_digit_iterator<Int> view_of_int_digit_iterator_begin(const Int& x) 
{return View_of_int_digit_iterator<Int>{x};}

template <Type::Integer Int>
inline 
View_of_int_digit_iterator<Int> view_of_int_digit_iterator_end() 
{return View_of_int_digit_iterator<Int>{std::numeric_limits<Int>::max()};}

}// namespace private_
 
// Creamos una view de un int para poder modificarla
template <Type::Integer Int>
class View_of_int{
public:
// Types
    using Digit_iterator = private_::View_of_int_digit_iterator<Int>;

// Constructor
    explicit View_of_int(Int& x) : x_{x} {}


// Iteradores por las cifras
    Digit_iterator digit_begin() const 
    {return private_::view_of_int_digit_iterator_begin<Int>(x_);}

    Digit_iterator digit_end() const 
    {return private_::view_of_int_digit_iterator_end<Int>();}

private:
    Int& x_;
};


}// namespace
 

#endif


