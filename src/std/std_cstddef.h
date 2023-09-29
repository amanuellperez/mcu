// Copyright (C) 2019-2020 Manuel Perez 
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

#ifndef __MTD_STD_CSTDDEF_H__
#define __MTD_STD_CSTDDEF_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Equivalente a cstddef
 *
 *   - COMENTARIOS: Incluye std::byte
 *
 *   - HISTORIA:
 *	Manuel Perez 
 *	06/02/2019 byte
 *	29/09/2023 nullptr_t
 *
 *
 ****************************************************************************/
#include "std_config.h"

#include <stddef.h> // ptrdiff_t


namespace STD{

using ::ptrdiff_t;
using ::size_t;
using nullptr_t = decltype(nullptr);

/***************************************************************************
 *				byte
 ***************************************************************************/
// Tipo que usa byte
using __byte_type = unsigned char;

/// std::byte
enum class byte: __byte_type {};

/// Equivalent to: return Integer(b)
template<typename Integer>
inline constexpr Integer to_integer(byte b) noexcept
{ return static_cast<Integer>(b); }

// Operaciones básicas con bytes
/// a or b
inline constexpr byte operator|(byte a, byte b) noexcept
{ return byte(static_cast<__byte_type>(a) | static_cast<__byte_type>(b)); }

/// a and b
inline constexpr byte operator&(byte a, byte b) noexcept
{ return byte(static_cast<__byte_type>(a) & static_cast<__byte_type>(b)); }

/// a xor b
inline constexpr byte operator^(byte a, byte b) noexcept
{ return byte(static_cast<__byte_type>(a) ^ static_cast<__byte_type>(b)); }

/// a |= b
inline constexpr byte& operator|=(byte& a, byte b) noexcept
{ return a = (a | b);}

/// a &= b
inline constexpr byte operator&=(byte& a, byte b) noexcept
{ return a = (a & b);}

/// a ^= b
inline constexpr byte operator^=(byte& a, byte b) noexcept
{ return a = (a ^ b);}

/// not b
inline constexpr byte operator~(byte b) noexcept
{ return byte(~static_cast<__byte_type>(b)); }

// operator<<
template <typename Integer>
inline constexpr byte operator<<(byte b, Integer shift) noexcept
{return byte(static_cast<__byte_type>(b) << shift);}

// operator>>
template <typename Integer>
inline constexpr byte operator>>(byte b, Integer shift) noexcept
{return byte(static_cast<__byte_type>(b) >> shift);}


// operator<<=
template <typename Integer>
inline constexpr byte& operator<<=(byte& b, Integer shift) noexcept
{return b = (b << shift);}

// operator>>=
template <typename Integer>
inline constexpr byte& operator>>=(byte& b, Integer shift) noexcept
{return b = (b >> shift);}



}// namespace

#endif


