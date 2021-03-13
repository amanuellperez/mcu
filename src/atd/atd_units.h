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

#ifndef __ATD_UNITS_H__
#define __ATD_UNITS_H__


/****************************************************************************
 *
 *  - DESCRIPCION: Unidades en el S.I.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    13/03/2021 v0.0 Reestructurado. Las 'Units' las necesitan tanto
 *		      atd::Magnitude como atd::ENG_magnitude
 *
 ****************************************************************************/

namespace atd{

/// Unidades en sistema MKS
template <int M, int Kg, int S, int Kelvin>
struct Unit{
    enum {m = M, kg = Kg, s = S, K = Kelvin};
};

// Operations
// Addition: Unit_multiply<U1, U2>
template <typename U1, typename U2>
struct __Unit_multiply{
    using type = Unit<U1::m + U2::m,
		      U1::kg + U2::kg,
		      U1::s + U2::s,
		      U1::K + U2::K
		     >;
};

template <typename U1, typename U2>
using Unit_multiply = typename __Unit_multiply<U1, U2>::type;

// Substraction: Unit_divide<U1, U2>
template <typename U1, typename U2>
struct __Unit_divide{
    using type = Unit<U1::m - U2::m,
		      U1::kg - U2::kg,
		      U1::s - U2::s,
		      U1::K - U2::K
		     >;
};

template <typename U1, typename U2>
using Unit_divide = typename __Unit_divide<U1, U2>::type;


// Inverse: Unit_inverse<U>
template <typename U>
struct __Unit_inverse{
    using type = Unit<-U::m, -U::kg, -U::s, -U::K>;
};

template <typename U>
using Unit_inverse = typename __Unit_inverse<U>::type;


// Different types of units
using Units_scalar      = Unit<0, 0, 0, 0>;
using Units_length      = Unit<1, 0, 0, 0>;
using Units_mass	= Unit<0, 1, 0, 0>;
using Units_time        = Unit<0, 0, 1, 0>;
using Units_temperature = Unit<0, 0, 0, 1>;
using Units_pressure    = Unit<-1, 1, -2, 0>;
using Units_frequency   = Unit<0, 0, -1, 0>;
using Units_velocity    = Unit<1, 0, -1, 0>;


// Symbols
template <typename U>
struct __Unit_symbol;

template <typename U>
inline constexpr const char* Unit_symbol = __Unit_symbol<U>::value;

template <>
struct __Unit_symbol<Units_length>
{ static constexpr const char* value = "m"; };

template <>
struct __Unit_symbol<Units_mass>
{ static constexpr const char* value = "g"; };

template <>
struct __Unit_symbol<Units_time>
{ static constexpr const char* value = "s"; };

template <>
struct __Unit_symbol<Units_temperature>
{ static constexpr const char* value = "K"; };

template <>
struct __Unit_symbol<Units_pressure>
{ static constexpr const char* value = "Pa"; };

template <>
struct __Unit_symbol<Units_frequency>
{ static constexpr const char* value = "Hz"; };

}// namespace

#endif
