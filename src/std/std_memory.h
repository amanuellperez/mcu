// Copyright (C) 2023 Manuel Perez 
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

#ifndef __MCU_STD_MEMORY_H__
#define __MCU_STD_MEMORY_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	El correspondiente a <memory>
 *
 * HISTORIA
 *    Manuel Perez
 *    27/08/2023 to_address, pointer_traits (solo especialización con punteros)
 *    28/09/2023 addressof
 *
 ****************************************************************************/
#include "std_config.h"
#include "std_type_traits.h"	

namespace STD{

// pointer_traits
// --------------
template <typename P>
struct pointer_traits{
    using pointer = P;
// TODO ...
};

template <typename T>
struct pointer_traits<T*>{
    using pointer	  = T*;
    using element_type    = T;
    using difference_type = ptrdiff_t;

    template <typename U>
    using rebind = U*;

    static constexpr pointer pointer_to(element_type& r) noexcept
    { return &r;}
};


// to_address
// ----------
template<typename T>
constexpr T* to_address(T* p) noexcept
{
    static_assert(!is_function_v<T>);
    return p;
}
 
template<typename T>
constexpr auto to_address(const T& p) noexcept
{
    if constexpr (requires{ pointer_traits<T>::to_address(p); })
        return pointer_traits<T>::to_address(p);
    else
        return to_address(p.operator->());
}

// addressof
// ---------
// Iba a basar la implementación en la propuesta por cppreference, 
// pero en la propia página de cppreference dice:
// "Correct implementation of this function requires compiler support"
// Así que tengo que usar __builtin_addressof que no es estandar.
template <typename T>
inline T* addressof(T& r) noexcept
{
    return __builtin_addressof(r);
//    if constexpr (is_object_v<T>){
//	return reinterpreted_cast<T*>(
//			&const_cast<char&>(
//			    reinterpreted_cast<const volatile char&>(r)));
//    }
//
//    else    
//	return &r;
}


template <typename T>
const T* addressof(const T&&) = delete;

}// namespace


#endif


